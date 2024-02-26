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
    command_info = input;
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
    /** general strategy for all levels is as follows:
     *    recurse down, allowing higher priority operators to start, then
     *    deal with current operator as it arises, then
     *    recurse down for subsequent operand, then
     *    combine these two operands with the current operator
     */
    // recurse to higher priority operators
    Expr expr = conjunction();
    // deal with any OR operators
    while (current_matches(OR)) {
        // find the operation to be completed
        lexemes opcode = current_token();
        // recurse again to get the right operand
        Expr right = conjunction();
        // combine the operands, building up the syntax tree
        expr = Binary(expr, opcode, right);
    }
    // combined nested expressions is the new expression
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "and"/"AND".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::conjunction() {
    Expr expr = inversion();
    while (current_matches(AND)) {
        lexemes opcode = current_token();
        Expr right = inversion();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "not"/"NOT".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::inversion() {
    // TODO: deal with unary operator
    Expr expr = comparison();
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
    // TODO: handle cascaded comaprison operators properly
    // TODO: handle "is not" and "not in" keywords
    while (current_matches(EQUAL) || current_matches(N_EQUAL) || current_matches(GREATER) ||
           current_matches(G_EQUAL) || current_matches(LESS) || current_matches(L_EQUAL) ||
           current_matches(IS) || current_matches(IN)) {
        lexemes opcode = current_token();
        Expr right = bor();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "|"/"B_OR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::bor() {
    Expr expr = bxor();
    while (current_matches(B_OR)) {
        lexemes opcode = current_token();
        Expr right = bxor();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "^"/"B_XOR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::bxor() {
    Expr expr = band();
    while (current_matches(B_XOR)) {
        lexemes opcode = current_token();
        Expr right = band();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "&"/"B_AND".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::band() {
    Expr expr = shift();
    while (current_matches(B_AND)) {
        lexemes opcode = current_token();
        Expr right = shift();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "<<"/"B_SLL", ">>"/"B_SAR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::shift() {
    Expr expr = sum();
    while (current_matches(B_SLL) || current_matches(B_SAR)) {
        lexemes opcode = current_token();
        Expr right = sum();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "+"/"PLUS", "-"/"MINUS".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::sum() {
    // TODO: deal with unary versions
    Expr expr = term();
    while (current_matches(PLUS) || current_matches(MINUS)) {
        lexemes opcode = current_token();
        Expr right = term();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "*"/"STAR", "/"/"SLASH", "//"/"D_SLASH", "%"/"PERCENT", "@"/"AT".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::term() {
    Expr expr = factor();
    while (current_matches(STAR) || current_matches(SLASH) || current_matches(D_SLASH) ||
           current_matches(PERCENT) || current_matches(AT)) {
        lexemes opcode = current_token();
        Expr right = factor();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "+"/"PLUS", "-"/"MINUS", "~"/"B_NOT".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::factor() {
    // TODO: deal with binary versions
    Expr expr = power();
    while (current_matches(PLUS) || current_matches(MINUS) || current_matches(B_NOT)) {
        lexemes opcode = current_token();
        Expr right = factor();
        expr = Unary(opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "**"/"D_STAR".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::power() {
    Expr expr = primary();
    while (current_matches(D_STAR)) {
        lexemes opcode = current_token();
        Expr right = primary();
        expr = Binary(expr, opcode, right);
    }
    return expr;
}


/**
 * \brief Handles literal values: "STRING", "NUMBER", "TRUE", "FALSE", "NONE".
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::primary() {
    // TODO: set up data types so that any value can be used
    // base case deals with literal values and parentheses
    // sentinel literal values
    if (current_matches(FALSE)) {
        return Literal(false);
    } else if (current_matches(NONE)) {
        return Literal(NULL);
    } else if (current_matches(TRUE)) {
        return Literal(true);
    }
    // number and string literal values that need to be fetched
    if (current_matches(NUMBER)) {
        // TODO: logic to get the current number value from the struct
        return Literal(NULL);
    } else if (current_matches(STRING)) {
        // TODO: logic to get the current string value from the struct
        return Literal(NULL);
    }
    // deal with parentheses
    if (current_matches(L_PAREN)) {
        // any general expression can be nested in parentheses
        Expr expr = expression();
        // TODO: validate and consume closing parenthesis
    }
}


/**
 * \brief Determines if the current token being parsed is some particular token. Consumes it if so.
 * \param [in] token The token type to check equality with.
 * \return True if the token does match; false otherwise.
 */
bool Parser::current_matches(lexemes token) {
    if (end_reached()) {
        return false;
    }
    if ((command_info -> tokens[current]) == token) {
        // must consume this token and move along (assumption of calling code)
        advance_current();
        return true;
    }
    return false;
}


/**
 * \brief Provides the current token being parsed.
 * \return The token currently beng parsed.
 */
lexemes Parser::current_token() {
    if (end_reached()) {
        return EMPTY;
    }
    return command_info -> tokens[current];
}


/**
 * \brief Provides the token before the one being parsed.
 * \return The token prior to the one currently being parsed.
 */
lexemes Parser::previous_token() {
    if (current == 0) {
        return EMPTY;
    }
    return command_info -> tokens[current - 1];
}


/**
 * \brief Increments the pointer to the current token being parsed.
 */
void Parser::advance_current() {
    if (!end_reached()) {
        current++;
    }
}


/**
 * \brief Helper function to determine if we are at the token list's end or not.
 * \return True if the end has been reached; false otherwise.
 */
bool Parser::end_reached() {
    return current >= (command_info -> token_count);
}


/**
 * \brief Parses the input tokens into an expression.
 * \return The internal representation of the expression.
 */
Expr Parser::parse_input() {
    // TODO: handle more complex inputs (statements, blocks, ...)
    return expression();
}
