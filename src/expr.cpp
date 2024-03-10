/*********************************************************************************
* Description: Defines the various syntax tree nodes and associated functionality
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "expr.h"
#include "lexer.h"
#include "parser.h"
#include "utility.h"


/** Much of this code is based on Crafting Interpreters by Robert Nystrom.
    Specifically, this file takes inspiration mostly from these chapters:
      * Chapter 5 (https://craftinginterpreters.com/representing-code.html)
      * Chapter 18 (https://craftinginterpreters.com/types-of-values.html)
      * Chapter 19 (https://craftinginterpreters.com/strings.html)
    Some inspiration was also taken from another person's partial implementation:
      * Loxy in C (https://github.com/gcatlin/loxy)
*/


/**
 * \brief Constructor for an assignment operation's syntax tree node.
 * \param [in] name The name of identifier to write into.
 * \param [in] right Pointer to the node representing the expression to assign.
 * \return A structure representing the syntax tree node.
 */
node make_new_assign(char name[], node * value) {
    node current;
    current.type = ASSIGN_NODE;
    for (uint16_t i = 0; i < MAX_IDENTIFIER_LEN; i++) {
        current.entry.assign_val.name[i] = name[i];
    }
    // ---------------------------------------------------------
    print_string(current.entry.assign_val.name);
    // ---------------------------------------------------------
    current.entry.assign_val.value = value;
    return current;
}


/**
 * \brief Constructor for a binary arithmetic operation's syntax tree node.
 * \param [in] left Pointer to the node representing the first operand.
 * \param [in] opcode The token representing the operation to complete.
 * \param [in] right Pointer to the node representing the second operand.
 * \return A structure representing the syntax tree node.
 */
node make_new_binary(node * left, lexemes opcode, node * right) {
    node current;
    current.type = BINARY_NODE;
    current.entry.binary_val.left = left;
    current.entry.binary_val.opcode = opcode;
    current.entry.binary_val.right = right;
    return current;
}


/**
 * \brief Constructor for a grouping's syntax tree node.
 * \param [in] expression Pointer to the node representing the nested expression.
 * \return A structure representing the syntax tree node.
 */
node make_new_grouping(node * expression) {
    node current;
    current.type = GROUPING_NODE;
    current.entry.grouping_val.expression = expression;
    return current;
}


/**
 * \brief Constructor for a literal value's syntax tree node.
 * \param [in] value A structure representing the literal's value.
 * \return A structure representing the syntax tree node.
 */
node make_new_literal(literal_value value) {
    node current;
    current.type = LITERAL_NODE;
    current.entry.literal_val.type = value.type;
    // need to copy over actualdata contained
    if (current.entry.literal_val.type == NUMBER_VALUE) {
        current.entry.literal_val.data.number = value.data.number;
    } else if (current.entry.literal_val.type == STRING_VALUE) {
        for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
            current.entry.literal_val.data.string[i] = value.data.string[i];
        }
    }
    return current;
}


/**
 * \brief Constructor for a binary logical operation's syntax tree node.
 * \param [in] left Pointer to the node representing the first operand.
 * \param [in] opcode The token representing the operation to complete.
 * \param [in] right Pointer to the node representing the second operand.
 * \return A structure representing the syntax tree node.
 */
node make_new_logical(node * left, lexemes opcode, node * right) {
    node current;
    current.type = LOGICAL_NODE;
    current.entry.logical_val.left = left;
    current.entry.logical_val.opcode = opcode;
    current.entry.logical_val.right = right;
    return current;
}


/**
 * \brief Constructor for a unary operation's syntax tree node.
 * \param [in] opcode The token representing the operation to complete.
 * \param [in] right Pointer to the node representing the operand.
 * \return A structure representing the syntax tree node.
 */
node make_new_unary(lexemes opcode, node * right) {
    node current;
    current.type = UNARY_NODE;
    current.entry.unary_val.opcode = opcode;
    current.entry.unary_val.right = right;
    return current;
}


/**
 * \brief Constructor for an variable-fetching operation's syntax tree node.
 * \param [in] name The name of identifier to read from.
 * \return A structure representing the syntax tree node.
 */
