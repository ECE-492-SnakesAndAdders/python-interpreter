/*********************************************************************************
* Description: Initialization file to set up the project.
* Author(s): Isaac Joffe
* Copyright: University of Alberta, 2024
* License: CC-BY-4.0
*********************************************************************************/


#include <IOConfig.h>
#include <SystemClock.h>
#include "main.h"


void __attribute__((constructor)) Initialize() {
    sys_clock_init(crys_24_576_MHz, _49_152_MHz);
    io_set_config(0b1110011, io_PG0);
}
