/*********************************************************************************
* Description: The evaluator (the third and final stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "evaluator.h"


/** Much of this code is based on Crafting Interpreters by Robert Nystrom.
    Specifically, this file takes inspiration mostly from these chapters:
      * Chapter 7 (https://craftinginterpreters.com/evaluating-expressions.html)
*/


// Placeholder
Evaluator::Evaluator(uint16_t nothing) {
    // TODO: remove or make useful
}


/**
 * \brief Evaluates a binary operation represented by a syntax tree node.
 * \param [in] expr The internal representation of the binary operation.
 * \return The computed value of the syntax tree node.
 */
literal_value Evaluator::evaluate_binary(binary_value expr) {
    // evaluate each operand before evaluating combination
    // TODO: support short-circuit oerators and right-associative ones (not just left-to-right)
    literal_value left = evaluate(*(expr.left));
    literal_value right = evaluate(*(expr.right));
    literal_value result;
    // perform corresponding operation
    switch (expr.opcode) {
        case B_AND:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number & right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case B_OR:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number | right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case B_SAR:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number >> right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case B_SLL:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number << right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case B_XOR:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number ^ right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case D_SLASH:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number / right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case D_STAR:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number;
                // TODO: Make this cross-compilable
                // for (uint16_t i = 1; i < right.data.number; i++) {
                //     result.data.number = result.data.number * left.data.number;
                // }
            } else {
                // TODO: report error
                // TODO: handle negative case
            }
            break;
        case MINUS:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number - right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case PLUS:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number + right.data.number;
            } else {
                // TODO: report error
                // TODO: support string concatenation
            }
            break;
        case SLASH:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number / right.data.number;
            } else {
                // TODO: report error
                // TODO: support string concatenation
            }
            break;
        case STAR:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                // TODO: make this cross-compilable
                // result.data.number = left.data.number * right.data.number;
                result.data.number = left.data.number;
                // for (uint16_t i = 1; i < right.data.number; i++) {
                //     result.data.number += left.data.number;
                // }
            } else {
                // TODO: report error
                // TODO: support string concatenation
            }
            break;
        default:
            // TODO: report error
            break;
    }
    return result;
}


/**
 * \brief Evaluates a nested expression within parentheses on a syntax tree node.
 * \param [in] expr The internal represententation of the nested expression.
 * \return The computed value of the syntax tree node.
 */
literal_value Evaluator::evaluate_grouping(grouping_value expr) {
    // just pass along result of nested expression
    return evaluate(*(expr.expression));
}


/**
 * \brief Evaluates a literal expression represented by a syntax tree node.
 * \param [in] expr The internal representation of teh literal value.
 * \return The computed value of the syntax tree node.
 */
literal_value Evaluator::evaluate_literal(literal_value expr) {
    // no action required, just pass this value along
    return expr;
}


/**
 * \brief Evaluates a unary operation represented by a syntax tree node.
 * \param [in] expr The internal representation of the unary operation.
 * \return The computed value of the syntax tree node.
 */
literal_value Evaluator::evaluate_unary(unary_value expr) {
    // evaluate the operand before evaluating result
    literal_value right = evaluate(*(expr.right));
    // perform corresponding operatio
    switch (expr.opcode) {
        case B_NOT:
            if (right.type == NUMBER_VALUE) {
                right.data.number = ~right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case MINUS:
            if (right.type == NUMBER_VALUE) {
                right.data.number = -right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case NOT:
            if (right.type == FALSE_VALUE) {
                right.type = TRUE_VALUE;
            } else if (right.type == TRUE_VALUE) {
                right.type = FALSE_VALUE;
            } else {
                // TODO: report error
            }
            break;
        case PLUS:
            break;
    }
    return right;
}


/**
 * \brief General function to evaluate a portion of a syntax tree.
 * \param [in] tree_node The syntax tree node to evaluate.
 * \return The computed value of the syntax tree node.
 */
literal_value Evaluator::evaluate(node tree_node) {
    literal_value result;
    // call appropriate function based on the operation needed (polymorphism not possible)
    switch (tree_node.type) {
        case BINARY_NODE:
            result = evaluate_binary(tree_node.entry.binary_val);
            break;
        case GROUPING_NODE:
            result = evaluate_grouping(tree_node.entry.grouping_val);
            break;
        case LITERAL_NODE:
            result = evaluate_literal(tree_node.entry.literal_val);
            break;
        case UNARY_NODE:
            result = evaluate_unary(tree_node.entry.unary_val);
            break;
    }
    return result;
}
