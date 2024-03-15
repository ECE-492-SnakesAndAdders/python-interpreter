/*********************************************************************************
* Description: The evaluator (the third and final stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "error.h"
#include "evaluator.h"
#include "utility.h"


/** Much of this code is based on Crafting Interpreters by Robert Nystrom.
    Specifically, this file takes inspiration mostly from these chapters:
      * Chapter 7 (https://craftinginterpreters.com/evaluating-expressions.html)
*/


/**
 * \brief Constructor for evaluator class.
 * \param [in] nothing Unused value required to be passed in for syntax.
 */
Evaluator::Evaluator(uint16_t nothing) {
    // TODO: make useful
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
 * \brief Compares two values and determines if they are equal.
 * \param [in] left The first value to compare.
 * \param [in] left The second value to compare.
 * \return True if the two values are equal; false otherwise.
 */
bool Evaluator::equals(literal_value left, literal_value right) {
    // numerical values must be numerically equal
    if (is_numerical(left.type) && is_numerical(right.type)) {
        return (numerify(left) == numerify(right));
    // string values must have each and every character match
    } else if ((left.type == STRING_VALUE) && (right.type == STRING_VALUE)) {
        return (strcmp(left.data.string, right.data.string) == 0);
    // if both are None, then they are equal
    } else if ((left.type == NONE_VALUE) && (right.type == NONE_VALUE)) {
        return true;
    // values cannot possibly match
    } else {
        return false;
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
    // evaluate each operand left-to-right before evaluating combination
    // this prevents short-circuiting and is slightly slower, but required by system constraint
    literal_value left = evaluate(*(expr.left));
    literal_value right = evaluate(*(expr.right));
    literal_value result;

    // perform corresponding operation
    switch (expr.opcode) {
        // logical and operation (and)
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

        // matrix multiplication operation (@)
        case AT:
            // TODO: support?
            report_failure("@ operator (matrix multiplication) not supported");
            break;

        // bitwise and operation (&)
        case B_AND:
            // same as logical counterparts for boolean inputs
            if (is_boolean(left.type) && is_boolean(right.type)) {
                if (boolify(left) && boolify(right)) {
                    result.type = TRUE_VALUE;
                } else {
                    result.type = FALSE_VALUE;
                }
            // directly translates to C operator for numerical values only
            } else if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) & numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // bitwise or operation (|)
        case B_OR:
            // same as logical counterparts for boolean inputs
            if (is_boolean(left.type) && is_boolean(right.type)) {
                if (boolify(left) || boolify(right)) {
                    result.type = TRUE_VALUE;
                } else {
                    result.type = FALSE_VALUE;
                }
            // directly translates to C operator for numerical values only
            } else if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) | numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // bitwise arithmetic shift right operation (>>)
        case B_SAR:
            // directly translates to C operator for numerical values only
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) >> numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // bitwise logical shift left operation (<<)
        case B_SLL:
            // directly translates to C operator for numerical values only
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) << numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // bitwise exclusive or operation (^)
        case B_XOR:
            // same as logical counterparts for boolean inputs
            if (is_boolean(left.type) && is_boolean(right.type)) {
                if (boolify(left) != boolify(right)) {
                    result.type = TRUE_VALUE;
                } else {
                    result.type = FALSE_VALUE;
                }
            // directly translates to C operator for numerical values only
            } else if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) ^ numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // floor division operation (//)
        case D_SLASH:
            // directly translates to C operator for numerical values only
            // identical to regular divison because only integers are allowed
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) / numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // exponentiation operation (**)
        case D_STAR:
            // no access to standard library or multiplication operator, so manual computation for numerical values only
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left);
                for (uint16_t i = 1; i < numerify(right); i++) {
                    uint16_t temp = result.data.number;
                    for (uint16_t j = 1; j < numerify(left); j++) {
                        temp += result.data.number;
                    }
                    result.data.number = temp;
                }
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            // TODO: handle negative case
            break;

        // equality operation (==)
        case EQUAL:
            // check that contained values match
            if (equals(left, right)) {
                result.type = TRUE_VALUE;
            } else {
                result.type = FALSE_VALUE;
            }
            break;
        
        // greater than operation (>)
        case GREATER:
            // numeric-adjacent types directly translate to C operator
            if (is_numerical(left.type) && is_numerical(right.type)) {
                if (numerify(left) > numerify(right)) {
                    result.type = TRUE_VALUE;
                } else {
                    result.type = FALSE_VALUE;
                }
            } else {
                report_error(TYPE, "not supported between instances of");
                error_occurred = true;
            }
            // TODO: support string comparison
            break;

        // greater than or equal to operation (>=)
        case G_EQUAL:
            // numeric-adjacent types directly translate to C operator
            if (is_numerical(left.type) && is_numerical(right.type)) {
                if (numerify(left) >= numerify(right)) {
                    result.type = TRUE_VALUE;
                } else {
                    result.type = FALSE_VALUE;
                }
            } else {
                report_error(TYPE, "not supported between instances of");
                error_occurred = true;
            }
            // TODO: support string comparison
            break;
            
        // identity operation (is)
        case IS:
            // check that types and contained values match
            if ((left.type == right.type) && equals(left, right)) {
                result.type = TRUE_VALUE;
            } else {
                result.type = FALSE_VALUE;
            }
            // TODO: make this different from "=="
            break;

        // less than operation (<)
        case LESS:
            // numeric-adjacent types directly translate to C operator
            if (is_numerical(left.type) && is_numerical(right.type)) {
                if (numerify(left) < numerify(right)) {
                    result.type = TRUE_VALUE;
                } else {
                    result.type = FALSE_VALUE;
                }
            } else {
                report_error(TYPE, "not supported between instances of");
                error_occurred = true;
            }
            // TODO: support string comparison
            break;
            
        // less than or equal to operation (<=)
        case L_EQUAL:
            // numeric-adjacent types directly translate to C operator
            if (is_numerical(left.type) && is_numerical(right.type)) {
                if (numerify(left) <= numerify(right)) {
                    result.type = TRUE_VALUE;
                } else {
                    result.type = FALSE_VALUE;
                }
            } else {
                report_error(TYPE, "not supported between instances of");
                error_occurred = true;
            }
            // TODO: support string comparison
            break;

        // subtraction operation (-)
        case MINUS:
            // directly translates to C operator for numerical values only
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) - numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // not equals operation (!=)
        case N_EQUAL:
            // simple negation of the equality logic
            if (!equals(left, right)) {
                result.type = TRUE_VALUE;
            } else {
                result.type = FALSE_VALUE;
            }
            break;

        // logical or operation (or)
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

        // modulus operation (%)
        case PERCENT:
            // directly translates to C operator for numerical values only
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) % numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            // TODO: support string concatenation
            break;

        // addition and string concatenation operation (+)
        case PLUS:
            // directly translates to C operator for numerical values, concatenates two strings
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) + numerify(right);
            } else if ((left.type == STRING_VALUE) && (right.type == STRING_VALUE)) {
                result.type = STRING_VALUE;
                // track how many characters consumed from the left string
                uint16_t left_count = 0;
                for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
                    // always start by simply transcribing left string
                    if (left.data.string[i]) {
                        result.data.string[i] = left.data.string[i];
                        left_count++;
                    // once left string is done, insert the right string from the start (hence the offset)
                    } else if (right.data.string[i - left_count]) {
                        result.data.string[i] = right.data.string[i - left_count];
                    // add null terminator once both strings are done
                    } else {
                        result.data.string[i] = '\0';
                        break;
                    }
                }
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // division operator (/)
        case SLASH:
            // directly translates to C operator for numerical values only
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left) / numerify(right);
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            break;

        // multiplication and string repitition operation (+)
        case STAR:
            // no access to standard library or multiplication operator, so manual computation for numerical values
            // repeats a string
            if (is_numerical(left.type) && is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = numerify(left);
                for (uint16_t i = 1; i < numerify(right); i++) {
                    result.data.number += numerify(left);
                }
            } else {
                report_error(TYPE, "unsupported operand type(s)");
                error_occurred = true;
            }
            // TODO: support string concatenation
            break;

        // theoretically unreachable
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

    // perform corresponding operation
    switch (expr.opcode) {
        // bitwise not operation (~)
        case B_NOT:
            // directly translates to C operator for numerical values only
            if (is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = ~numerify(right);
            } else {
                report_error(TYPE, "bad operand type");
                error_occurred = true;
            }
            break;

        // unary negation operation (-)
        case MINUS:
            // directly translates to C operator for numerical values only
            if (is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = -numerify(right);
            } else {
                report_error(TYPE, "bad operand type");
                error_occurred = true;
            }
            break;

        // logical negation operator (not)
        case NOT:
            // simply negates logical value of input
            if (boolify(right)) {
                result.type = FALSE_VALUE;
            } else {
                result.type = TRUE_VALUE;
            }
            break;

        // unary positive operation (+)
        case PLUS:
            // directly translates to C operator for numerical values only
            if (is_numerical(right.type)) {
                result.type = NUMBER_VALUE;
                result.data.number = +numerify(right);
            } else {
                report_error(TYPE, "bad operand type");
                error_occurred = true;
            }
            break;

        // theoretically unreachable
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
    if (input) {
        *output = evaluate(*input);
    } else {
        output -> type = NONE_VALUE;
    }
    // report any errors that occurred during execution
    if (has_error()) {
        return 1;
    }
    return 0;
}
