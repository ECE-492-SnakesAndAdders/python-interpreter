# Python Interpreter

This repository contains the code for a Python interpreter for the XInC2 processor on the C3 board, both made by Eleven Engineering.

### Running Instructions

To run this program on the build, execute the following commands in the project root directory:

```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
sxc-xdt --hid -t --hex python-interpreter.hex
```

This will flash the program to the board. After turning the board off and back on again twice, the program interpreter will start.

To re-launch the interpreter, simply run `sxc-xdt --hid -t --hex python-interpreter.hex`. To reflect changes in the code, re-run `make` first.
