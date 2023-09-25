#include "grammar.h"
#include <unordered_map>
#include <string>
#include <vector>

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
