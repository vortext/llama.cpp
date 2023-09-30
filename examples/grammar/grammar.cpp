#include "grammar.h"
#include <stdlib.h>

struct llama_grammar * llama_cached_parse_grammar(const char * grammar_str) {
  static std::unordered_map<std::string, grammar_parser::parse_state> parsed_grammar_cache;
  std::string key = grammar_str;

  auto it = parsed_grammar_cache.find(key);
  grammar_parser::parse_state parsed_grammar;
  if (it != parsed_grammar_cache.end()) {
    // Use cached parsed grammar
    parsed_grammar = it->second;
  } else {
    // Parse and cache the result
    parsed_grammar = grammar_parser::parse(grammar_str);
    parsed_grammar_cache[key] = parsed_grammar;

    // Optionally print the grammar
    grammar_parser::print_grammar(stderr, parsed_grammar);
  }

  std::vector<const llama_grammar_element *> grammar_rules(parsed_grammar.c_rules());

  struct llama_grammar * grammar = NULL;
  grammar = llama_grammar_init(grammar_rules.data(), grammar_rules.size(), parsed_grammar.symbol_ids.at("root"));

  return grammar;
}

struct llama_sampler_params llama_sampler_default_params() {
  struct llama_sampler_params result = {
    0.80f, // temp;
    1.10f, // repeat_penalty
    64, // last_n_repeat
    0.00f, // frequency_penalty
    0.00f, // presence_penalty
    2, // mirostat
    5.00f, // mirostat_tau
    0.10f, // mirostat_eta
  };
  return result;
}

llama_token llama_grammar_sample_token(struct llama_context * ctx,
                                       struct llama_grammar * grammar,
                                       struct llama_sampler_params params,
                                       struct llama_token_data_array  * cur_p,
                                       bool reset) {

  const int n_ctx = llama_n_ctx(ctx);

  static std::vector<llama_token> last_tokens(n_ctx);
  std::fill(last_tokens.begin(), last_tokens.end(), 0);

  if (reset) {
    // Clear last_tokens vector
    last_tokens.clear();
    last_tokens.resize(n_ctx, 0);
  }

  const float   temp            = params.temp;
  const int32_t repeat_last_n   = params.repeat_last_n < 0 ? n_ctx : params.repeat_last_n;
  const float   repeat_penalty  = params.repeat_penalty;
  const float   alpha_presence  = params.presence_penalty;
  const float   alpha_frequency = params.frequency_penalty;
  const int     mirostat        = params.mirostat;
  const float   mirostat_tau    = params.mirostat_tau;
  const float   mirostat_eta    = params.mirostat_eta;

  llama_token id = 0;

  // apply penalties
  if (!last_tokens.empty()) {
    const int last_n_repeat = std::min(std::min((int)last_tokens.size(), repeat_last_n), n_ctx);

    llama_sample_repetition_penalty(ctx, cur_p,
                                    last_tokens.data() + last_tokens.size() - last_n_repeat,
                                    last_n_repeat, repeat_penalty);
    llama_sample_frequency_and_presence_penalties(ctx, cur_p,
                                                  last_tokens.data() + last_tokens.size() - last_n_repeat,
                                                  last_n_repeat, alpha_frequency, alpha_presence);

  }

  if (grammar != NULL) {
    llama_sample_grammar(ctx, cur_p, grammar);
  }

  if (temp <= 0) {
    // Greedy sampling
    id = llama_sample_token_greedy(ctx, cur_p);
  } else {
    if (mirostat == 1) {
      static float mirostat_mu = 2.0f * mirostat_tau;
      const int mirostat_m = 100;
      llama_sample_temperature(ctx, cur_p, temp);
      id = llama_sample_token_mirostat(ctx, cur_p, mirostat_tau, mirostat_eta, mirostat_m, &mirostat_mu);
    } else if (mirostat == 2) {
      static float mirostat_mu = 2.0f * mirostat_tau;
      llama_sample_temperature(ctx, cur_p, temp);
      id = llama_sample_token_mirostat_v2(ctx, cur_p, mirostat_tau, mirostat_eta, &mirostat_mu);
    }
  }
  // printf("`%d`", candidates_p.size);

  if (grammar != NULL) {
    llama_grammar_accept_token(ctx, grammar, id);
  }

  last_tokens.erase(last_tokens.begin());
  last_tokens.push_back(id);

  return id;
}
