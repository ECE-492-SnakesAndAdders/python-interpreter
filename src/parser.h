/*********************************************************************************
* Description: The parser (the second stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef PARSER_H
#define PARSER_H


#include "expr.h"
#include "lexer.h"


#ifndef MAX_LIT_LEN
#define MAX_LIT_LEN 32
#endif

#ifndef MAX_NUM_NODES
#define MAX_NUM_NODES 64
#endif


/**
 * \brief The parser of the interpreter; it takes a token list as input and a syntax tree as output.
 */
class Parser {
    private:
        // input information
        lexed_command command_info;
        // the current indices of command information being read
        uint16_t current = 0;
        uint16_t current_str_lit = 0;
        uint16_t current_num_lit = 0;
        // output tree information and operations
        node tree_nodes[MAX_NUM_NODES];
        uint16_t current_node = 0;
        node * write_new_node(node * value);
        // for parsing expressions recursively
        node * expression();
        node * disjunction();
        node * conjunction();
        node * inversion();
        node * comparison();
        node * bor();
        node * bxor();
        node * band();
        node * shift();
        node * sum();
        node * term();
        node * factor();
        node * power();
        node * primary();
        // checks the current token and sees if it matches a desired value
        bool current_matches(lexemes token);
        // fetches tokens of interest
        lexemes current_token();
        lexemes previous_token();
        // safe way of considering next token
        void advance_current();
        // checks if there are any more characters to be read
        bool end_reached();

    public:
        // basic constructor for the class
        Parser(lexed_command input);
        // converts the input token list into a syntax tree
        node * parse_input();
};


#endif
