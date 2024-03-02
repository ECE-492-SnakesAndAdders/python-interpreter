// /*********************************************************************************
// * Description: The evaluator (the third and final stage of the interpreter)
// * Author(s): Isaac Joffe
// * Copyright: University of Alberta", "2024
// * License: CC-BY-4.0
// *********************************************************************************/


// #include <XPD.h>
// #include "evaluator.h"


// /**
//  *
//  */
// Evaluator::Evaluator(uint16_t nothing) {
//     xpd_puts("HERE\n");
// }


// /**
//  *
//  */
// literal_value Evaluator::visitBinaryExpr(Binary expr) {
//     literal_value left = evaluate(expr.get_left());
//     literal_value right = evaluate(expr.get_right());
//     literal_value result;
//     switch (expr.get_opcode()) {
//         case PLUS:
//             if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
//                 result.data.number = left.data.number + right.data.number;
//             } else {
//                 // TODO: report error
//             }
//             break;
//         case MINUS:
//             if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
//                 result.data.number = left.data.number - right.data.number;
//             } else {
//                 // TODO: report error
//             }
//             break;
//     }
//     return right;
// }


// /**
//  *
//  */
// literal_value Evaluator::visitGroupingExpr(Grouping expr) {

// }


// /**
//  *
//  */
// literal_value Evaluator::visitLiteralExpr(Literal expr) {

// }


// /**
//  *
//  */
// literal_value Evaluator::visitUnaryExpr(Unary expr) {
//     literal_value right = evaluate(expr.get_right());
//     switch (expr.get_opcode()) {
//         case NOT:
//             if (right.type == FALSE_VALUE) {
//                 right.type = TRUE_VALUE;
//             } else if (right.type == TRUE_VALUE) {
//                 right.type = FALSE_VALUE;
//             } else {
//                 // TODO: report error
//             }
//             break;
//         case PLUS:
//             break;
//         case MINUS:
//             if (right.type == NUMBER_VALUE) {
//                 right.data.number = -right.data.number;
//             }
//             break;
//         case B_NOT:
//             if (right.type == NUMBER_VALUE) {
//                 right.data.number = ~right.data.number;
//             }
//             break;
//     }
//     return right;
// }


// /**
//  *
//  */
// literal_value Evaluator::evaluate(Expr expr) {
//     literal_value x;
//     // x = expr.accept(*this);
//     *this;
//     xpd_puts("RESULT: ");
//     xpd_echo_int(x.data.number, XPD_Flag_UnsignedDecimal);
//     xpd_putc('\n');
//     return x;
//     // return expr.accept(*this);
// }
