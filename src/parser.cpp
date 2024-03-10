/*********************************************************************************
* Description: The parser (the second stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "error.h"
#include "expr.h"
#include "lexer.h"
#include "parser.h"
#include "utility.h"


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
 * \param [in] output Pointer to pointer to where to store the produced syntax tree.
 */
Parser::Parser(lexed_command input, node ** output) {
    command_info = input;
    syntax_tree = output;
}


/**
 * \brief Allocates and transcribes a syntax tree node for storage.
 * \param [in] value Pointer to the node to save.
 * \return Pointer to where the node has been safely stored.
 */
node * Parser::write_new_node(node * value) {
    // determine the type of node needed to be created
    tree_nodes[current_node].type = value -> type;

    // transfer the data correctly into this safe memory location
    switch (tree_nodes[current_node].type) {
        case ASSIGN_NODE:
            for (uint16_t i = 0; i < MAX_IDENTIFIER_LEN; i++) {
                tree_nodes[current_node].entry.assign_val.name[i] = value -> entry.assign_val.name[i];
            }
            tree_nodes[current_node].entry.assign_val.value = value -> entry.assign_val.value;
            break;

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
            // additional processing to transfer actual stored data
            if (tree_nodes[current_node].entry.literal_val.type == NUMBER_VALUE) {
                tree_nodes[current_node].entry.literal_val.data.number = value -> entry.literal_val.data.number;
            } else if (tree_nodes[current_node].entry.literal_val.type == STRING_VALUE) {
                for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
                    tree_nodes[current_node].entry.literal_val.data.string[i] = value -> entry.literal_val.data.string[i];
                }
            }
            break;

        case LOGICAL_NODE:
            tree_nodes[current_node].entry.logical_val.left = value -> entry.logical_val.left;
            tree_nodes[current_node].entry.logical_val.opcode = value -> entry.logical_val.opcode;
            tree_nodes[current_node].entry.logical_val.right = value -> entry.logical_val.right;
            break;

        case UNARY_NODE:
            tree_nodes[current_node].entry.unary_val.opcode = value -> entry.unary_val.opcode;
            tree_nodes[current_node].entry.unary_val.right = value -> entry.unary_val.right;
            break;
        
        case VARIABLE_NODE:
            for (uint16_t i = 0; i < MAX_IDENTIFIER_LEN; i++) {
                tree_nodes[current_node].entry.variable_val.name[i] = value -> entry.variable_val.name[i];
            }
            break;
    }

    // return the address of this current node while moving along counter to new node location
    return &tree_nodes[current_node++];
}


/**
 * \brief Starts the chain of parsing a Python statement.
 * \return The internal representation of the statement.
 */
node * Parser::statement() {
    // start recursively looking for statement operators
    return assign_statement();
}


/**
 * \brief Handles assignment statements (both normal and augmented).
 * \return The internal representation of the statement parsed so far.
 */
node * Parser::assign_statement() {
    // TODO: support more complex variables to be written to (necessary?)
    // assignment must begin with variable on the left-hand side
    if (current_matches(IDENTIFIER)) {
        // normal assignment is handled differently, no variable reading needed
        if (current_matches(ASSIGN)) {
            // assignments can be chained, but the value being assigned is an expression
            node * value_ptr = write_new_node(assign_statement());
            // make a tree node for this assignment with the correct variable name
            node expr = make_new_assign(command_info.identifiers[current_identifier], value_ptr);
            // one more identifier name has been read
            current_identifier++;
            // save this node to build up the syntax tree
            node * expr_ptr = write_new_node(&expr);
            return expr_ptr;

        // augmented assignment involves an additional binary operation on the data, but no chaining
        } else {
            // make a tree node for reading the original variable
            node var = make_new_variable(command_info.identifiers[current_identifier]);
            node * var_ptr = write_new_node(&var);
            // value that variable is augmented with must be an expression
            node * value_ptr = write_new_node(expr_statement());
            // make a tree node for the augmentation operation
            node aug;

            // addition-augmented assignment (+=)
            if (current_matches(A_ASSIGN)) {
                aug = make_new_binary(var_ptr, PLUS, value_ptr);
            // subtraction-augmented assignment (-=)
            } else if (current_matches(S_ASSIGN)) {
                aug = make_new_binary(var_ptr, MINUS, value_ptr);
            // multiplication-augmented assignment (*=)
            } else if (current_matches(M_ASSIGN)) {
                aug = make_new_binary(var_ptr, STAR, value_ptr);
            // matrix-multiplication-augmented assignment (@=)
            } else if (current_matches(I_ASSIGN)) {
                aug = make_new_binary(var_ptr, AT, value_ptr);
            // division-augmented assignment (/=)
            } else if (current_matches(D_ASSIGN)) {
                aug = make_new_binary(var_ptr, SLASH, value_ptr);
            // subtraction-augmented assignment (-=)
            } else if (current_matches(R_ASSIGN)) {
                aug = make_new_binary(var_ptr, PLUS, value_ptr);
            // exponentiation-augmented assignment (**=)
            } else if (current_matches(E_ASSIGN)) {
                aug = make_new_binary(var_ptr, D_STAR, value_ptr);
            // floor-division-augmented assignment (//=)
            } else if (current_matches(F_ASSIGN)) {
                aug = make_new_binary(var_ptr, D_SLASH, value_ptr);
            // bitwise-and-augmented assignment (&=)
            } else if (current_matches(BA_ASSIGN)) {
                aug = make_new_binary(var_ptr, B_AND, value_ptr);
            // bitwise-or-augmented assignment (|=)
            } else if (current_matches(BO_ASSIGN)) {
                aug = make_new_binary(var_ptr, B_OR, value_ptr);
            // bitwise-xor-augmented assignment (^=)
            } else if (current_matches(BX_ASSIGN)) {
                aug = make_new_binary(var_ptr, B_XOR, value_ptr);
            // bitwise-shift-left-augmented assignment (<<=)
            } else if (current_matches(BL_ASSIGN)) {
                aug = make_new_binary(var_ptr, B_SLL, value_ptr);
            // bitwise-shift-right-augmented assignment (>>=)
            } else if (current_matches(BR_ASSIGN)) {
                aug = make_new_binary(var_ptr, B_SAR, value_ptr);
            // walrus-augmented assignment (:=)
            } else if (current_matches(W_ASSIGN)) {
                aug = make_new_binary(var_ptr, COLON, value_ptr);
            }

            node * aug_ptr = write_new_node(&aug);
            // make a tree node for this assignment with the correct variable name
            node expr = make_new_assign(command_info.identifiers[current_identifier], aug_ptr);
            // one more identifier name has been read
            current_identifier++;
            // save this node to build up the syntax tree
            node * expr_ptr = write_new_node(&expr);
            return expr_ptr;
        }
    }
    return expr_statement();
}


