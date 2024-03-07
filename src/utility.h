/*********************************************************************************
* Description: Some helper functions for the main Python interpreter file
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#ifndef UTILITY_H
#define UTILITY_H


// for emptying arrays between runs
void memclear(char array[], uint16_t length);
// for checking if a character is a number
bool isdigit(char character);
// for checking if a character is a letter
bool isalpha(char character);
// for checking if a character is a number or letter
bool isalphanumeric(char character);
// for checking if a string is made of only letters and/or numbers
bool isalphanumeric(char * str);
// // string concatenation function
// void strcat(char s1[], char s2[], char ** s3);
// for checking if two strings match
bool strcmp(const char * s1, const char * s2);
// for converting between numerical strings and integer values
uint16_t stoi(char ** num_str, uint16_t str_len);
void itos(char ** num_str, uint16_t num_value);
// for printing output reliably
void print_string(char ** str);
void print_string(const char * str);


#endif
