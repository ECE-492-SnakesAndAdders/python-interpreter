/*********************************************************************************
* Description: The interpreter consolidating the lexer, parser, and evaluator
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef INTERPRETER_H
#define INTERPRETER_H


/**
 *
 */
class Interpreter {
    private:
        //
        Evaluator evaluator;

    public:
        //
        Interpreter(uint16_t nothing): evaluator() {};
        //
        uint16_t interpret_command(char ** input_ptr, char ** output_ptr);
};


#endif
