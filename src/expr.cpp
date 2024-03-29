/*********************************************************************************
* Description: Defines the various syntax tree nodes and associated functionality
* Author(s): Isaac Joffe
* Copyright: University of Alberta", "2024
* License: CC-BY-4.0
*********************************************************************************/


#include <cstdio>
#include <cstring>
#include "expr.h"
#include "lexer.h"
#include "parser.h"
#include "utility.h"


/** Much of this code is based on Crafting Interpreters by Robert Nystrom.
    Specifically, this file takes inspiration mostly from these chapters:
      * Chapter 5 (https://craftinginterpreters.com/representing-code.html)
      * Chapter 18 (https://craftinginterpreters.com/types-of-values.html)
      * Chapter 19 (https://craftinginterpreters.com/strings.html)
    Some inspiration was also taken from another person's partial implementation:
      * Loxy in C (https://github.com/gcatlin/loxy)
*/


/**
 * \brief Constructor for an assignment operation's syntax tree node.
 * \param [in] name The name of identifier to write into.
 * \param [in] right Pointer to the node representing the expression to assign.
 * \return A structure representing the syntax tree node.
 */
node make_new_assign(char name[], node * value) {
    node current;
    current.type = ASSIGN_NODE;
    for (int i = 0; i < MAX_IDENTIFIER_LEN; i++) {
        current.entry.assign_val.name[i] = name[i];
    }
    current.entry.assign_val.value = value;
    return current;
}


/**
 * \brief Constructor for a binary arithmetic operation's syntax tree node.
 * \param [in] left Pointer to the node representing the first operand.
 * \param [in] opcode The token representing the operation to complete.
 * \param [in] right Pointer to the node representing the second operand.
 * \return A structure representing the syntax tree node.
 */
node make_new_binary(node * left, lexemes opcode, node * right) {
    node current;
    current.type = BINARY_NODE;
    current.entry.binary_val.left = left;
    current.entry.binary_val.opcode = opcode;
    current.entry.binary_val.right = right;
    return current;
}


/**
 * \brief Constructor for a block's syntax tree node.
 * \param [in] statements Pointer to the a pointer representing the list of statements.
 * \return A structure representing the syntax tree node.
 */
node make_new_block(node ** statements) {
    node current;
    current.type = BLOCK_NODE;
    for (int i = 0; i < MAX_NUM_STMTS; i++) {
        current.entry.block_val.statements[i] = statements[i];
    }
    return current;
}


/**
 * \brief Constructor for a grouping's syntax tree node.
 * \param [in] expression Pointer to the node representing the nested expression.
 * \return A structure representing the syntax tree node.
 */
node make_new_grouping(node * expression) {
    node current;
    current.type = GROUPING_NODE;
    current.entry.grouping_val.expression = expression;
    return current;
}


/**
 * \brief Constructor for a if-else statement's syntax tree node.
 * \param [in] condition Pointer to the node representing the condition to branch on.
 * \param [in] ifbranch Pointer to the node representing the statements to run if the condition is true.
 * \param [in] elsebranch Pointer to the node representing the statements to run if the condition is false.
 * \return A structure representing the syntax tree node.
 */
node make_new_ifelse(node * condition, node * ifbranch, node * elsebranch) {
    node current;
    current.type = IFELSE_NODE;
    current.entry.ifelse_val.condition = condition;
    current.entry.ifelse_val.ifbranch = ifbranch;
    current.entry.ifelse_val.elsebranch = elsebranch;
    return current;
}


/**
 * \brief Constructor for a literal value's syntax tree node.
 * \param [in] value A structure representing the literal's value.
 * \return A structure representing the syntax tree node.
 */
node make_new_literal(literal_value value) {
    node current;
    current.type = LITERAL_NODE;
    current.entry.literal_val.type = value.type;
    // need to copy over actualdata contained
    if (current.entry.literal_val.type == NUMBER_VALUE) {
        current.entry.literal_val.data.number = value.data.number;
    } else if (current.entry.literal_val.type == STRING_VALUE) {
        for (int i = 0; i < MAX_LIT_LEN; i++) {
            current.entry.literal_val.data.string[i] = value.data.string[i];
        }
    }
    return current;
}


/**
 * \brief Constructor for a binary logical operation's syntax tree node.
 * \param [in] left Pointer to the node representing the first operand.
 * \param [in] opcode The token representing the operation to complete.
 * \param [in] right Pointer to the node representing the second operand.
 * \return A structure representing the syntax tree node.
 */
node make_new_logical(node * left, lexemes opcode, node * right) {
    node current;
    current.type = LOGICAL_NODE;
    current.entry.logical_val.left = left;
    current.entry.logical_val.opcode = opcode;
    current.entry.logical_val.right = right;
    return current;
}


