/*********************************************************************************
* Description: The lexer (the first stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef LEXER_H
#define LEXER_H


#include <XPD.h>


// the maximum number of characters in a line of input
#ifndef MAX_INPUT_LEN
#define MAX_INPUT_LEN 64
#endif

// the maximum number of tokens in a line of input
#ifndef MAX_INPUT_TOKENS
#define MAX_INPUT_TOKENS 64
#endif

// the maximum number of literals in a line of input
#ifndef MAX_LITS
#define MAX_LITS 16
#endif

// the maximum number of characters in a literal
#ifndef MAX_LIT_LEN
#define MAX_LIT_LEN 32
#endif

// the maximum number of identifiers in a line of input
#ifndef MAX_IDENTIFIERS
#define MAX_IDENTIFIERS 16
#endif

// the maximum number of characters in any identifier
#ifndef MAX_IDENTIFIER_LEN
#define MAX_IDENTIFIER_LEN 32
#endif


/**
 * \brief The list of all possible tokens in Python.
 */
enum lexemes {
    // so that the array is 'nullable'
    EMPTY,
    // types of brackets
    L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_BRACKET, R_BRACKET,
    // significant symbols
    COMMA, DOT, COLON, SEMICOLON, ARROW,
    // arithmetic operators
    PLUS, MINUS, STAR, SLASH, PERCENT, AT, D_STAR, D_SLASH,
    // bitwise operations
    B_AND, B_OR, B_XOR, B_NOT, B_SLL, B_SAR,
    // assignment operators
    ASSIGN, A_ASSIGN, S_ASSIGN, M_ASSIGN, I_ASSIGN, D_ASSIGN, R_ASSIGN, E_ASSIGN, F_ASSIGN, BA_ASSIGN, BO_ASSIGN, BX_ASSIGN, BL_ASSIGN, BR_ASSIGN, W_ASSIGN,
    // comparison operators
    EQUAL, GREATER, LESS, N_EQUAL, G_EQUAL, L_EQUAL,
    // literals
    STRING, NUMBER,
    // names of things like variables
    IDENTIFIER,
    // reserved keywords
    TRUE, FALSE, NONE, AND, OR, NOT, IS, ISNOT, IF, ELIF, ELSE,    // selection
    FOR, WHILE, CONTINUE, BREAK, PASS, IN, NOTIN,                 // iteration
    DEF, RETURN, YIELD, CLASS, LAMBDA,                      // control flow
    TRY, FINALLY, EXCEPT, RAISE,                            // exceptions
    IMPORT, FROM, WITH, AS,                                 // linking
    GLOBAL, NONLOCAL, ASYNC, AWAIT, ASSERT, DEL,            // other
};


// for ease of printing
const char * const token_names[] = {
    "NULL",
    "L_PAREN", "R_PAREN", "L_BRACE", "R_BRACE", "L_BRACKET", "R_BRACKET",
    "COMMA", "DOT", "COLON", "SEMICOLON", "ARROW",
    "PLUS", "MINUS", "STAR", "SLASH", "PERCENT", "AT", "D_STAR", "D_SLASH",
    "B_AND", "B_OR", "B_XOR", "B_NOT", "B_SLL", "B_SAR",
    "ASSIGN", "A_ASSIGN", "S_ASSIGN", "M_ASSIGN", "I_ASSIGN", "D_ASSIGN", "R_ASSIGN", "E_ASSIGN", "F_ASSIGN", "BA_ASSIGN", "BO_ASSIGN", "BX_ASSIGN", "BL_ASSIGN", "BR_ASSIGN", "W_ASSIGN",
    "EQUAL", "GREATER", "LESS", "N_EQUAL", "G_EQUAL", "L_EQUAL",
    "STRING", "NUMBER",
    "IDENTIFIER",
    "TRUE", "FALSE", "NONE", "AND", "OR", "NOT", "IS", "ISNOT", "IF", "ELIF", "ELSE",
    "FOR", "WHILE", "CONTINUE", "BREAK", "PASS", "IN", "NOTIN",
    "DEF", "RETURN", "YIELD", "CLASS", "LAMBDA",
    "TRY", "FINALLY", "EXCEPT", "RAISE",
    "IMPORT", "FROM", "WITH", "AS",
    "GLOBAL", "NONLOCAL", "ASYNC", "AWAIT", "ASSERT", "DEL",
};
// for ease of printing
const char * const token_symbols[] = {
    "",
    "(", ")", "{", "}", "[", "]",
    ",", ".", ":", ";", "->",
    "+", "-", "*", "/", "%", "@", "**", "//",
    "&", "|", "^", "~", "<<", ">>",
    "=", "+=", "-=", "*=", "@=", "/=", "%=", "**=", "//=", "&=", "|=", "^=", "<<=", ">>=", ":=",
    "=", ">", "<", "!=", ">=", "<=",
    "str", "int",
    "name",
    "True", "False", "None", "and", "or", "not", "is", "is not", "if", "elif", "else",
    "for", "while", "continue", "break", "pass", "in", "not in",
    "def", "return", "yield", "class", "lambda",
    "try", "finally", "except", "raise",
    "import", "from", "with", "as",
    "global", "nonlocal", "async", "await", "assert", "del",
};


/**
 * \brief Holds information about a command.
 */
struct lexed_command {
    // the list of all tokens and other items in the order they appear in the input
    lexemes tokens[MAX_INPUT_TOKENS];
    char str_lits[MAX_LITS][MAX_LIT_LEN];
    uint16_t num_lits[MAX_LITS];
    char identifiers[MAX_IDENTIFIERS][MAX_IDENTIFIER_LEN];
    // the number of non-null values of each type produced
    uint16_t token_count = 0;
    uint16_t str_lit_count = 0;
    uint16_t num_lit_count = 0;
    uint16_t identifier_count = 0;
};


/**
 * \brief The lexer of the interpreter; it takes a string as input and a list of tokens as output.
 */
class Lexer {
    private:
        // stores the input line to be lexed
        char line[MAX_INPUT_LEN] = "";
        // information extracted about the input
        lexed_command * command_info;
        // the current character index being read
        uint16_t current = 0;
        // the number of non-null input characters to decode
        uint16_t length = 0;
        // main logic of the lexer; maps charcters to tokens
        void scan_next_token();
        // adds a value to the end of that value's list
        void add_token(lexemes token);
        void add_str_lit(char * str_lit);
        void add_num_lit(uint16_t num_lit);
        void add_identifier(char * identifier);
        // checks if the next character in the input matches a certain value
        bool next_matches(char character);
        // parses a literal's value
        void match_string(char terminator, char ** output_ptr);
        void match_number(uint16_t * output_ptr);
        void match_identifier(char ** output_ptr);
        lexemes iskeyword(char ** input_ptr);
        // checks if there are any more characters to be read
        bool end_reached();
        // for error handling
        bool error_occurred = false;
        bool has_error();

    public:
        // basic constructor for the class
        Lexer(char ** input, lexed_command * output);
        // converts the input line into a list of tokens
        uint16_t scan_input();
};


#endif
