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
 *
 */
Binary::Binary(Expr left, lexemes opcode, Expr right) {
    this -> left = left;
    this -> opcode = opcode;
    this -> right = right;
}


/**
 *
 */
Expr Binary::get_left() {
    return left;
}


/**
 *
 */
lexemes Binary::get_opcode() {
    return opcode;
}


/**
 *
 */
Expr Binary::get_right() {
    return right;
}


/**
 *
 */
literal_value Binary::accept(Visitor visitor) {
    return visitor.visitBinaryExpr(*this);
}


/**
 *
 */
Grouping::Grouping(Expr expression) {
    this -> expression = expression;
}


/**
 *
 */
literal_value Grouping::accept(Visitor visitor) {
    return visitor.visitGroupingExpr(*this);
}


/**
 *
 */
Literal::Literal(literal_value value) {
    this -> value = value;
}


/**
 *
 */
literal_value Literal::accept(Visitor visitor) {
    return visitor.visitLiteralExpr(*this);
}


/**
 *
 */
Unary::Unary(lexemes opcode, Expr right) {
    this -> opcode = opcode;
    this -> right = right;
}


/**
 *
 */
lexemes Unary::get_opcode() {
    return opcode;
}


/**
 *
 */
Expr Unary::get_right() {
    return right;
}


/**
 *
 */
literal_value Unary::accept(Visitor visitor) {
    return visitor.visitUnaryExpr(*this);
}
