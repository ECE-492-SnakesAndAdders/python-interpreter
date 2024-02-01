/*********************************************************************************
* Description: The lexer (the first stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "lexer.h"


/**
 * \brief Basic constructor for the lexer.
 * \param [in] input Pointer to the input string entered by the user.
 */
Lexer::Lexer(char ** input) {
    // transfer input string into object
    for (uint16_t i = 0; i < MAX_INPUT_LEN; i++) {
        line[i] = *(*input + i);
    }
    // ensure no remnants of previous command in memory
    for (uint16_t i = 0; i < MAX_INPUT_TOKENS; i++) {
        tokens[i] = EMPTY;
    }
    // get the number of non-null characters in the command
    while (*(*input + length) != '\0') {
        length++;
    }
}


/**
 * \brief scans the input string and returns a list of its tokens.
 * \return A pointer to an array of tokens.
 */
lexemes * Lexer::scan_input() {
    // keep on reading next character until command is over
    while (!(end_reached())) {
        start = current;
        scan_next_token();
    }
    // ------------------------------------------------------------------------
    // FOR DEBUGGING; print each token to see that lexer works
    for (uint16_t i = 0; i < MAX_INPUT_TOKENS; i++) {
        xpd_puts(names[tokens[i]]);
        xpd_putc(' ');
    }
    xpd_putc('\n');
    // -----------------------------------------------------------------------
    return tokens;
}


/**
 * \brief Reads the next character in the input and creates a corresponding token.
 */
void Lexer::scan_next_token() {
    // look at the current character in the string
    char current_char = line[current];
    char * lit;
    // consider each possible case entered
    switch (current_char) {
        // unconditionally single-character tokens
        case '(':
            add_token(L_PAREN);
            break;
        case ')':
            add_token(R_PAREN);
            break;
        case '{':
            add_token(L_BRACE);
            break;
        case '}':
            add_token(R_BRACE);
            break;
        case '[':
            add_token(L_BRACKET);
            break;
        case ']':
            add_token(R_BRACKET);
            break;
        case ',':
            add_token(COMMA);
            break;
        case '.':
            add_token(DOT);
            break;
        case ':':
            add_token(COLON);
            break;
        case ';':
            add_token(SEMICOLON);
            break;
        case '~':
            add_token(B_NOT);
        // possible single- or double-character tokens
        case '+':
            add_token(next_matches('=') ? A_ASSIGN : PLUS);
            break;
        case '-':
            add_token(next_matches('=') ? S_ASSIGN : MINUS);
            break;
        case '%':
            add_token(next_matches('=') ? R_ASSIGN : PERCENT);
            break;
        case '&':
            add_token(next_matches('=') ? BA_ASSIGN : B_AND);
            break;
        case '|':
            add_token(next_matches('=') ? BO_ASSIGN : B_OR);
            break;
        case '^':
            add_token(next_matches('=') ? BX_ASSIGN : B_XOR);
            break;
        case '=':
            add_token(next_matches('=') ? EQUAL : ASSIGN);
            break;
        case '!':
            add_token(next_matches('=') ? N_EQUAL : NOT);
            break;
        // possible one-, two-, or three-character tokens
        case '*':
            if (next_matches('=')) {
                add_token(M_ASSIGN);
            } else if (next_matches('*')) {
                if (next_matches('=')) {
                    add_token(E_ASSIGN);
                } else {
                    add_token(D_STAR);
                }
            } else {
                add_token(STAR);
            }
            break;
        case '/':
            if (next_matches('=')) {
                add_token(D_ASSIGN);
            } else if (next_matches('/')) {
                if (next_matches('=')) {
                    add_token(F_ASSIGN);
                } else {
                    add_token(D_SLASH);
                }
            } else {
                add_token(SLASH);
            }
            break;
        case '>':
            if (next_matches('=')) {
                add_token(G_EQUAL);
            } else if (next_matches('>')) {
                if (next_matches('=')) {
                    add_token(BR_ASSIGN);
                } else {
                    add_token(B_SAR);
                }
            } else {
                add_token(GREATER);
            }
            break;
        case '<':
            if (next_matches('=')) {
                add_token(L_EQUAL);
            } else if (next_matches('<')) {
                if (next_matches('=')) {
                    add_token(BL_ASSIGN);
                } else {
                    add_token(B_SLL);
                }
            } else {
                add_token(LESS);
            }
            break;
        // TODO: literals
        case '\"':
            match_string('\"', &lit);
            xpd_puts(lit);
            add_token(STRING);
            break;
        case '\'':
            match_string('\'', &lit);
            xpd_puts(lit);
            add_token(STRING);
            break;
        // TODO: whitespace
        // comment means rest of line is discarded
        case '#':
            return;
        // default:
            // xpd_puts("Error: unexpected character.\n");
            // return;
    }
    current++;
}


void Lexer::add_token(lexemes token) {
    tokens[token_count] = token;
    token_count++;
}


bool Lexer::next_matches(char character) {
    current++;
    if (end_reached()) {
        current--;
        return false;
    }
    if (line[current] == character) {
        return true;
    } else {
        current--;
        return false;
    }
    // return (line[current] == character);
}


void Lexer::match_string(char terminator, char ** output_ptr) {
    // char value[MAX_STR_LEN] = "";
    // char * value_ptr = (char *) value;
    uint16_t i = 0;
    current++;
    xpd_puts("STRING READ: ");
    while (line[current] != terminator) {
        xpd_putc(line[current]);
        *(*output_ptr + i) = line[current];
        // value[i] = line[current];
        i++;
        current++;
    }
    current++;
    xpd_putc('\n');
    // return value_ptr;
}


bool Lexer::end_reached() {
    return current >= length - 1;
}
