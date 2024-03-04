/*********************************************************************************
* Description: The evaluator (the third and final stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "error.h"
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
 * \brief Determines if a literal value is legal to use in a numerical operation.
 * \param [in] value The literal value's type to check.
 * \return True if the literal can be used in a numerical expression; false otherwise.
 */
bool Evaluator::is_numerical(literal_types type) {
    return ((type == FALSE_VALUE) || (type == NUMBER_VALUE) || (type == TRUE_VALUE));
}


/**
 * \brief Converts a legal literal value into a number for use in a numerical operation.
 * \param [in] value The literal value to convert.
 * \return The integer representation of the literal.
 */
uint16_t Evaluator::numerify(literal_value value) {
    switch (value.type) {
        // valid types map directly to integers
        case FALSE_VALUE:
            return 0;
        case NUMBER_VALUE:
            return value.data.number;
        case TRUE_VALUE:
            return 1;
        // strings and None are not acceptable for arithmetic operators
        // theoretically unreachable
        default:
            report_failure("numerical value expected for operation");
            error_occurred = true;
            return -1;
    }
}


/**
 * \brief Determines if a literal value is legal to use directly in a logical operation.
 * \param [in] value The literal value's type to check.
 * \return True if the literal can be used directly in a logical expression; false otherwise.
 */
bool Evaluator::is_boolean(literal_types type) {
    return ((type == FALSE_VALUE) || (type == TRUE_VALUE));
}


/**
 * \brief Converts a legal literal value into a boolean for use in a numerical operation.
 * \param [in] value The literal value to convert.
 * \return The boolean representation of the literal.
 */
bool Evaluator::boolify(literal_value value) {
    switch (value.type) {
        case FALSE_VALUE:
            return false;
        case NONE_VALUE:
            return false;
        // number 0 is False and all others are true
        case NUMBER_VALUE:
            if (value.data.number == 0) {
                return false;
            } else {
                return true;
            }
        // empty string is false and all others are true
        case STRING_VALUE:
            return (bool) value.data.string[0];
        case TRUE_VALUE:
            return true;
        // theoretically unreachable
        default:
            report_failure("logical value expected for operation");
            error_occurred = true;
            return -1;
    }
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


/**
 * \brief Evaluates a binary operation represented by a syntax tree node.
 * \param [in] expr The internal representation of the binary operation.
 * \return The computed value of the syntax tree node.
 */
literal_value Evaluator::evaluate_binary(binary_value expr) {
    // evaluate each operand before evaluating combination
    // TODO: support real short-circuit operators and right-associative ones (not just left-to-right)
    literal_value left = evaluate(*(expr.left));
    literal_value right = evaluate(*(expr.right));
    literal_value result;
    // perform corresponding operation
    switch (expr.opcode) {
        case AND:
            // case where left operand is "False" -- always must be False output (short circuit)
            if (is_boolean(left.type) && !boolify(left)) {
                result.type = FALSE_VALUE;
            // case where left operand is non-boolean but "False" -- return the left operand
            } else if (!is_boolean(left.type) && !boolify(left)) {
                result = left;
            // case where left operand is "True" -- just return the right operand
            } else if (boolify(left)) {
                result = right;
            } else {
                // TODO: determine if this is reachable in CPython
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case B_AND:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) & numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case B_OR:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) | numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case B_SAR:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) >> numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case B_SLL:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) << numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case B_XOR:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) ^ numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case D_SLASH:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) / numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case D_STAR:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left);
                // TODO: Make this cross-compilable
                // for (uint16_t i = 1; i < numerify(right); i++) {
                //     result.data.number = result.data.number * numerify(left);
                // }
            } else {
                // TODO: handle negative case
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case IS:
            // if types are the same, the values must match to satusfy "is"
            if (left.type == right.type) {
                switch (left.type) {
                    case FALSE_VALUE:
                        result.type = TRUE_VALUE;
                        break;
                    case NONE_VALUE:
                        result.type = TRUE_VALUE;
                        break;
                    case NUMBER_VALUE:
                        if (left.data.number == right.data.number) {
                            result.type = TRUE_VALUE;
                        } else {
                            result.type = FALSE_VALUE;
                        }
                        break;
                    case STRING_VALUE:
                        // TODO: compare strings by value
                        result.type = FALSE_VALUE;
                        break;
                    case TRUE_VALUE:
                        result.type = TRUE_VALUE;
                        break;
                    default:
                        report_failure("unexpected error");
                        error_occurred = true;
                }
            // if types don't match, we are guaranteed that the values do not match
            } else {
                result.type = FALSE_VALUE;
            }
            break;

        case MINUS:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) - numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case OR:
            // case where left operand is "True" -- always must be True output (short circuit)
            if (is_boolean(left.type) && boolify(left)) {
                result.type = TRUE_VALUE;
            // case where left operand is non-boolean but "True" -- return the left operand
            } else if (!is_boolean(left.type) && boolify(left)) {
                result = left;
            // case where left operand is "False" -- just return the right operand
            } else if (!boolify(left)) {
                result = right;
            } else {
                // TODO: determine if this is reachable in CPython
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case PERCENT:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) % numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            // TODO: support string concatenation
            break;

        case PLUS:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) + numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            // TODO: support string concatenation
            break;

        case SLASH:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) / numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        case STAR:
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                // TODO: make this cross-compilable
                // result.data.number = numerify(left) * numerify(right);
                result.data.number = numerify(left);
                // for (uint16_t i = 1; i < numerify(right); i++) {
                //     result.data.number += numerify(left);
                // }
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            // TODO: support string concatenation
            break;

        default:
            report_failure("no such binary operator exists");
            error_occurred = true;
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
    literal_value result;
    // perform corresponding operatio
    switch (expr.opcode) {
        case B_NOT:
            if (is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = ~numerify(right);
            } else {
                report_error(TYPE, "bad operand type");
                error_occurred = true;
            }
            break;

        case MINUS:
            if (is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = -numerify(right);
            } else {
                report_error(TYPE, "bad operand type");
                error_occurred = true;
            }
            break;

        case NOT:
            if (boolify(right)) {
                result.type = FALSE_VALUE;
            } else {
                result.type = TRUE_VALUE;
            }
            break;

        case PLUS:
            if (is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = +numerify(right);
            } else {
                report_error(TYPE, "bad operand type");
                error_occurred = true;
            }
            break;
            
        default:
            report_failure("no such unary operator exists");
            error_occurred = true;
            break;
    }
    return result;
}


/**
 * \brief Tells if an error has occurred while parsing a token sequence.
 * \return True if an error has occurred; false otherwise.
 */
bool Evaluator::has_error() {
    return error_occurred;
}


/**
 * \brief Evaluates the input syntax tree and executes it.
 * \param [in] input The syntax tree to evaluate.
 * \param [in] output Pointer to where to store the output value.
 * \return 0 if execution succeeded; non-zero value if an error occurred.
 */
uint16_t Evaluator::evaluate_input(node * input, literal_value * output) {
    *output = evaluate(*input);
    // report any errors that occurred during execution
    if (has_error()) {
        return 1;
    }
    return 0;
}
