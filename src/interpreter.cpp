/*********************************************************************************
* Description: The interpreter consolidating the lexer, parser, and evaluator
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "evaluator.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "utility.h"


/**
 *
 */
uint16_t Interpreter::interpret_command(char ** input_ptr, char ** output_ptr) {
    uint16_t return_code = 0;

    // lex command, convert raw string into a sequence of tokens
    lexed_command token_sequence;
    Lexer lexer(input_ptr, &token_sequence);
    if ((return_code = lexer.scan_input())) {
        return 1;
    }
    // ------------------------------------------------------------------------
    // // FOR DEBUGGING; print each token to see that lexer works
    // print_string("LEXED INFO:\n");
    // print_string("Tokens: ");
    // for (uint16_t i = 0; i < token_sequence.token_count; i++) {
    //     xpd_echo_int(token_sequence.tokens[i], XPD_Flag_UnsignedDecimal);
    //     print_string(" ");
    //     print_string(token_names[token_sequence.tokens[i]]);
    //     print_string(", ");
    // }
    // xpd_putc('\n');
    // print_string("Strings: ");
    // for (uint16_t i = 0; i < token_sequence.str_lit_count; i++) {
    //     print_string(token_sequence.str_lits[i]);
    //     print_string(", ");
    // }
    // xpd_putc('\n');
    // print_string("Numbers: ");
    // for (uint16_t i = 0; i < token_sequence.num_lit_count; i++) {
    //     xpd_echo_int(token_sequence.num_lits[i], XPD_Flag_UnsignedDecimal);
    //     print_string(", ");
    // }
    // xpd_putc('\n');
    // print_string("Identifiers: ");
    // for (uint16_t i = 0; i < token_sequence.identifier_count; i++) {
    //     print_string(token_sequence.identifiers[i]);
    //     print_string(", ");
    // }
    // xpd_putc('\n');
    // ------------------------------------------------------------------------

    // parse command, convert sequence of tokens into a syntax tree
    node * tree;
    Parser parser(token_sequence, &tree);
    if ((return_code = parser.parse_input())) {
        return 1;
    }
    // ------------------------------------------------------------------------
    // // FOR DEBUGGING; print tree to see that parser works
    // print_string("PARSED INFO:\n");
    // print_tree(*tree);
    // xpd_putc('\n');
    // ------------------------------------------------------------------------

    // evaluate command, convert syntax tree into a result
    literal_value result;
    if ((return_code = evaluator.evaluate_input(tree, &result))) {
        return 1;
    }

    // save the result of the execution
    stringify_value(result, output_ptr);
    return 0;
}
