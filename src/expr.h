/*********************************************************************************
* Description:
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef EXPR_H
#define EXPR_H


#include "lexer.h"


class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;

/**
 * \brief The list of all possible literal types in Python.
 */
enum literal_types {
    FALSE_VALUE, NONE_VALUE, NUMBER_VALUE, STRING_VALUE, TRUE_VALUE
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
 *
 */
 class Visitor {
    public:
        // T visitAssignExpr(Assign expr);
        template <typename T> T visitBinaryExpr(Binary expr);
        // T visitCallExpr(Call expr);
        // T visitGetExpr(Get expr);
        template <typename T> T visitGroupingExpr(Grouping expr);
        template <typename T> T visitLiteralExpr(Literal expr);
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
class Grouping : public Expr {
    private:
        Expr expression;

    public:
        Grouping(Expr expression);
        template <typename T> T accept(Visitor visitor);
};


/**
 *
 */
class Literal : public Expr {
    private:
        literal_value value;

    public:
        Literal(literal_value value);
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
