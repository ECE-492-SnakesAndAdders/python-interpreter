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
Binary::Binary(node * left, lexemes opcode, node * right) {
    expr.type = BINARY_NODE;
    expr.entry.binary_val.left = left;
    expr.entry.binary_val.opcode = opcode;
    expr.entry.binary_val.right = right;
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


/**
 *
 */
literal_value Binary::accept(Visitor visitor) {
    return visitor.visitBinaryExpr(*this);
}


// /**
//  *
//  */
// Grouping::Grouping(Expr expression) {
//     this -> expression = expression;
// }
/**
 *
 */
Grouping::Grouping(node * expression) {
    expr.type = GROUPING_NODE;
    expr.entry.grouping_val.expression = expression;
}


/**
 *
 */
literal_value Grouping::accept(Visitor visitor) {
    return visitor.visitGroupingExpr(*this);
}


// /**
//  *
//  */
// Literal::Literal(literal_value value) {
//     this -> value = value;
// }
/**
 *
 */
Literal::Literal(node * value) {
    expr.type = LITERAL_NODE;
    expr.entry.literal_val.type = value -> entry.literal_val.type;
    if (expr.entry.literal_val.type == NUMBER_VALUE) {
        expr.entry.literal_val.data.number = value -> entry.literal_val.data.number;
    } else if (expr.entry.literal_val.type == STRING_VALUE) {
        for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
            expr.entry.literal_val.data.string[i] = value -> entry.literal_val.data.string[i];
        }
    }
}


/**
 *
 */
literal_value Literal::accept(Visitor visitor) {
    return visitor.visitLiteralExpr(*this);
}


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
Unary::Unary(lexemes opcode, node * right) {
    expr.type = UNARY_NODE;
    expr.entry.unary_val.opcode = opcode;
    expr.entry.unary_val.right = right;
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


/**
 *
 */
literal_value Unary::accept(Visitor visitor) {
    return visitor.visitUnaryExpr(*this);
}
