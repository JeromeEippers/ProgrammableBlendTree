#ifndef _INC_PYTHONINIT_
#define _INC_PYTHONINIT_

#define PY_SSIZE_T_CLEAN
#include<Python.h>
#include <stdbool.h>

typedef struct PythonEnv
{
    PyObject* main_module;
    PyObject* globals;

    PyObject* input_skeleton_buffer_class;
    PyObject* output_skeleton_buffer_class;
    PyObject* input_animation_buffer_class;
    PyObject* output_animation_buffer_class;
} PythonEnv;

bool init_python();
void deinit_python();
PythonEnv * python_env();

#endif