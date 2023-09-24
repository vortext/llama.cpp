#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "llama.h"
#include "grammar-parser.h"

#ifdef __cplusplus
extern "C" {
#endif

  struct llama_grammar * llama_parse_grammar(const char * grammar_str);

#ifdef __cplusplus
}
#endif

#endif // GRAMMAR_H
