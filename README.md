# Python Interpreter

This repository contains the code for a Python interpreter for the XInC2 processor on the C3 board, both made by Eleven Engineering. Specific details on supported functionality [is available](https://github.com/ECE-492-SnakesAndAdders/python-interpreter/blob/main/INFO.md).

### Running Instructions

To run this program on the C3 board, execute the following commands in the project root directory:

```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
sxc-xdt --hid -t --hex python-interpreter.hex
```

This will flash the program to the board. After turning the board off and back on again twice, the program interpreter will start.

To re-launch the interpreter, simply run `sxc-xdt --hid -t --hex python-interpreter.hex` again. To reflect changes in the code, re-run `make` first.

You may have to run ```open_distro_window``` first to open the MinGW terminal.
