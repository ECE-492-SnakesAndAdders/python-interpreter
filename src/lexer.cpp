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
        // start = current;
        scan_next_token();
    }

    // ------------------------------------------------------------------------
    // FOR DEBUGGING; print each token to see that lexer works
    for (uint16_t i = 0; i < MAX_INPUT_TOKENS; i++) {
        xpd_puts(names[tokens[i]]);
        xpd_putc(' ');
    }
    xpd_putc('\n');
    // ------------------------------------------------------------------------

    return tokens;
}


/**
 * \brief Reads the next character in the input and creates a corresponding token.
 */
void Lexer::scan_next_token() {
    // to store string and number literals
    char str_lit[MAX_STR_LEN] = "";
    char * str_lit_ptr = (char *) str_lit;
    uint16_t num_lit = 0;

    // look at the current character in the string
    char current_char = line[current];
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
            add_token(DOT);    // note: no decimals in numbers for now
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
        // string literals
        case '\"':
            match_string('\"', &str_lit_ptr);
            // now 'str_lit' contains the string; that is, 'xpd_puts(str_lit);' prints the string
            add_token(STRING);
            break;
        case '\'':
            match_string('\'', &str_lit_ptr);
            // now 'str_lit' contains the string; that is, 'xpd_puts(str_lit);' prints the string
            add_token(STRING);
            break;
        // comment symbol means rest of line is discarded
        case '#':
            return;
        // default case handles the rest
        default:
            if (isdigit(current_char)) {
                match_number(&num_lit);
                xpd_puts("NUMBER RECEIVED: ");
                xpd_echo_int(num_lit, XPD_Flag_UnsignedDecimal);
                xpd_putc('\n');
                add_token(NUMBER);
            } else {
                // xpd_puts("Error: unexpected character.\n");
                // xpd_putc(current_char);
                // current++;
                // return;
            }
        // TODO: whitespace
    }
    current++;
}


/**
 * \brief Adds a token to the list that represents the instruction.
 */
void Lexer::add_token(lexemes token) {
    tokens[token_count] = token;
    token_count++;
}


/**
 * \brief Checks if the next character in the input matches the desired one.
 * \param [in] character The character to match to.
 * \return True if the character does match; false otherwise.
 */
bool Lexer::next_matches(char character) {
    // consider the next character
    current++;
    // if we are at the end, it cannot match
    if (end_reached()) {
        // no character consumed
        current--;
        return false;
    }
    // directly check if chracater matches
    if (line[current] == character) {
        // character consumed because it does match
        return true;
    } else {
        // no character consumed
        current--;
        return false;
    }
}


/**
 * \brief Recovers the string literal in the instruction.
 * \param [in] terminator The string terminator (either '"' or "'").
 * \param [in] output_ptr Pointer to where to store the literal value.
 */
void Lexer::match_string(char terminator, char ** output_ptr) {
    // make an index to track the literal's value string
    uint16_t i = 0;
    // start at character after the leading delimiter
    current++;
    // add each character to the literla that is not its terminator
    while (line[current] != terminator) {
        *(*output_ptr + i) = line[current];
        // consider next character
        i++;
        current++;
    }
    // move past the terminator character
    current++;
}


/**
 * \brief Recovers the number literal in the instruction.
 * \param [in] output_ptr Pointer to where to store the literal value.
 */
void Lexer::match_number(uint16_t * output_ptr) {
    char num_str[MAX_STR_LEN] = "";
    char * num_str_ptr = (char *) num_str;
    uint16_t i = 0;
    xpd_puts("NUMBER STRING READ: ");
    while (isdigit(line[current])) {
        xpd_putc(line[current]);
        *(num_str_ptr + i) = line[current];
        i++;
        current++;
    }
    current++;
    xpd_putc('\n');
    xpd_puts(num_str);
    *output_ptr = str_to_int(&num_str_ptr);
}


/**
 * \brief Determines if a particular character is a numerical digit or not.
 * \param [in] character The character to test.
 */
bool Lexer::isdigit(char character) {
    return (character >= '0') && (character <= '9');
}


/**
 *
 */
uint16_t Lexer::str_to_int(char ** num_str) {
    uint16_t cumulative_value = 0;
    uint16_t i = MAX_STR_LEN - 1;
    while (*(*num_str + i) == '\0') {
        i--;
    }
    xpd_putc('\n');
    uint16_t num_digits = i++;
    for (; i > 0; i--) {
        uint16_t value =  *(*num_str + i - 1) - 48;
        uint16_t position = 10 * (num_digits - (i - 1));
        xpd_puts("HERE: ");
        xpd_echo_int(num_digits - (i - 1), XPD_Flag_UnsignedDecimal);
        xpd_putc('-');
        xpd_echo_int(value, XPD_Flag_UnsignedDecimal);
        xpd_putc('-');
        xpd_echo_int(cumulative_value, XPD_Flag_UnsignedDecimal);
        xpd_putc('\n');
        for (uint16_t j = 0; j < position; j++) {
            cumulative_value = cumulative_value + value;
        }
        // uint16_t j = 0;
        // while (j != 10 * (num_digits - (i - 1))) {
        //     cumulative_value = cumulative_value + value;
        //     j++;
        // }
    }
    return cumulative_value;
}


/**
 * \brief Helper function to determine if we are at the instruction's end or not.
 * \return True if the end has been reached; false otherwise.
 */
bool Lexer::end_reached() {
    return current >= length - 1;
}
