#include <stdio.h>

#include "python/init.h"
#include <Python.h>

int main(void)
{
    printf("hello world\n");

    bool status = init_python();
    assert(status);

    PyRun_SimpleString("import numpy as np \nprint('Hello from python\\n', np.eye(4, dtype=np.float32))");

    deinit_python();
    
    return 0;
}

