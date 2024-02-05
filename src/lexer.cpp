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
    for (uint16_t i = 0; i < MAX_LITS; i++) {
        // for (uint16_t j = 0; j < MAX_IDENTIFIER_LEN; j++) {
        //     str_lits[i][j] = 0;
        // }
        num_lits[i] = 0;
    }
    for (uint16_t i = 0; i < MAX_IDENTIFIERS; i++) {
        // for (uint16_t j = 0; j < MAX_IDENTIFIER_LEN; j++) {
        //     identifiers[i][j] = 0;
        // }
    }
    // get the number of non-null characters in the command
    while (*(*input + length) != '\0') {
        length++;
    }
}


/**
 * \brief scans the input string and returns a list of its tokens.
 */
void Lexer::scan_input() {
    // keep on reading next character until command is over
    while (!(end_reached())) {
        scan_next_token();
    }

    // ------------------------------------------------------------------------
    // FOR DEBUGGING; print each token to see that lexer works
    xpd_puts("\nPARSED INFO:\n");
    xpd_puts("Tokens: ");
    for (uint16_t i = 0; i < token_count; i++) {
        xpd_echo_int(tokens[i], XPD_Flag_UnsignedDecimal);
        xpd_putc(' ');
        xpd_puts(names[tokens[i]]);
        xpd_putc(',');
        xpd_putc(' ');
    }
    xpd_putc('\n');
    xpd_puts("Strings: ");
    for (uint16_t i = 0; i < str_lit_count; i++) {
        xpd_puts(str_lits[i]);
        xpd_putc(',');
        xpd_putc(' ');
    }
    xpd_putc('\n');
    xpd_puts("Numbers: ");
    for (uint16_t i = 0; i < num_lit_count; i++) {
        xpd_echo_int(num_lits[i], XPD_Flag_UnsignedDecimal);
        xpd_putc(',');
        xpd_putc(' ');
    }
    xpd_putc('\n');
    xpd_puts("Identifiers: ");
    for (uint16_t i = 0; i < identifier_count; i++) {
        xpd_puts(identifiers[i]);
        xpd_putc(',');
        xpd_putc(' ');
    }
    xpd_putc('\n');
    xpd_putc('\n');
    // ------------------------------------------------------------------------
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
            break;
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
            add_token(STRING);
            add_str_lit(str_lit);
            break;
        case '\'':
            match_string('\'', &str_lit_ptr);
            add_token(STRING);
            add_str_lit(str_lit);
            break;
        // comment symbol means rest of line is discarded
        case '#':
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
    tokens[token_count] = token;
    token_count++;
}


/**
 * \brief Adds a string literal to the list for the instruction.
 * \param [in] str_lit The string literal to be added to the list.
 */
void Lexer::add_str_lit(char * str_lit) {
    for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
        str_lits[str_lit_count][i] = *(str_lit + i);
    }
    str_lit_count++;
}


/**
 * \brief Adds a number literal to the list for the instruction.
 * \param [in] num_lit The number literal to be added to the list.
 */
void Lexer::add_num_lit(uint16_t num_lit) {
    num_lits[num_lit_count] = num_lit;
    num_lit_count++;
}


/**
 * \brief Adds an identifier to the list for the instruction.
 * \param [in] identifier The identifier to be added to the list.
 */
