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


/**
 * \brief The parser of the interpreter; it takes a token list as input and a syntax tree as output.
 */
class Parser {
    private:
        // input information
        lexed_command * command_info;
        // the current token index being read
        uint16_t current = 0;
        uint16_t current_str_lit = 0;
        uint16_t current_num_lit = 0;
        // for parsing expressions recursively
        Expr expression();
        Expr disjunction();
        Expr conjunction();
        Expr inversion();
        Expr comparison();
        Expr bor();
        Expr bxor();
        Expr band();
        Expr shift();
        Expr sum();
        Expr term();
        Expr factor();
        Expr power();
        Expr primary();
        // checks the current token and sees if it matches a desired value
        bool current_matches(lexemes token);
        lexemes current_token();
        lexemes previous_token();
        void advance_current();
        // checks if there are any more characters to be read
        bool end_reached();

    public:
        // basic constructor for the class
        Parser(lexed_command * input);
        // converts the input token list into a syntax tree
        Expr parse_input();
};

#endif
