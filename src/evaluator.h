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


// the maximum number of characters in a line of output
#ifndef MAX_OUTPUT_LEN
#define MAX_OUTPUT_LEN 256
#endif


/**
 * \brief The evaluator of the interpreter; it takes a syntax tree as input and executes it.
 */
class Evaluator {
    private:
        // for storing variables
        environment * env;
        // for determining if data types are compatible
        bool is_numerical(literal_types type);
        int numerify(literal_value value);
        bool is_boolean(literal_types type);
        bool boolify(literal_value value);
        bool is_iterable(literal_types type);
        bool equals(literal_value left, literal_value right);
        // for evaluating each type of syntax tree node
        literal_value evaluate(node tree_node);
        literal_value evaluate_assign(assign_value expr);
        literal_value evaluate_binary(binary_value expr);
        literal_value evaluate_block(block_value expr);
        literal_value evaluate_forloop(forloop_value expr);
        literal_value evaluate_grouping(grouping_value expr);
        literal_value evaluate_ifelse(ifelse_value expr);
        literal_value evaluate_literal(literal_value expr);
        literal_value evaluate_logical(logical_value expr);
        literal_value evaluate_special(special_value expr);
        literal_value evaluate_unary(unary_value expr);
        literal_value evaluate_variable(variable_value expr);
        literal_value evaluate_whileloop(whileloop_value expr);
        // for error handling
        bool error_occurred = false;
        bool has_error();
        // for storing outputs
        char * output_str;

    public:
        // constructor to svae pre-created environment
        Evaluator(environment * env);
        // converts a tree (or subtree, or node) into a single value
        int evaluate_input(node * input, char ** output);
};


#endif
