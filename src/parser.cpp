/*********************************************************************************
* Description: The parser (the second stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include "lexer.h"
#include <XPD.h>
#include "error.h"
#include "expr.h"
#include "lexer.h"
#include "parser.h"


/** Much of this code is based on Crafting Interpreters by Robert Nystrom.
    Specifically, this file takes inspiration mostly from these chapters:
      * Chapter 5 (https://craftinginterpreters.com/representing-code.html)
      * Chapter 6 (https://craftinginterpreters.com/parsing-expressions.html)
      * Chapter 17 (https://craftinginterpreters.com/compiling-expressions.html)
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
Parser::Parser(lexed_command input, node ** output) {
    command_info = input;
    syntax_tree = output;
}


/**
 * \brief Under construction.
 */
node * Parser::write_new_node(node * value) {
    // determine the type of node needed to be created
    tree_nodes[current_node].type = value -> type;
    // transfer the data correctly into this safe memory location
    switch (tree_nodes[current_node].type) {
        case BINARY_NODE:
            tree_nodes[current_node].entry.binary_val.left = value -> entry.binary_val.left;
            tree_nodes[current_node].entry.binary_val.opcode = value -> entry.binary_val.opcode;
            tree_nodes[current_node].entry.binary_val.right = value -> entry.binary_val.right;
            break;
        case GROUPING_NODE:
            tree_nodes[current_node].entry.grouping_val.expression = value -> entry.grouping_val.expression;
            break;
        case LITERAL_NODE:
            tree_nodes[current_node].entry.literal_val.type = value -> entry.literal_val.type;
            if (tree_nodes[current_node].entry.literal_val.type == NUMBER_VALUE) {
                tree_nodes[current_node].entry.literal_val.data.number = value -> entry.literal_val.data.number;
            } else if (tree_nodes[current_node].entry.literal_val.type == STRING_VALUE) {
                for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
                    tree_nodes[current_node].entry.literal_val.data.string[i] = value -> entry.literal_val.data.string[i];
                }
            }
            break;
        case UNARY_NODE:
            tree_nodes[current_node].entry.unary_val.opcode = value -> entry.unary_val.opcode;
            tree_nodes[current_node].entry.unary_val.right = value -> entry.unary_val.right;
            break;
    }
    // return the address of this current node while moving along counter to new node location
    return &tree_nodes[current_node++];
}


/**
 * \brief Starts the chain of parsing a Python expression.
 * \return The internal representation of the expression.
 */
node * Parser::expression() {
    return disjunction();
}


