/*********************************************************************************
* Description: The evaluator (the third and final stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "expr.h"


class Evaluator {
    private:
    public:
        Evaluator(uint16_t nothing);
        literal_value evaluate(node tree_node);
        literal_value evaluate_binary(binary_value expr);
        literal_value evaluate_grouping(grouping_value expr);
        literal_value evaluate_literal(literal_value expr);
        literal_value evaluate_unary(unary_value expr);
};

#endif
