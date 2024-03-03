/*********************************************************************************
* Description:
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef EXPR_H
#define EXPR_H


#include <XPD.h>
#include "lexer.h"


#ifndef MAX_LIT_LEN
#define MAX_LIT_LEN 32
#endif


struct node;    // forward declaration


/**
 * \brief The list of all possible literal types in Python.
 */
enum literal_types {
    FALSE_VALUE, NONE_VALUE, NUMBER_VALUE, STRING_VALUE, TRUE_VALUE
};


/**
 * \brief The internal representation of a binary operation.
 */
struct binary_value {
    // the first operand of this operation
    node * left;
    // the actual operation to complete
    lexemes opcode;
    // the second operand of this operation
    node * right;
};


/**
 * \brief The internal representation of a parenthetical expression.
 */
struct grouping_value {
    // the expression inside the parentheses
    node * expression;
};


/**
 * \brief The internal representation of a literal value.
 */
struct literal_value {
    // the data type of the value being stored
    literal_types type;
    // the actual data being stored
    union {
        uint16_t number;
        char string[MAX_LIT_LEN];
    } data;
};


/**
 * \brief The internal representation of a unary operation.
 */
struct unary_value {
    // the actual operation to complete
    lexemes opcode;
    // the operand of this operation
    node * right;
};


/**
 * \brief The list of all possible nodes in the syntax tree.
 */
enum node_types {
    BINARY_NODE, GROUPING_NODE, LITERAL_NODE, UNARY_NODE
};


/**
 * \brief The internal representation of a node in the syntax tree.
 */
struct node {
    // the type of node -- unary, binary, literal, etc.
    node_types type;
    // the internal representation of that type of node, conserving memory
    union {
        binary_value binary_val;
        grouping_value grouping_val;
        literal_value literal_val;
        unary_value unary_val;
    } entry;
};


// constructor functions for these node structs
node make_new_binary(node * left, lexemes opcode, node * right);
node make_new_grouping(node * expression);
node make_new_literal(literal_value value);
node make_new_unary(lexemes opcode, node * right);
// to convert a literal value into a well-formatted string
void stringify_value(literal_value value, char ** output_ptr);
// to print a representation of the syntax tree for debugging
void print_tree(node tree);


#endif