void Lexer::add_identifier(char * identifier) {
    for (uint16_t i = 0; i < MAX_LIT_LEN; i++) {
        identifiers[identifier_count][i] = *(identifier + i);
    }
    identifier_count++;
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
    *output_ptr = str_to_int(&num_str_ptr);
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
    if (strcmp(*input_ptr, "True")) {
        return TRUE;
    } else if (strcmp(*input_ptr, "False")) {
        return FALSE;
    } else if (strcmp(*input_ptr, "None")) {
        return NONE;
    } else if (strcmp(*input_ptr, "and")) {
        return AND;
    } else if (strcmp(*input_ptr, "or")) {
        return OR;
    } else if (strcmp(*input_ptr, "not")) {
        return NOT;
    } else if (strcmp(*input_ptr, "is")) {
        return IS;
    } else if (strcmp(*input_ptr, "if")) {
        return IF;
    } else if (strcmp(*input_ptr, "elif")) {
        return ELIF;
    } else if (strcmp(*input_ptr, "else")) {
        return ELSE;
    } else if (strcmp(*input_ptr, "for")) {
        return FOR;
    } else if (strcmp(*input_ptr, "while")) {
        return WHILE;
    } else if (strcmp(*input_ptr, "continue")) {
        return CONTINUE;
    } else if (strcmp(*input_ptr, "break")) {
        return BREAK;
    } else if (strcmp(*input_ptr, "pass")) {
        return PASS;
    } else if (strcmp(*input_ptr, "in")) {
        return IN;
    } else if (strcmp(*input_ptr, "def")) {
        return DEF;
    } else if (strcmp(*input_ptr, "return")) {
        return RETURN;
    } else if (strcmp(*input_ptr, "yield")) {
        return YIELD;
    } else if (strcmp(*input_ptr, "class")) {
        return CLASS;
    } else if (strcmp(*input_ptr, "lambda")) {
        return LAMBDA;
    } else if (strcmp(*input_ptr, "try")) {
        return TRY;
    } else if (strcmp(*input_ptr, "finally")) {
        return FINALLY;
    } else if (strcmp(*input_ptr, "except")) {
        return EXCEPT;
    } else if (strcmp(*input_ptr, "raise")) {
        return RAISE;
    } else if (strcmp(*input_ptr, "import")) {
        return IMPORT;
    } else if (strcmp(*input_ptr, "from")) {
        return FROM;
    } else if (strcmp(*input_ptr, "with")) {
        return WITH;
    } else if (strcmp(*input_ptr, "as")) {
        return AS;
    } else if (strcmp(*input_ptr, "global")) {
        return GLOBAL;
    } else if (strcmp(*input_ptr, "nonlocal")) {
        return NONLOCAL;
    } else if (strcmp(*input_ptr, "async")) {
        return ASYNC;
    } else if (strcmp(*input_ptr, "await")) {
        return AWAIT;
    } else if (strcmp(*input_ptr, "assert")) {
        return ASSERT;
    } else if (strcmp(*input_ptr, "del")) {
        return DEL;
    // not a keyword, like returning false
    } else {
        return EMPTY;
    }
}


/**
 * \brief Determines if a particular character is a numerical digit or not, like the standard isdigit().
 * \param [in] character The character to test.
 * \return True if the character is a digit; false otherwise.
 */
bool Lexer::isdigit(char character) {
    return (character >= '0') && (character <= '9');
}


/**
 * \brief Determines if a particular character is a letter or not, like the standard isalpha().
 * \param [in] character The character to test.
 * \return True if the character is a letter or underscore; false otherwise.
 */
bool Lexer::isalpha(char character) {
    return ((character >= 'a') && (character <= 'z')) ||
           ((character >= 'A') && (character <= 'Z')) ||
           (character == '_');
}


/**
 * \brief Determines if a particular character is a letter or digit not, like the standard isalphanumeric().
 * \param [in] character The character to test.
 * \return True if the character is a letter or digit; false otherwise.
 */
bool Lexer::isalphanumeric(char character) {
    return (isalpha(character) || isdigit(character));
}


/**
 * \brief Determines if two strings are equal (look the same).
 * \param [in] s1 The first string to be compared for equality.
 * \param [in] s1 The second string to be compared for equality.
 * \return True if teh two strings are equal; false otehrwise.
 */
bool Lexer::strcmp(const char * s1, const char * s2) {
    // traverse down strng, ensuring it exists and checking for equality
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return !((* (char *) s1) - (* (char *) s2));
}


/**
 * \brief Converts a string of numbers to an integer, like the standard atoi().
 * \param [in] num_str Pointer to the string to be converted.
 * \return The numerical integer representation of the string.
 */
uint16_t Lexer::str_to_int(char ** num_str) {
    // iterate backwards until the number's length is found
    uint16_t i = MAX_LIT_LEN - 1;
    while (*(*num_str + i) == '\0') {
        i--;
    }
    // cumulative value of the integer
    uint16_t value = 0;
    // add up contribution from each digit in the number
    for (uint16_t j = 0; (j < i + 1) && (j < MAX_LIT_LEN); j++) {
        // multiply current number by 10, which is a decimal point shift right
        value *= 10;
        // add latest value into new ones column
        // 48 is the ASCII value of '0', so offset numbers by that for real value
        value += (*(*num_str + j) - 48);
    }
    return value;
}


/**
 * \brief Helper function to determine if we are at the instruction's end or not.
 * \return True if the end has been reached; false otherwise.
 */
bool Lexer::end_reached() {
    return current >= length;
}
