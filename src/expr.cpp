/*********************************************************************************
* Description:
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "expr.h"
#include "lexer.h"
#include "parser.h"


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
