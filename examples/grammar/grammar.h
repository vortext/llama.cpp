#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#include "llama.h"
#include "grammar-parser.h"

#ifdef __cplusplus
extern "C" {
#endif
  // llama_sampler.h

#pragma once


  struct llama_sampler_params {
    float   temp = 0.80f;               // Temperature
    float   repeat_penalty = 1.10f;     // Penalty for repeated tokens
    int32_t repeat_last_n = 64;         // Number of tokens to consider for repeat penalty
    float   frequency_penalty = 0.00f;  // Penalty for frequent tokens
    float   presence_penalty = 0.00f;   // Penalty for present tokens
    int32_t mirostat = 2;               // Mirostat version (0 = disabled, 1 = mirostat, 2 = mirostat 2.0)
    float   mirostat_tau = 5.00f;       // Mirostat target entropy
    float   mirostat_eta = 0.10f;       // Mirostat learning rate
    int32_t top_k = 40;                 // Top-K for sampling
    float   top_p = 0.95f;              // Top-P for sampling
    float   tfs_z = 1.0f;               // TFS-Z value
    float   typical_p = 1.0f;           // Typical-P value
    int32_t n_probs = 0;                // Number of probabilities to output (0 for no output)
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