/**
 * \brief Handles the lowest priority operator: "or"/"OR".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::disjunction() {
    /** general strategy for all levels is as follows:
     *    recurse down, allowing higher priority operators to start, then
     *    deal with current operator as it arises, then
     *    recurse down for subsequent operand, then
     *    combine these two operands with the current operator
     */
    // recurse to higher priority operators
    node * expr_ptr = conjunction();
    // deal with any OR operators
    while (current_matches(OR)) {
        // find the operation to be completed
        lexemes opcode = previous_token();
        // recurse again to get the right operand
        node * right_ptr = write_new_node(conjunction());
        // combine the operands, building up the syntax tree
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    // combined nested expressions is the new expression
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "and"/"AND".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::conjunction() {
    node * expr_ptr = inversion();
    while (current_matches(AND)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(inversion());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "not"/"NOT".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::inversion() {
    while (current_matches(NOT)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(inversion());
        node expr = make_new_unary(opcode, right_ptr);
        return write_new_node(&expr);
    }
    return comparison();
}


/**
 * \brief Handles the next lowest priority operator: "=="/"EQUAL", "!="/"N_EQUAL", ">"/"GREATER",
 *                                                   ">="/"G_EQUAL", "<"/"LESS", "<="/"L_EQUAL",
 *                                                   "is"/"IS", "is not"/"", "in"/"IN", "not in"/"") .
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::comparison() {
    node * expr_ptr = bor();
    // TODO: handle cascaded comaprison operators properly
    // TODO: handle "is not" and "not in" keywords
    while (current_matches(EQUAL) || current_matches(N_EQUAL) || current_matches(GREATER) ||
           current_matches(G_EQUAL) || current_matches(LESS) || current_matches(L_EQUAL) ||
           current_matches(IS) || current_matches(IN)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(bor());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "|"/"B_OR".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::bor() {
    node * expr_ptr = bxor();
    while (current_matches(B_OR)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(bxor());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "^"/"B_XOR".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::bxor() {
    node * expr_ptr = band();
    while (current_matches(B_XOR)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(band());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "&"/"B_AND".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::band() {
    node * expr_ptr = shift();
    while (current_matches(B_AND)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(shift());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "<<"/"B_SLL", ">>"/"B_SAR".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::shift() {
    node * expr_ptr = sum();
    while (current_matches(B_SLL) || current_matches(B_SAR)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(sum());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "+"/"PLUS", "-"/"MINUS".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::sum() {
    node * expr_ptr = term();
    while (current_matches(PLUS) || current_matches(MINUS)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(term());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "*"/"STAR", "/"/"SLASH", "//"/"D_SLASH", "%"/"PERCENT", "@"/"AT".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::term() {
    node * expr_ptr = factor();
    while (current_matches(STAR) || current_matches(SLASH) || current_matches(D_SLASH) ||
           current_matches(PERCENT) || current_matches(AT)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(factor());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "+"/"PLUS", "-"/"MINUS", "~"/"B_NOT".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::factor() {
    while (current_matches(PLUS) || current_matches(MINUS) || current_matches(B_NOT)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(factor());
        node expr = make_new_unary(opcode, right_ptr);
        return write_new_node(&expr);
    }
    return power();
}


/**
 * \brief Handles the next lowest priority operator: "**"/"D_STAR".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::power() {
    node * expr_ptr = primary();
    while (current_matches(D_STAR)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(primary());
        node expr = make_new_binary(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles literal values: "STRING", "NUMBER", "TRUE", "FALSE", "NONE".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::primary() {
    // TODO: set up data types so that any value can be used
    // base case deals with literal values and parentheses
    // sentinel literal values
    node * expr_ptr = NULL;
    literal_value lit;
    if (current_matches(FALSE)) {
        lit.type = FALSE_VALUE;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    } else if (current_matches(NONE)) {
        lit.type = NONE_VALUE;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    } else if (current_matches(TRUE)) {
        lit.type = TRUE_VALUE;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    }
    // number and string literal values that need to be fetched
    if (current_matches(NUMBER)) {
        lit.type = NUMBER_VALUE;
        lit.data.number = command_info.num_lits[current_num_lit];
        current_num_lit++;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    } else if (current_matches(STRING)) {
        lit.type = STRING_VALUE;
        char * temp = command_info.str_lits[current_str_lit];
        for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
            lit.data.string[i] = *(temp + i);
        }
        current_str_lit++;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    }
    // deal with parentheses
    if (current_matches(L_PAREN)) {
        // any general expression can be nested in parentheses
        node expr = make_new_grouping(expression());
        expr_ptr = write_new_node(&expr);
        if (!current_matches(R_PAREN)) {
            // TODO: enable parentheses over multiple line
            // error detected, must have closing parenthesis
            report_error(SYNTAX, "invalid syntax");
            error_occurred = true;
        }
    }
    // deal with parentheses
    if (current_matches(IDENTIFIER)) {
        xpd_puts("NOT IMPLEMENETED YET\n");
        error_occurred = true;
    }
    // TODO: reconcile this with future additions
    // error detected, must have some operand
    if (expr_ptr == NULL) {
        report_error(SYNTAX, "invalid syntax");
        error_occurred = true;
    }
    return expr_ptr;
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
 * \brief Tells if an error has occurred while parsing a token sequence.
 * \return True if an error has occurred; false otherwise.
 */
bool Parser::has_error() {
    return error_occurred;
}


/**
 * \brief Parses the input tokens into an expression.
 * \return 0 if execution succeeded; non-zero value if an error occurred.
 */
uint16_t Parser::parse_input() {
    // TODO: handle more complex inputs (statements, blocks, ...)
    *syntax_tree = expression();
    if (has_error()) {
        return 1;
    }
    return 0;
}
