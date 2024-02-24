/*********************************************************************************
* Description:
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef EXPR_H
#define EXPR_H


#include "lexer.h"


class Binary;
class Unary;


/**
 *
 */
 class Visitor {
    public:
        // T visitAssignExpr(Assign expr);
        template <typename T> T visitBinaryExpr(Binary expr);
        // T visitCallExpr(Call expr);
        // T visitGetExpr(Get expr);
        // T visitGroupingExpr(Grouping expr);
        // T visitLiteralExpr(Literal expr);
        // T visitLogicalExpr(Logical expr);
        // T visitSetExpr(Set expr);
        // T visitSuperExpr(Super expr);
        // T visitThisExpr(This expr);
        template <typename T> T visitUnaryExpr(Unary expr);
        // T visitVariableExpr(Variable expr);
};


/**
 *
 */
class Expr {
    public:
        template <typename T> T accept(Visitor visitor);
};


/**
 *
 */
class Binary : public Expr {
    private:
        Expr left;
        lexemes opcode;
        Expr right;

    public:
        Binary(Expr left, lexemes opcode, Expr right);
        template <typename T> T accept(Visitor visitor);
};


/**
 *
 */
class Unary : public Expr {
    private:
        lexemes opcode;
        Expr right;

    public:
        Unary(lexemes opcode, Expr right);
        template <typename T> T accept(Visitor visitor);
};

#endif
