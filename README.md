# Python Interpreter

This repository contains the code for a Python interpreter for the XInC2 processor on the C3 board, both made by Eleven Engineering.

### Running Instructions

Run ```open_distro_window``` on your command prompt to open the MinGW terminal.
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
