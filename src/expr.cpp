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


// /**
//  *
//  */
// Binary::Binary(Expr left, lexemes opcode, Expr right) {
//     this -> left = left;
//     this -> opcode = opcode;
//     this -> right = right;
// }
/**
 *
 */
node make_new_binary(node * left, lexemes opcode, node * right) {
    node current;
    current.type = BINARY_NODE;
    current.entry.binary_val.left = left;
    current.entry.binary_val.opcode = opcode;
    current.entry.binary_val.right = right;
    return current;
}


// /**
//  *
//  */
// Expr Binary::get_left() {
//     return left;
// }


// /**
//  *
//  */
// lexemes Binary::get_opcode() {
//     return opcode;
// }


// /**
//  *
//  */
// Expr Binary::get_right() {
//     return right;
// }


// /**
//  *
//  */
// literal_value Binary::accept(Visitor visitor) {
//     return visitor.visitBinaryExpr(*this);
// }


// /**
//  *
//  */
// Grouping::Grouping(Expr expression) {
//     this -> expression = expression;
// }
/**
 *
 */
node make_new_grouping(node * expression) {
    node current;
    current.type = GROUPING_NODE;
    current.entry.grouping_val.expression = expression;
    return current;
}


// /**
//  *
//  */
// literal_value Grouping::accept(Visitor visitor) {
//     return visitor.visitGroupingExpr(*this);
// }


// /**
//  *
//  */
// Literal::Literal(literal_value value) {
//     this -> value = value;
// }
/**
 *
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


// /**
//  *
//  */
// literal_value Literal::accept(Visitor visitor) {
//     return visitor.visitLiteralExpr(*this);
// }


// /**
//  *
//  */
// Unary::Unary(lexemes opcode, Expr right) {
//     this -> opcode = opcode;
//     this -> right = right;
// }
/**
 *
 */
node make_new_unary(lexemes opcode, node * right) {
    node current;
    current.type = UNARY_NODE;
    current.entry.unary_val.opcode = opcode;
    current.entry.unary_val.right = right;
    return current;
}


// /**
//  *
//  */
// lexemes Unary::get_opcode() {
//     return opcode;
// }


// /**
//  *
//  */
// Expr Unary::get_right() {
//     return right;
// }


// /**
//  *
//  */
// literal_value Unary::accept(Visitor visitor) {
//     return visitor.visitUnaryExpr(*this);
// }
