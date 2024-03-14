/*********************************************************************************
* Description: The interpreter consolidating the lexer, parser, and evaluator
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef INTERPRETER_H
#define INTERPRETER_H


/**
 * \brief The overall interpreter; it executes commands from start to finish.
 */
class Interpreter {
    private:
        // The evaluator must persist between runs to save variables and environment
        Evaluator evaluator;

    public:
        // basic constructor (needs dummy parameter to be callable) that calls evaluator constructor
        Interpreter(environment * test_var): evaluator(test_var) {};
        // Interpreter(uint16_t nothing): evaluator() {};
        // executes a single command from start to finish
        uint16_t interpret_command(char ** input_ptr, char ** output_ptr);
};


#endif
