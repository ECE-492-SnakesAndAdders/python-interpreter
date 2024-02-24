/*********************************************************************************
* Description: The parser (the second stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef PARSER_H
#define PARSER_H


#include "lexer.h"
#include "expr.h"


/**
 * \brief The parser of the interpreter; it takes a token list as input and a syntax tree as output.
 */
class Parser {
    private:
        // input information
        lexed_command command_info;
        // the current token index being read
        uint16_t current = 0;
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

    public:
        // basic constructor for the class
        Parser(lexed_command * input);
        // converts the input token list into a syntax tree
        Expr parse_input();
};

#endif
