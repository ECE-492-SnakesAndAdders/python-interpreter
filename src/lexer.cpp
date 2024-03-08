/*********************************************************************************
* Description: The lexer (the first stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <XPD.h>
#include "error.h"
#include "lexer.h"
#include "utility.h"


/** Much of this code is based on Crafting Interpreters by Robert Nystrom.
    Specifically, this file takes inspiration mostly from these chapters:
      * Chapter 4 (https://craftinginterpreters.com/scanning.html)
      * Chapter 16 (https://craftinginterpreters.com/scanning-on-demand.html)
*/


/**
 * \brief Basic constructor for the lexer.
 * \param [in] input Pointer to the input string entered by the user.
 * \param [in] output Pointer to the location to store information about the lexed command.
 */
Lexer::Lexer(char ** input, lexed_command * output) {
    // transfer input string into object
    for (uint16_t i = 0; i < MAX_INPUT_LEN; i++) {
        line[i] = *(*input + i);
    }
    // get the number of non-null characters in the command
    while (*(*input + length) != '\0') {
        length++;
    }
    // store where to save the final output to
    command_info = output;
}


/**
 * \brief Reads the next character in the input and creates a corresponding token.
 */
void Lexer::scan_next_token() {
    // to store string and number literals as well as identifiers
    char str_lit[MAX_LIT_LEN] = "";
    char * str_lit_ptr = (char *) str_lit;
    uint16_t num_lit = 0;
    char identifier[MAX_IDENTIFIER_LEN] = "";
    char * identifier_ptr = (char *) identifier;

    // look at the current character in the string
    char current_char = line[current];
    // consider each possible case entered
    switch (current_char) {
        // unconditionally single-character tokens
        case '(':
            // approach: just add token no matter what
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
        case ';':
            add_token(SEMICOLON);
            break;
        case '~':
            add_token(B_NOT);
            break;
        // possible single- or double-character tokens
        case ':':
            // approach: add two-character version if second character matches, single-character version otherwise
            add_token(next_matches('=') ? W_ASSIGN : COLON);
            break;
        case '+':
            add_token(next_matches('=') ? A_ASSIGN : PLUS);
            break;
        case '-':
            // consider both possible second characters, only add single if next is neither desired character
            add_token(next_matches('=') ? S_ASSIGN : (next_matches('>') ? ARROW : MINUS));
            break;
        case '%':
            add_token(next_matches('=') ? R_ASSIGN : PERCENT);
            break;
        case '@':
            add_token(next_matches('=') ? I_ASSIGN : AT);
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
            // approach: attempt to add longest one possible (maximal munch)
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
            add_token(STRING);
            add_str_lit(str_lit);
            break;
        case '\'':
            match_string('\'', &str_lit_ptr);
            add_token(STRING);
            add_str_lit(str_lit);
            break;
        // deal with statement spread out over multiple lines
        case '\\':
            // TODO: allow multi-line statements to be read
            break;
        // comment symbol means rest of line is discarded
        case '#':
            return;
        // illegal characters that can never occur in a program
        case '$':
            report_error(SYNTAX, "invalid syntax");
            error_occurred = true;
            return;
        case '?':
            report_error(SYNTAX, "invalid syntax");
            error_occurred = true;
            return;
        case '`':
            report_error(SYNTAX, "invalid syntax");
            error_occurred = true;
            return;
        // default case handles the rest (number literals, identifiers, keywords, whitespace)
        default:
            // number literal case
            if (isdigit(current_char)) {
                match_number(&num_lit);
                add_token(NUMBER);
                add_num_lit(num_lit);
            // identifier and keyword case
            } else if (isalpha(current_char)) {
                match_identifier(&identifier_ptr);
                // map string read to a keyword for keyword case
                if (lexemes keyword = iskeyword(&identifier_ptr)) {
                    add_token(keyword);
                // leave raw string for identifier case
                } else {
                    add_token(IDENTIFIER);
                    add_identifier(identifier);
                }
            // all other cases
            } else {
                // TODO: consider whitespace
            }
    }
    // consider the next character along
    current++;
}


/**
 * \brief Adds a token to the list that represents the instruction.
 * \param [in] token The lexeme to be added to the list.
 */
void Lexer::add_token(lexemes token) {
    command_info -> tokens[command_info -> token_count] = token;
    command_info -> token_count++;
}


/**
 * \brief Adds a string literal to the list for the instruction.
 * \param [in] str_lit The string literal to be added to the list.
 */
void Lexer::add_str_lit(char * str_lit) {
    for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
        command_info -> str_lits[command_info -> str_lit_count][i] = *(str_lit + i);
    }
    command_info -> str_lit_count++;
}