/**
 * \brief Handles expression statements (just an expression).
 * \return The internal representation of the statement parsed so far.
 */
node * Parser::expr_statement() {
    return expression();
}


/**
 * \brief Starts the chain of parsing a Python expression.
 * \return The internal representation of the expression.
 */
node * Parser::expression() {
    // start recursively looking for expression operators
    return disjunction();
}


/**
 * \brief Handles the lowest priority operator: "or"/"OR".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::disjunction() {
    /** general strategy for all levels of expression operators is as follows:
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
        node expr = make_new_logical(expr_ptr, opcode, right_ptr);
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
        node expr = make_new_logical(expr_ptr, opcode, right_ptr);
        expr_ptr = write_new_node(&expr);
    }
    return expr_ptr;
}


/**
 * \brief Handles the next lowest priority operator: "not"/"NOT".
 * \return The internal representation of the expression parsed so far.
 */
node * Parser::inversion() {
    // unary operator handled differently, no left operand to handle first
    while (current_matches(NOT)) {
        lexemes opcode = previous_token();
        // recursively call itself to allow stacked unary operators
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
    // TODO: handle cascaded comaprison operators properly
    // TODO: handle "is not" and "not in" keywords
    node * expr_ptr = bor();
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
    if (current_matches(D_STAR)) {
        lexemes opcode = previous_token();
        node * right_ptr = write_new_node(power());
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
    // base case deals with literal values and parentheses
    node * expr_ptr = NULL;

    // sentinel literal values
    if (current_matches(FALSE)) {
        literal_value lit;
        lit.type = FALSE_VALUE;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    } else if (current_matches(NONE)) {
        literal_value lit;
        lit.type = NONE_VALUE;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    } else if (current_matches(TRUE)) {
        literal_value lit;
        lit.type = TRUE_VALUE;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    }

    // number and string literal values that need to be fetched
    if (current_matches(NUMBER)) {
        literal_value lit;
        lit.type = NUMBER_VALUE;
        lit.data.number = command_info.num_lits[current_num_lit];
        current_num_lit++;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    } else if (current_matches(STRING)) {
        literal_value lit;
        lit.type = STRING_VALUE;
        char * temp = command_info.str_lits[current_str_lit];
        for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
            lit.data.string[i] = *(temp + i);
        }
        current_str_lit++;
        node expr = make_new_literal(lit);
        expr_ptr = write_new_node(&expr);
    }

    // deal with parentheses (nested expressions)
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

    // deal with identifiers (meaning variables)
    if (current_matches(IDENTIFIER)) {
        node expr = make_new_variable(command_info.identifiers[current_identifier]);
        current_identifier++;
        expr_ptr = write_new_node(&expr);
    }

    // TODO: reconcile this with future additions
    // // error detected, must have some operand
    // if (expr_ptr == NULL) {
    //     report_error(SYNTAX, "invalid syntax");
    //     error_occurred = true;
    // }
    
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
    *syntax_tree = statement();
    if (has_error()) {
        return 1;
    }
    return 0;
}
