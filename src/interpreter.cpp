/*********************************************************************************
* Description: The interpreter consolidating the lexer, parser, and evaluator
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <cstdio>
#include <cstring>
#include "evaluator.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "utility.h"


/**
 * \brief Executes a single command from start to finish.
 * \param [in] input_ptr Pointer to where the input received is stored.
 * \param [in] output_ptr Pointer to where to store the text to be printed.
 * \return 0 on success; a non-zero error number on failure.
 */
int Interpreter::interpret_command(char ** input_ptr, char ** output_ptr) {
    int return_code = 0;

    // lex command, convert raw string into a sequence of tokens
    lexed_command token_sequence;
    Lexer lexer(input_ptr, &token_sequence);
    if ((return_code = lexer.scan_input())) {
        return 1;
    }
    // ------------------------------------------------------------------------
    // // FOR DEBUGGING; print each token to see that lexer works
    // printf("LEXED INFO:\n");
    // printf("Tokens: ");
    // for (int i = 0; i < token_sequence.token_count; i++) {
    //     printf("%d",token_sequence.tokens[i]);
    //     printf(" ");
    //     printf(token_names[token_sequence.tokens[i]]);
    //     printf(", ");
    // }
    // printf("\n");
    // printf("Strings: ");
    // for (int i = 0; i < token_sequence.str_lit_count; i++) {
    //     printf(token_sequence.str_lits[i]);
    //     printf(", ");
    // }
    // printf("\n");
    // printf("Numbers: ");
    // for (int i = 0; i < token_sequence.num_lit_count; i++) {
    //     printf("%d",token_sequence.num_lits[i]);
    //     printf(", ");
    // }
    // printf("\n");
    // printf("Identifiers: ");
    // for (int i = 0; i < token_sequence.identifier_count; i++) {
    //     printf(token_sequence.identifiers[i]);
    //     printf(", ");
    // }
    // printf("\n");
    // ------------------------------------------------------------------------

    // parse command, convert sequence of tokens into a syntax tree
    node * tree[MAX_NUM_STMTS];
    memset(tree, 0, sizeof(tree));
    Parser parser(token_sequence, tree);
    if ((return_code = parser.parse_input())) {
        return 1;
    }
    // ------------------------------------------------------------------------
    // // FOR DEBUGGING; print tree to see that parser works
    // printf("PARSED INFO:\n");
    // int j = 0;
    // while (tree[j] != NULL) {
    //     print_tree(*tree[j]);
    //     printf("\n");
    //     j++;
    // }
    // ------------------------------------------------------------------------

    int i = 0;
    while (tree[i] != NULL) {
        // evaluate command, convert syntax tree into a result
        literal_value result;
        if ((return_code = evaluator.evaluate_input(tree[i], &result))) {
            return 1;
        }
        // save the result of the execution
        stringify_value(result, output_ptr);
        i++;
    }
    return 0;
}
