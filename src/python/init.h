#ifndef _INC_PYTHONINIT_
#define _INC_PYTHONINIT_

#define PY_SSIZE_T_CLEAN

#include <stdbool.h>

bool init_python();
void deinit_python();

#endif