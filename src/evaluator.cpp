/*********************************************************************************
* Description: The evaluator (the third and final stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "evaluator.h"


/**
 * \brief Placeholder.
 */
Evaluator::Evaluator(uint16_t nothing) {
    // TODO: remove or make useful
}


/**
 * \brief General function to evaluate a portion of a syntax tree.
 * \param [in] tree_node The syntax tree node to evaluate.
 * \return The computed value of the syntax tree node.
 */
literal_value Evaluator::evaluate(node tree_node) {
    literal_value result;
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


/**
 * \brief Evaluates a binary operation represented by a syntax tree node.
 * \param [in] expr The internal representation of the binary operation.
 * \return The computed value of the syntax tree node.
 */
literal_value Evaluator::evaluate_binary(binary_value expr) {
    literal_value left = evaluate(*(expr.left));
    literal_value right = evaluate(*(expr.right));
    literal_value result;
    switch (expr.opcode) {
        case PLUS:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number + right.data.number;
            } else {
                // TODO: report error
            }
            break;
        case MINUS:
            if (left.type == NUMBER_VALUE && right.type == NUMBER_VALUE) {
                result.data.number = left.data.number - right.data.number;
            } else {
                // TODO: report error
            }
            break;
    }
    return result;
}


/**
 *
 */
literal_value Evaluator::evaluate_grouping(grouping_value expr) {
    return evaluate(*(expr.expression));
}


/**
 *
 */
literal_value Evaluator::evaluate_literal(literal_value expr) {
    return expr;
}


/**
 *
 */
literal_value Evaluator::evaluate_unary(unary_value expr) {
    literal_value right = evaluate(*(expr.right));
    switch (expr.opcode) {
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
        case MINUS:
            if (right.type == NUMBER_VALUE) {
                right.data.number = -right.data.number;
            }
            break;
        case B_NOT:
            if (right.type == NUMBER_VALUE) {
                right.data.number = ~right.data.number;
            }
            break;
    }
    return right;
}
