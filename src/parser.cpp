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


/** Much of this code is based on Crafting Interpreters by Robert Nystrom
    https://craftinginterpreters.com/representing-code.html, https://craftinginterpreters.com/parsing-expressions.html
*/


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
Parser::Parser(lexed_command input) {
    xpd_puts("HERE IN PARSER()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
    command_info = input;
}


/**
 * \brief Starts the chain of parsing a Python expression.
 * \return The internal representation of the expression.
 */
Expr Parser::expression() {
    xpd_puts("HERE IN EXPRESSION()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN DISJUNCTION()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN CONJUNCTION()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN INVERSION()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
    Expr expr = comparison();
    while (current_matches(NOT)) {
        lexemes opcode = current_token();
        Expr right = inversion();
        expr = Unary(opcode, right);
    }
    return expr;
}


/**
 * \brief Handles the next lowest priority operator: "=="/"EQUAL", "!="/"N_EQUAL", ">"/"GREATER",
 *                                                   ">="/"G_EQUAL", "<"/"LESS", "<="/"L_EQUAL",
 *                                                   "is"/"IS", "is not"/"", "in"/"IN", "not in"/"") .
 * \return The internal representation of the expression parsed so far.
 */
Expr Parser::comparison() {
    xpd_puts("HERE IN COMPARISON()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN BOR()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN BXOR()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN BAND()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN SHIFT()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN SUM()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
    Expr expr = term();
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
    while (current_matches(PLUS) || current_matches(MINUS)) {
        xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
        xpd_echo_int(previous_token(), XPD_Flag_UnsignedDecimal);
        xpd_puts("HERE in (+)\n");
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
    xpd_puts("HERE IN TERM()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN FACTOR()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
    // TODO: deal with binary versions
    Expr expr = power();
    while (current_matches(PLUS) || current_matches(MINUS) || current_matches(B_NOT)) {
        xpd_puts("HERE in (+)\n");
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
    xpd_puts("HERE IN POWER()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
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
    xpd_puts("HERE IN PRIMARY()\n");
    xpd_echo_int(current_token(), XPD_Flag_UnsignedDecimal);
    // TODO: set up data types so that any value can be used
    // base case deals with literal values and parentheses
    // sentinel literal values
    literal_value lit;
    if (current_matches(FALSE)) {
        lit.type = FALSE_VALUE;
        return Literal(lit);
    } else if (current_matches(NONE)) {
        lit.type = NONE_VALUE;
        return Literal(lit);
    } else if (current_matches(TRUE)) {
        lit.type = TRUE_VALUE;
        return Literal(lit);
    }
    // number and string literal values that need to be fetched
    if (current_matches(NUMBER)) {
        xpd_puts
        lit.type = NUMBER_VALUE;
        lit.data.number = command_info.num_lits[current_num_lit];
        current_num_lit++;
        return Literal(lit);
    } else if (current_matches(STRING)) {
        lit.type = STRING_VALUE;
        char * temp = command_info.str_lits[current_str_lit];
        for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
            lit.data.string[i] = *(temp + i);
        }
        current_str_lit++;
        return Literal(lit);
    }
    // deal with parentheses
    if (current_matches(L_PAREN)) {
        // any general expression can be nested in parentheses
        Expr expr = expression();
        if (!current_matches(R_PAREN)) {
            // TODO: report error
        }
        return Grouping(expr);
    }
    // theoretically unreachable
    return Expr();
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
    if ((command_info.tokens[current]) == token) {
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
    return command_info.tokens[current];
}


/**
 * \brief Provides the token before the one being parsed.
 * \return The token prior to the one currently being parsed.
 */
lexemes Parser::previous_token() {
    if (current == 0) {
        return EMPTY;
    }
    return command_info.tokens[current - 1];
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
    return current >= (command_info.token_count);
}


/**
 * \brief Parses the input tokens into an expression.
 * \return The internal representation of the expression.
 */
Expr Parser::parse_input() {
    // TODO: handle more complex inputs (statements, blocks, ...)
    xpd_puts("HERE IN PARSE()\n");
    xpd_echo_int(current, XPD_Flag_UnsignedDecimal);
    return expression();
}
