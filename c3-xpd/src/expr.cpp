/*********************************************************************************
* Description:
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
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
 * \brief Constructor for a binary operation's syntax tree node.
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
        case BINARY_NODE:
            xpd_puts(" ( ");
            print_tree(*(tree.entry.binary_val.left));
            xpd_puts(token_names[tree.entry.binary_val.opcode]);
            print_tree(*(tree.entry.binary_val.right));
            xpd_puts(" ) ");
            break;

        case GROUPING_NODE:
            xpd_puts(" ( ");
            print_tree(*(tree.entry.grouping_val.expression));
            xpd_puts(" ) ");
            break;

        case LITERAL_NODE:
            if (tree.entry.literal_val.type == FALSE_VALUE) {
                xpd_puts(" False ");
            } else if (tree.entry.literal_val.type == NONE_VALUE) {
                xpd_puts(" None ");
            } else if (tree.entry.literal_val.type == NUMBER_VALUE) {
                xpd_putc(' ');
                xpd_echo_int(tree.entry.literal_val.data.number, XPD_Flag_SignedDecimal);
                xpd_putc(' ');
            } else if (tree.entry.literal_val.type == STRING_VALUE) {
                xpd_putc(' ');
                xpd_puts(tree.entry.literal_val.data.string);
                xpd_putc(' ');
            } else if (tree.entry.literal_val.type == TRUE_VALUE) {
                xpd_puts(" True ");
            }
            break;
            
        case UNARY_NODE:
            xpd_puts(" ( ");
            xpd_puts(token_names[tree.entry.unary_val.opcode]);
            print_tree(*(tree.entry.unary_val.right));
            xpd_puts(" ) ");
            break;
    }
}
