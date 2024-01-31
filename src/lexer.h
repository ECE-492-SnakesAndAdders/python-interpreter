/*********************************************************************************
* Description: The lexer (the first stage of the interpreter)
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef MAX_INPUT_LEN
#define MAX_INPUT_LEN 64
#endif

#ifndef MAX_INPUT_TOKENS
#define MAX_INPUT_TOKENS 64
#endif


/**
 * \brief The list of all possible tokens in Python.
 */
enum lexemes {
    // so that array is 'nullable'
    EMPTY,
    // types of brackets
    L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_BRACKET, R_BRACKET,
    // significant symbols
    COMMA, DOT, COLON, SEMICOLON,
    // arithmetic operators
    PLUS, MINUS, STAR, SLASH, PERCENT, D_STAR, D_SLASH,
    // assignment operators
    ASSIGN, A_ASSIGN, S_ASSIGN, M_ASSIGN, D_ASSIGN, R_ASSIGN, E_ASSIGN, F_ASSIGN,   // TODO: add the rest (&=, |=, ^=, >>=, <<=) later
    // comparison operators
    EQUAL, GREATER, LESS, N_EQUAL, G_EQUAL, L_EQUAL,
    // bitwise operations
    B_AND, B_OR, B_XOR, B_NOT, B_SLL, B_SAR,
    // literals
    STRING, NUMBER,
    // names of things like variables
    IDENTIFIER,
    // reserved keywords
    TRUE, FALSE, NONE, AND, OR, NOT, IS, IF, ELIF, ELSE,    // selection
    FOR, WHILE, CONTINUE, BREAK, PASS, IN,                  // iteration
    DEF, RETURN, YIELD, CLASS, LAMBDA,                      // control flow
    TRY, FINALLY, EXCEPT, RAISE,                            // exceptions
    IMPORT, FROM, WITH, AS,                                 // linking
    GLOBAL, NONLOCAL, ASYNC, AWAIT, ASSERT, DEL,            // other
};


// for ease of printing
const char * const names[] = {
    "NULL",
    "L_PAREN", "R_PAREN", "L_BRACE", "R_BRACE", "L_BRACKET", "R_BRACKET",
    "COMMA", "DOT", "COLON", "SEMICOLON",
    "PLUS", "MINUS", "STAR", "SLASH", "PERCENT", "D_STAR", "D_SLASH",
    "ASSIGN", "A_ASSIGN", "S_ASSIGN", "M_ASSIGN", "D_ASSIGN", "R_ASSIGN", "E_ASSIGN", "F_ASSIGN",
    "EQUAL", "GREATER", "LESS", "N_EQUAL", "G_EQUAL", "L_EQUAL",
    "B_AND", "B_OR", "B_XOR", "B_NOT", "B_SLL", "B_SAR",
    "STRING", "NUMBER",
    "IDENTIFIER",
    "TRUE", "FALSE", "NONE", "AND", "OR", "NOT", "IS", "IF", "ELIF", "ELSE",
    "FOR", "WHILE", "CONTINUE", "BREAK", "PASS", "IN",
    "DEF", "RETURN", "YIELD", "CLASS", "LAMBDA",
    "TRY", "FINALLY", "EXCEPT", "RAISE",
    "IMPORT", "FROM", "WITH", "AS",
    "GLOBAL", "NONLOCAL", "ASYNC", "AWAIT", "ASSERT", "DEL",
};


/**
 * \brief The lexer of the interpreter; it takes a string as input and a list of tokens as output.
 */
class Lexer {
    private:
        char line[MAX_INPUT_LEN] = "";
        lexemes tokens[MAX_INPUT_TOKENS];
        uint16_t start = 0;
        uint16_t current = 0;
        uint16_t length = 0;
        uint16_t token_count = 0;

    public:
        // basic constructor for the class
        Lexer(char ** input);
        //
        lexemes * scan_input();
        //
        void scan_next_token();
        //
        void add_token(lexemes token);
        //
        bool next_matches(char character);
        //
        bool end_reached();
};
