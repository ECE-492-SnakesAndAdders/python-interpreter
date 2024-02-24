/*********************************************************************************
* Description: The parser (the second stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "parser.h"
#include "lexer.h"
#include "expr.h"


/** precedence levels of operators are as follows (from lowest to highest):
    https://docs.python.org/3/reference/grammar.html
        logical or (OR) [disjunction]
        logical and (AND) [conjunction]
        logical not (NOT) [inversion]
        comparison/identity/membership (==, !=, >, >=, <, <=, is, is not, in, not in) [comparison]
        bitwise or (|) [bor]
        bitwise xor (^) [bxor]
        bitwise and (&) [band]
        bitwise shift (>>, <<) [shift]
        add/subtract (+, -) [sum]
        multiply/divide (*, /, //, %, @) [term]
        unary (+, -, ~) [factor]
        exponent (**) [power]
    all operators in the same tier are left-associative, except for power
*/


/**
 * \brief Basic constructor for the parser.
 * \param [in] input Pointer to the token list returned by the lexer.
 */
Parser::Parser(lexed_command * input) {
    command_info = *input;
}


/**
 * \brief Starts the chain of parsing a Python expression.
 * \return The internal representation of the expression.
 */
Expr Parser::expression() {
    return disjunction();
}


/**
 * \brief Handles the lowest priority operator: "or"/"OR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::disjunction() {
    Expr expr = conjunction();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "and"/"AND".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::conjunction() {
    Expr expr = inversion();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "not"/"NOT".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::inversion() {
    Expr expr = comparison();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "=="/"EQUAL", "!="/"N_EQUAL", ">"/"GREATER",
 *                                                   ">="/"G_EQUAL", "<"/"LESS", "<="/"L_EQUAL",
 *                                                   "is"/"IS", "is not"/"", "in"/"IN", "not in"/"") .
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::comparison() {
    Expr expr = bor();
    // TODO: fill in details
    // TODO: handle cascaded comaprison operators properly
    // TODO: handle "is not" and "not in" keywords
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "|"/"B_OR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::bor() {
    Expr expr = bxor();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "^"/"B_XOR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::bxor() {
    Expr expr = band();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "&"/"B_AND".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::band() {
    Expr expr = shift();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "<<"/"B_SLL", ">>"/"B_SAR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::shift() {
    Expr expr = sum();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "+"/"PLUS", "-"/"MINUS".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::sum() {
    Expr expr = term();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "*"/"STAR", "/"/"SLASH", "//"/"D_SLASH", "%"/"PERCENT", "@"/"AT".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::term() {
    Expr expr = factor();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "+"/"PLUS", "-"/"MINUS", "~"/"B_NOT".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::factor() {
    Expr expr = power();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "**"/"D_STAR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::power() {
    Expr expr = primary();
    // TODO: fill in details
    return expr;
}


/**
 * \brief Handles literal values: "STRING", "NUMBER", "TRUE", "FALSE", "NONE".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::primary() {
    Expr expr = Expr();
    // TODO: base case of recursion
    return expr;
}


/**
 * \brief Parses the input tokens into an expression.
 * \return The internal representation of the expression.
 */
Expr Parser::parse_input() {
    // TODO: handle more complex inputs (statements, blocks, ...)
    return expression();
}
