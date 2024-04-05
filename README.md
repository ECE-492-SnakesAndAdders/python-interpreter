# Python Interpreter

This repository contains the code for a Python interpreter for the XInC2 processor on the C3 board, both made by Eleven Engineering. Specific details on supported functionality [is available](https://github.com/ECE-492-SnakesAndAdders/python-interpreter/blob/main/INFO.md).

### Version

This is the locally-ran version of the interpreter (there is a separate [embedded version](https://github.com/ECE-492-SnakesAndAdders/python-interpreter/tree/c3-embed) and [XPD-based version](https://github.com/ECE-492-SnakesAndAdders/python-interpreter/tree/c3-xpd)). It runs on any standard computer and is accessible through a terminal interface. This is the most advanced version of the interpreter; it supports not only the evaluation of expressions, but also the use of variables, selection, and iteration.

### Running Instructions

To run this program, execute the following commands in the project root directory:

```
g++ src/main.cpp -I inc -o main src/environment.cpp src/error.cpp src/evaluator.cpp src/interpreter.cpp src/lexer.cpp src/parser.cpp src/tree.cpp src/utility.cpp
./main
```