node make_new_variable(char name[]) {
    node current;
    current.type = VARIABLE_NODE;
    for (uint16_t i = 0; i < MAX_IDENTIFIER_LEN; i++) {
        current.entry.variable_val.name[i] = name[i];
    }
    // ---------------------------------------------------------
    print_string(current.entry.variable_val.name);
    // ---------------------------------------------------------
    return current;
}


/**
 * \brief Converts a literal value to a properly-formatted string.
 * \param [in] value The literal value to be converted into a string.
 * \param [in] output_ptr Pointer to the output string buffer.
 */
void stringify_value(literal_value value, char ** output_ptr) {
    switch (value.type) {
        case FALSE_VALUE:
            *(*output_ptr + 0) = 'F';
            *(*output_ptr + 1) = 'a';
            *(*output_ptr + 2) = 'l';
            *(*output_ptr + 3) = 's';
            *(*output_ptr + 4) = 'e';
            break;

        case NONE_VALUE:
            break;

        case NUMBER_VALUE:
            itos(output_ptr, value.data.number);
            break;

        case STRING_VALUE:
            **output_ptr = '\'';
            for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
                if (value.data.string[i]) {
                    *(*output_ptr + i + 1) = value.data.string[i];
                } else {
                    *(*output_ptr + i + 1) = '\'';
                    *(*output_ptr + i + 2) = '\0';
                    break;
                }
            }
            break;

        case TRUE_VALUE:
            *(*output_ptr + 0) = 'T';
            *(*output_ptr + 1) = 'r';
            *(*output_ptr + 2) = 'u';
            *(*output_ptr + 3) = 'e';
            break;

        default:
            // TODO: verify that this is unreachable
            break;
    }
}


/**
 * \brief Prints the generated syntax tree for debugging.
 * \param [in] tree The tree (or subtree, or node) to print.
 */
void print_tree(node tree) {
    switch (tree.type) {
        case ASSIGN_NODE:
            print_string(" ( ");
            print_string(tree.entry.assign_val.name);
            print_string(" GETS ");
            print_tree(*(tree.entry.assign_val.value));
            print_string(" ) ");
            break;

        case BINARY_NODE:
            print_string(" ( ");
            print_tree(*(tree.entry.binary_val.left));
            print_string(token_names[tree.entry.binary_val.opcode]);
            print_tree(*(tree.entry.binary_val.right));
            print_string(" ) ");
            break;

        case GROUPING_NODE:
            print_string(" ( ");
            print_tree(*(tree.entry.grouping_val.expression));
            print_string(" ) ");
            break;

        case LITERAL_NODE:
            if (tree.entry.literal_val.type == FALSE_VALUE) {
                print_string(" False ");
            } else if (tree.entry.literal_val.type == NONE_VALUE) {
                print_string(" None ");
            } else if (tree.entry.literal_val.type == NUMBER_VALUE) {
                print_string(" ");
                xpd_echo_int(tree.entry.literal_val.data.number, XPD_Flag_SignedDecimal);
                print_string(" ");
            } else if (tree.entry.literal_val.type == STRING_VALUE) {
                print_string(" ");
                print_string(tree.entry.literal_val.data.string);
                print_string(" ");
            } else if (tree.entry.literal_val.type == TRUE_VALUE) {
                print_string(" True ");
            }
            break;

        case LOGICAL_NODE:
            print_string(" ( ");
            print_tree(*(tree.entry.logical_val.left));
            print_string(token_names[tree.entry.logical_val.opcode]);
            print_tree(*(tree.entry.logical_val.right));
            print_string(" ) ");
            break;
            
        case UNARY_NODE:
            print_string(" ( ");
            print_string(token_names[tree.entry.unary_val.opcode]);
            print_tree(*(tree.entry.unary_val.right));
            print_string(" ) ");
            break;

        case VARIABLE_NODE:
            print_string(" ( ");
            print_string(tree.entry.variable_val.name);
            print_string(" ) ");
            break;
    }
}
