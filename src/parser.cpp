/*********************************************************************************
* Description: The parser (the second stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


/**
 *
 */
Parser::Parser(lexed_command * input) {
    command_info = *input;
}


/**
 *
 */
Expr Parser::parse_input() {
    return;
}
