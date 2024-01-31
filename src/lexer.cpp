/*********************************************************************************
* Description: The lexer (the first stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "lexer.h"


Lexer::Lexer(char ** input) {
    for (uint16_t i = 0; i < MAX_INPUT_LEN; i++) {
        line[i] = *(*input + i);
    }
    while (*(*input + length) != '\0') {
        length++;
    }
}


lexemes * Lexer::scan_input() {
    while (!(end_reached())) {
        start = current;
        scan_next_token();
    }
    for (uint16_t i = 0; i < MAX_INPUT_TOKENS; i++) {
        xpd_puts(names[tokens[i]]);
        xpd_putc(' ');
    }
    xpd_putc('\n');
    return tokens;
}


void Lexer::scan_next_token() {
    char current_char = line[current];
    switch (current_char) {
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
        case '+':
            add_token(next_matches('=') ? A_ASSIGN : PLUS);
            break;
        case '-':
            add_token(next_matches('=') ? S_ASSIGN : MINUS);
            break;
        case '*':
            add_token(next_matches('=') ? M_ASSIGN : STAR);
            break;
        case '/':
            add_token(next_matches('=') ? D_ASSIGN : SLASH);
            break;
        case '%':
            add_token(next_matches('=') ? R_ASSIGN : PERCENT);
            break;
        case '=':
            add_token(next_matches('=') ? EQUAL : ASSIGN);
            break;
        case '>':
            add_token(next_matches('=') ? G_EQUAL : GREATER);
            break;
        case '<':
            add_token(next_matches('=') ? L_EQUAL : LESS);
            break;
        case '!':
            add_token(next_matches('=') ? N_EQUAL : NOT);
            break;
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
    return (line[current] == character);
}


bool Lexer::end_reached() {
    return current >= length - 1;
}
