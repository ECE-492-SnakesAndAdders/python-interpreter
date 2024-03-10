/*********************************************************************************
* Description: Defines the various syntax tree nodes and associated functionality
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef EXPR_H
#define EXPR_H


#include <XPD.h>
#include "lexer.h"


// the maximum number of characters in a literal
#ifndef MAX_LIT_LEN
#define MAX_LIT_LEN 32
#endif

// the maximum number of characters in any identifier
#ifndef MAX_IDENTIFIER_LEN
#define MAX_IDENTIFIER_LEN 32
#endif


struct node;    // forward declaration


/**
 * \brief The internal representation of an assignment operation.
 */
struct assign_value {
    // the identifier to assign into
    char name[MAX_IDENTIFIER_LEN];
    // the expression to assign the identifier to
    node * value;
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
 * \brief The list of all possible literal types in Python.
 */
enum literal_types {
    FALSE_VALUE, NONE_VALUE, NUMBER_VALUE, STRING_VALUE, TRUE_VALUE
};


// for ease of printing
const char * const literal_names[] = {
    "bool", "NoneType", "int", "str", "bool"
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
 * \brief The internal representation of the operation of fetching a variable's value.
 */
struct variable_value {
    // the identifier to read from
    char name[MAX_IDENTIFIER_LEN];
};


/**
 * \brief The list of all possible nodes in the syntax tree.
 */
enum node_types {
    ASSIGN_NODE, BINARY_NODE, GROUPING_NODE, LITERAL_NODE, UNARY_NODE, VARIABLE_NODE
};


// for ease of printing
const char * const node_names[] = {
    "assign", "binary", "grouping", "literal", "unary", "variable"
};


/**
 * \brief The internal representation of a node in the syntax tree.
 */
struct node {
    // the type of node -- unary, binary, literal, etc.
    node_types type;
    // the internal representation of that type of node, conserving memory
    union {
        assign_value assign_val;
        binary_value binary_val;
        grouping_value grouping_val;
        literal_value literal_val;
        unary_value unary_val;
        variable_value variable_val;
    } entry;
};


// constructor functions for these node structs
node make_new_assign(char name[], node * value);
node make_new_binary(node * left, lexemes opcode, node * right);
node make_new_grouping(node * expression);
node make_new_literal(literal_value value);
node make_new_unary(lexemes opcode, node * right);
node make_new_variable(char name[]);
// to convert a literal value into a well-formatted string
void stringify_value(literal_value value, char ** output_ptr);
// to print a representation of the syntax tree for debugging
void print_tree(node tree);


#endif