/**
 * \brief Adds a number literal to the list for the instruction.
 * \param [in] num_lit The number literal to be added to the list.
 */
void Lexer::add_num_lit(uint16_t num_lit) {
    command_info -> num_lits[command_info -> num_lit_count] = num_lit;
    command_info -> num_lit_count++;
}


/**
 * \brief Adds an identifier to the list for the instruction.
 * \param [in] identifier The identifier to be added to the list.
 */
void Lexer::add_identifier(char * identifier) {
    for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
        command_info -> identifiers[command_info -> identifier_count][i] = *(identifier + i);
    }
    command_info -> identifier_count++;
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
    while ((line[current] != terminator) && (i < MAX_LIT_LEN)) {
        *(*output_ptr + i) = line[current];
        // consider next character
        i++;
        current++;
    }
    // if we never close the string, this is an error
    if (line[current] != terminator) {
        report_error(SYNTAX, "EOL while scanning string literal");
        error_occurred = true;
    }
}


/**
 * \brief Recovers the number literal in the instruction.
 * \param [in] output_ptr Pointer to where to store the literal value.
 */
void Lexer::match_number(uint16_t * output_ptr) {
    // track the number string to be parsed
    char num_str[MAX_LIT_LEN] = "";
    char * num_str_ptr = (char *) num_str;
    // add all numerical characters to a cumulative string
    uint16_t i = 0;
    while (isdigit(line[current]) && (i < MAX_LIT_LEN)) {
        *(num_str_ptr + i) = line[current];
        i++;
        current++;
    }
    // retreat since we looked at one more than we needed to
    current--;
    // convert string to integer and save into output parameter
    *output_ptr = stoi(&num_str_ptr, MAX_LIT_LEN);
}


/**
 * \brief Recovers the name of an identifier used in the instruction.
 * \param [in] output_ptr Pointer to where to store the identifier name.
 */
void Lexer::match_identifier(char ** output_ptr) {
    uint16_t i = 0;
    // add each character to the identifier until no more eligible characters
    while (isalphanumeric(line[current]) && (i < MAX_IDENTIFIER_LEN)) {
        *(*output_ptr + i) = line[current];
        // consider next character
        i++;
        current++;
    }
    // retreat since we looked at one more than we needed to
    current--;
}


/**
 * \brief Determines if a string of characters is a reserved keyword.
 * \param [in] input_ptr Pointer to where the string of characters to be tested is stored.
 * \return The lexeme for the keyword of interest; EMPTY (0) otherwise.
 */
