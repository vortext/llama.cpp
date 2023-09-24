#include "grammar.h"

struct llama_grammar * llama_parse_grammar(const char * grammar_str) {
    struct llama_grammar * grammar = NULL;
    grammar_parser::parse_state parsed_grammar;

    parsed_grammar = grammar_parser::parse(grammar_str);
    grammar_parser::print_grammar(stderr, parsed_grammar);

    std::vector<const llama_grammar_element *> grammar_rules(parsed_grammar.c_rules());
    grammar = llama_grammar_init(grammar_rules.data(), grammar_rules.size(), parsed_grammar.symbol_ids.at("root"));

    return grammar;
}
