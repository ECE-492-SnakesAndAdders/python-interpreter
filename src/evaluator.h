/*********************************************************************************
* Description: The evaluator (the third and final stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef EVALUATOR_H
#define EVALUATOR_H


#include "expr.h"


/**
 * \brief The evaluator of the interpreter; it takes a syntax tree as input and executes it.
 */
class Evaluator {
    private:
        // for evaluating each type of syntax tree node
        literal_value evaluate_binary(binary_value expr);
        literal_value evaluate_grouping(grouping_value expr);
        literal_value evaluate_literal(literal_value expr);
        literal_value evaluate_unary(unary_value expr);

    public:
        // basic constructor for the class
        Evaluator(uint16_t nothing);
        // converts a tree (or subtree, or node) into a single value
        literal_value evaluate(node tree_node);
};


#endif