/**
 * \brief Constructor for a unary operation's syntax tree node.
 * \param [in] opcode The token representing the operation to complete.
 * \param [in] right Pointer to the node representing the operand.
 * \return A structure representing the syntax tree node.
 */
node make_new_unary(lexemes opcode, node * right) {
    node current;
    current.type = UNARY_NODE;
    current.entry.unary_val.opcode = opcode;
    current.entry.unary_val.right = right;
    return current;
}


/**
 * \brief Constructor for an variable-fetching operation's syntax tree node.
 * \param [in] name The name of identifier to read from.
 * \return A structure representing the syntax tree node.
 */
node make_new_variable(char name[]) {
    node current;
    current.type = VARIABLE_NODE;
    for (int i = 0; i < MAX_IDENTIFIER_LEN; i++) {
        current.entry.variable_val.name[i] = name[i];
    }
    return current;
}


/**
 * \brief Converts a literal value to a properly-formatted string.
 * \param [in] value The literal value to be converted into a string.
 * \param [in] output_ptr Pointer to the output string buffer.
 */
void stringify_value(literal_value value, char ** output_ptr) {
    switch (value.type) {
        case FALSE_VALUE:
            sprintf(*output_ptr, "%sFalse\n", *output_ptr);
            break;

        case NONE_VALUE:
            break;

        case NUMBER_VALUE:
            sprintf(*output_ptr, "%s%d\n", *output_ptr, value.data.number);
            break;

        case STRING_VALUE:
            sprintf(*output_ptr, "%s\'%s\'\n", *output_ptr, value.data.string);
            break;

        case TRUE_VALUE:
            sprintf(*output_ptr, "%sTrue\n", *output_ptr);
            break;
    }
}


/**
 * \brief Prints the generated syntax tree for debugging.
 * \param [in] tree The tree (or subtree, or node) to print.
 */
void print_tree(node tree) {
    int i = 0;
    switch (tree.type) {
        case ASSIGN_NODE:
            printf(" ( ");
            printf(tree.entry.assign_val.name);
            printf(" GETS ");
            print_tree(*(tree.entry.assign_val.value));
            printf(" ) ");
            break;

        case BINARY_NODE:
            printf(" ( ");
            print_tree(*(tree.entry.binary_val.left));
            printf(token_names[tree.entry.binary_val.opcode]);
            print_tree(*(tree.entry.binary_val.right));
            printf(" ) ");
            break;

        case BLOCK_NODE:
            printf("BLOCK (\n");
            while (tree.entry.block_val.statements[i] != NULL) {
                printf("    STATEMENT: ");
                print_tree(*(tree.entry.block_val.statements[i]));
                printf("\n");
                i++;
            }
            printf(")");
            break;

        case GROUPING_NODE:
            printf(" ( ");
            print_tree(*(tree.entry.grouping_val.expression));
            printf(" ) ");
            break;

        case IFELSE_NODE:
            printf("if");
            printf(" ( ");
            print_tree(*(tree.entry.ifelse_val.condition));
            printf(" ) ");
            printf("then");
            print_tree(*(tree.entry.ifelse_val.ifbranch));
            printf("else");
            print_tree(*(tree.entry.ifelse_val.elsebranch));
            break;

        case LITERAL_NODE:
            if (tree.entry.literal_val.type == FALSE_VALUE) {
                printf(" False ");
            } else if (tree.entry.literal_val.type == NONE_VALUE) {
                printf(" None ");
            } else if (tree.entry.literal_val.type == NUMBER_VALUE) {
                printf(" ");
                printf("%d", tree.entry.literal_val.data.number);
                printf(" ");
            } else if (tree.entry.literal_val.type == STRING_VALUE) {
                printf(" ");
                printf(tree.entry.literal_val.data.string);
                printf(" ");
            } else if (tree.entry.literal_val.type == TRUE_VALUE) {
                printf(" True ");
            }
            break;

        case LOGICAL_NODE:
            printf(" ( ");
            print_tree(*(tree.entry.logical_val.left));
            printf(token_names[tree.entry.logical_val.opcode]);
            print_tree(*(tree.entry.logical_val.right));
            printf(" ) ");
            break;
            
        case UNARY_NODE:
            printf(" ( ");
            printf(token_names[tree.entry.unary_val.opcode]);
            print_tree(*(tree.entry.unary_val.right));
            printf(" ) ");
            break;

        case VARIABLE_NODE:
            printf(" ( ");
            printf(tree.entry.variable_val.name);
            printf(" ) ");
            break;
    }
}
