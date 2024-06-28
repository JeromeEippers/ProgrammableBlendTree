#ifndef _INC_PBT_PYTHONINIT_
#define _INC_PBT_PYTHONINIT_

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdbool.h>

typedef struct PbtPythonEnv
{
    PyObject* main_module;
    PyObject* globals;
    PyObject* locals;

    PyObject* skeleton_read_class;
    PyObject* skeleton_write_class;
    PyObject* pose_read_class;
    PyObject* pose_write_class;
    PyObject* animation_write_class;
} PbtPythonEnv;

bool pbt_init_python();
void pbt_deinit_python();
PbtPythonEnv * pbt_python_env();

bool pbt_python_begin_script_execution(const char * script);
bool pbt_python_call_main();
void pbt_python_end_script_execution();

typedef void (*py_instance_runner_t)(const char* name, PyObject* instance, void* data);
void pbt_python_foreach_instance(py_instance_runner_t callback, PyObject* classtype, void* data);

#endif