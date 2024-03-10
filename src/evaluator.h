/*********************************************************************************
* Description: The evaluator (the third and final stage of the interpreter)
* Author(s): Isaac Joffe and David Onchuru
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef EVALUATOR_H
#define EVALUATOR_H


#include "environment.h"
#include "expr.h"


/**
 * \brief The evaluator of the interpreter; it takes a syntax tree as input and executes it.
 */
class Evaluator {
    private:
        // for storing variables
        environment * env;
        // for determining if data types are compatible
        bool is_numerical(literal_types type);
        uint16_t numerify(literal_value value);
        bool is_boolean(literal_types type);
        bool boolify(literal_value value);
        bool equals(literal_value left, literal_value right);
        // for evaluating each type of syntax tree node
        literal_value evaluate(node tree_node);
        literal_value evaluate_assign(assign_value expr);
        literal_value evaluate_binary(binary_value expr);
        literal_value evaluate_grouping(grouping_value expr);
        literal_value evaluate_literal(literal_value expr);
        literal_value evaluate_unary(unary_value expr);
        literal_value evaluate_variable(variable_value expr);
        // for error handling
        bool error_occurred = false;
        bool has_error();

    public:
        // converts a tree (or subtree, or node) into a single value
        uint16_t evaluate_input(node * input, literal_value * output);
};


#endif
