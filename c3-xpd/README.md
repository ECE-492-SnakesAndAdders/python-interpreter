# Python Interpreter

This repository contains the code for a Python interpreter for the XInC2 processor on the C3 board, both made by Eleven Engineering. Specific details on supported functionality [is available](https://github.com/ECE-492-SnakesAndAdders/python-interpreter/blob/main/INFO.md).

### Version

This is the XPD version of the interpreter. It runs directly on the C3 board and is accessible through a terminal interface on the host machine. This version of the interpreter supports only the evaluation of expressions.

### Running Instructions

To run this program, plug in the C3 board to the XPD board and plug in both to a host computer. Then, execute the following commands in the project root directory:

```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
sxc-xdt --hid -t --hex python-interpreter.hex
```

This will flash the program to the board. After turning the board off and back on again twice, the interpreter will start.

To re-launch the interpreter, simply run `sxc-xdt --hid -t --hex python-interpreter.hex` again. To reflect changes in the code, re-run `make` first.

You may have to run ```open_distro_window``` first to open the MinGW terminal.
