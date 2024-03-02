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


// class Expr;
// class Binary;
// class Grouping;
// class Literal;
// class Unary;
struct node;

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


struct unary_value {
    lexemes opcode;
    node * right;
};


struct binary_value {
    node * left;
    lexemes opcode;
    node * right;
};


struct grouping_value {
    node * expression;
};


/**
 * \brief
 */
enum node_types {
    LITERAL_NODE, UNARY_NODE, BINARY_NODE, GROUPING_NODE
};


/**
 *
 */
struct node {
    node_types type;
    union {
        literal_value literal_val;
        unary_value unary_val;
        binary_value binary_val;
        grouping_value grouping_val;
    } entry;
};


node make_new_binary(node * left, lexemes opcode, node * right);
node make_new_unary(lexemes opcode, node * right);
node make_new_literal(literal_value value);
node make_new_grouping(node * expression);

// /**
//  *
//  */
//  class Visitor {
//     public:
//         // T visitAssignExpr(Assign expr);
//         literal_value visitBinaryExpr(Binary expr);
//         // T visitCallExpr(Call expr);
//         // T visitGetExpr(Get expr);
//         literal_value visitGroupingExpr(Grouping expr);
//         literal_value visitLiteralExpr(Literal expr);
//         // T visitLogicalExpr(Logical expr);
//         // T visitSetExpr(Set expr);
//         // T visitSuperExpr(Super expr);
//         // T visitThisExpr(This expr);
//         literal_value visitUnaryExpr(Unary expr);
//         // T visitVariableExpr(Variable expr);
// };


// /**
//  *
//  */
// class Expr {
//     public:
//         node expr;
//         literal_value accept(Visitor visitor);
// };


// // /**
// //  *
// //  */
// // class Binary : public Expr {
// //     private:
// //         Expr left;
// //         lexemes opcode;
// //         Expr right;

// //     public:
// //         Binary(Expr left, lexemes opcode, Expr right);
// //         Expr get_left();
// //         lexemes get_opcode();
// //         Expr get_right();
// //         literal_value accept(Visitor visitor);
// // };
// /**
//  *
//  */
// class Binary : public Expr {
//     // private:
//     //     Expr left;
//     //     lexemes opcode;
//     //     Expr right;

//     public:
//         Binary(node * left, lexemes opcode, node * right);
//         // Expr get_left();
//         // lexemes get_opcode();
//         // Expr get_right();
//         literal_value accept(Visitor visitor);
// };


// /**
//  *
//  */
// class Grouping : public Expr {
//     // private:
//     //     Expr expression;

//     public:
//         Grouping(node * expression);
//         literal_value accept(Visitor visitor);
// };


// /**
//  *
//  */
// class Literal : public Expr {
//     // private:
//     //     literal_value value;

//     public:
//         Literal(node * value);
//         literal_value accept(Visitor visitor);
// };


// // /**
// //  *
// //  */
// // class Unary : public Expr {
// //     private:
// //         lexemes opcode;
// //         Expr right;

// //     public:
// //         Unary(lexemes opcode, Expr right);
// //         lexemes get_opcode();
// //         Expr get_right();
// //         literal_value accept(Visitor visitor);
// // };
// /**
//  *
//  */
// class Unary : public Expr {
//     // private:
//     //     lexemes opcode;
//     //     Expr right;

//     public:
//         Unary(lexemes opcode, node * right);
//         // lexemes get_opcode();
//         // Expr get_right();
//         literal_value accept(Visitor visitor);
// };

#endif
