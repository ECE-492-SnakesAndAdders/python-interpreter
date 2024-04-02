# Python Interpreter

This repository contains the code for a Python interpreter for the XInC2 processor on the C3 board, both made by Eleven Engineering. Specific details on supported functionality [is available](https://github.com/ECE-492-SnakesAndAdders/python-interpreter/blob/main/INFO.md).

### Version

This is the locally-ran version of the interpreter. It runs on any standard computer and is accessible through a terminal interface. This is the most advanced version of the interpreter; it supports not only the evaluation of expressions, but also the use of variables, selection, and iteration.

### Running Instructions

To run this program, execute the following commands in the project root directory:

```
g++ main.cpp -o main environment.cpp error.cpp evaluator.cpp interpreter.cpp lexer.cpp parser.cpp tree.cpp utility.cpp
./main
```