lexemes Lexer::iskeyword(char ** input_ptr) {
    // attempt to map the identifier to a reserved keyword
    // initial switch statement offers considerable speedup (trie structure)
    if (**input_ptr) {
        switch (**input_ptr) {
            case 'a':
                // approach: check character-by-character for a match, ensuring the value has no extra characters
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'n') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'd') &&
                           !(*(*input_ptr + 3))) {
                    return AND;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 's') &&
                           !(*(*input_ptr + 2))) {
                    return AS;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 's') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'y') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'n') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'c') &&
                           !(*(*input_ptr + 5))) {
                    return ASYNC;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'w') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'a') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'i') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 't') &&
                           !(*(*input_ptr + 5))) {
                    return AWAIT;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 's') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 's') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'e') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'r') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 't') &&
                           !(*(*input_ptr + 6))) {
                    return ASSERT;
                }
                break;

            case 'b':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'r') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'e') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'a') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'k') &&
                           !(*(*input_ptr + 5))) {
                    return BREAK;
                }
                break;

            case 'c':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'o') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'n') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 't') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'i') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 'n') &&
                           (*(*input_ptr + 6)) && (*(*input_ptr + 6) == 'u') &&
                           (*(*input_ptr + 7)) && (*(*input_ptr + 7) == 'e') &&
                           !(*(*input_ptr + 8))) {
                    return CONTINUE;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'l') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'a') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 's') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 's') &&
                           !(*(*input_ptr + 5))) {
                    return CLASS;
                }
                break;

            case 'd':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'e') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'f') &&
                           !(*(*input_ptr + 3))) {
                    return DEF;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'e') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'l') &&
                           !(*(*input_ptr + 3))) {
                    return DEL;
                }
                break;

            case 'e':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'l') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'i') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'f') &&
                           !(*(*input_ptr + 4))) {
                    return ELIF;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'l') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 's') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'e') &&
                           !(*(*input_ptr + 4))) {
                    return ELSE;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'x') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'c') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'e') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'p') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 't') &&
                           !(*(*input_ptr + 6))) {
                    return EXCEPT;
                }
                break;

            case 'f':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'o') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'r') &&
                           !(*(*input_ptr + 3))) {
                    return FOR;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'i') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'n') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'a') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'l') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 'l') &&
                           (*(*input_ptr + 6)) && (*(*input_ptr + 6) == 'y') &&
                           !(*(*input_ptr + 7))) {
                    return FINALLY;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'r') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'o') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'm') &&
                           !(*(*input_ptr + 4))) {
                    return FROM;
                }
                break;

            case 'g':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'l') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'o') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'b') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'a') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 'l') &&
                           !(*(*input_ptr + 6))) {
                    return GLOBAL;
                }
                break;

            case 'i':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 's') &&
                           !(*(*input_ptr + 2))) {
                    return IS;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'f') &&
                           !(*(*input_ptr + 2))) {
                    return IF;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'n') &&
                           !(*(*input_ptr + 2))) {
                    return IN;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'm') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'p') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'o') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'r') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 't') &&
                           !(*(*input_ptr + 6))) {
                    return IMPORT;
                }
                break;

            case 'l':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'a') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'm') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'b') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'd') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 'a') &&
                           !(*(*input_ptr + 6))) {
                    return LAMBDA;
                }
                break;

            case 'n':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'o') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 't') &&
                           !(*(*input_ptr + 3))) {
                    return NOT;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'o') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'n') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'l') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'o') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 'c') &&
                           (*(*input_ptr + 6)) && (*(*input_ptr + 6) == 'a') &&
                           (*(*input_ptr + 7)) && (*(*input_ptr + 7) == 'l') &&
                           !(*(*input_ptr + 8))) {
                    return NONLOCAL;
                }
                break;

            case 'o':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'r') &&
                           !(*(*input_ptr + 2))) {
                    return OR;
                }
                break;

            case 'p':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'a') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 's') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 's') &&
                           !(*(*input_ptr + 4))) {
                    return PASS;
                }
                break;

            case 'r':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'e') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 't') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'u') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'r') &&
                           (*(*input_ptr + 5)) && (*(*input_ptr + 5) == 'n') &&
                           !(*(*input_ptr + 6))) {
                    return RETURN;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'a') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'i') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 's') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'e') &&
                           !(*(*input_ptr + 5))) {
                    return RAISE;
                }
                break;

            case 't':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'r') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'y') &&
                           !(*(*input_ptr + 3))) {
                    return TRY;
                }
                break;

            case 'w':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'h') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'i') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'l') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'e') &&
                           !(*(*input_ptr + 5))) {
                    return WHILE;
                } else if ((*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'i') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 't') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'h') &&
                           !(*(*input_ptr + 4))) {
                    return WITH;
                }
                break;

            case 'y':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'i') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'e') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'l') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'd') &&
                           !(*(*input_ptr + 5))) {
                    return YIELD;
                }
                break;

            case 'F':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'a') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'l') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 's') &&
                           (*(*input_ptr + 4)) && (*(*input_ptr + 4) == 'e') &&
                           !(*(*input_ptr + 5))) {
                    return FALSE;
                }
                break;

            case 'N':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'o') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'n') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'e') &&
                           !(*(*input_ptr + 4))) {
                    return NONE;
                }
                break;
                
            case 'T':
                if (       (*(*input_ptr + 1)) && (*(*input_ptr + 1) == 'r') &&
                           (*(*input_ptr + 2)) && (*(*input_ptr + 2) == 'u') &&
                           (*(*input_ptr + 3)) && (*(*input_ptr + 3) == 'e') &&
                           !(*(*input_ptr + 4))) {
                    return TRUE;
                }
                break;
        }
    }
    // not a keyword, like returning false
    return EMPTY;
}


/**
 * \brief Helper function to determine if we are at the instruction's end or not.
 * \return True if the end has been reached; false otherwise.
 */
bool Lexer::end_reached() {
    return current >= length;
}


/**
 * \brief Tells if an error has occurred while lexing a token.
 * \return True if an error has occurred; false otherwise.
 */
bool Lexer::has_error() {
    return error_occurred;
}


/**
 * \brief Scans the input string and returns a list of its tokens.
 * \return 0 if execution succeeded; non-zero value if an error occurred.
 */
uint16_t Lexer::scan_input() {
    // keep on reading next character until command is over
    while (!(end_reached())) {
        scan_next_token();
        // a syntax error at any time should stop all operations
        if (has_error()) {
            return 1;
        }
    }
    return 0;
}
