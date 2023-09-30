#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "llama.h"
#include "grammar-parser.h"

#ifdef __cplusplus
extern "C" {
#endif
  struct llama_sampler_params {
    float   temp;
    float   repeat_penalty;
    int32_t repeat_last_n;
    float   frequency_penalty;
    float   presence_penalty;
    int32_t mirostat;
    float   mirostat_tau;
    float   mirostat_eta;
  };

  llama_sampler_params  llama_sampler_default_params();

  struct llama_grammar * llama_cached_parse_grammar(const char * grammar_str);

  llama_token llama_grammar_sample_token(llama_context * ctx,
                                         llama_grammar * grammar,
                                         llama_sampler_params params,
                                         llama_token_data_array * cur_p,
                                         bool reset);



#ifdef __cplusplus
}
#endif

#endif // GRAMMAR_H
