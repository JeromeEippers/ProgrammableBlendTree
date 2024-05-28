#include <stdio.h>
#include <Python.h>

int main(void)
{
    printf("hello world\n");

    Py_Initialize();
    PyRun_SimpleString("print('Hello from python')");
    Py_Finalize();
    
    return 0;
}

