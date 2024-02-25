/*********************************************************************************
* Description:
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "expr.h"
#include "lexer.h"


/**
 *
 */
template <typename T> T Visitor::visitBinaryExpr(Binary expr) {
    return NULL;
}


/**
 *
 */
template <typename T> T Visitor::visitUnaryExpr(Unary expr) {
    return NULL;
}


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
template <typename T> T Binary::accept(Visitor visitor) {
    return visitor.visitBinaryExpr<T>(this);
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
template <typename T> T Grouping::accept(Visitor visitor) {
    return visitor.visitGroupingExpr<T>(this);
}


/**
 *
 */
Literal::Literal(void * value) {
    this -> value = value;
}


/**
 *
 */
template <typename T> T Literal::accept(Visitor visitor) {
    return visitor.visitLiteralExpr<T>(this);
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
template <typename T> T Unary::accept(Visitor visitor) {
    return visitor.visitUnaryExpr<T>(this);
}
