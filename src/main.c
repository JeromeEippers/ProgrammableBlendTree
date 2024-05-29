#include <stdio.h>

#include "python/init.h"
#include <Python.h>

const char* script;

int main(void)
{
    printf("hello world\n");

    bool status = init_python();
    assert(status);

    PyObject* locals = PyDict_New();

    PyObject* ret;

    ret = PyRun_String(script, Py_file_input, python_env()->globals, locals);
    if (ret)
        Py_DECREF(ret);


    // PyObject *keys;
    // keys = PyDict_Keys(locals);
    // Py_ssize_t count = PyList_Size(keys);
    // for (Py_ssize_t i =0; i < count; ++i)
    // {
    //     PyObject* key = PyList_GetItem(keys, i);
    //     PyObject* str = PyObject_Str(key);
    //     printf("local key : %s\n", PyUnicode_1BYTE_DATA(str));
    //     Py_DECREF(str);
    // }
    // Py_DECREF(keys);

    PyObject *str;
    PyObject *function_main = PyDict_GetItemString(locals, "main");
    str = PyObject_Str(function_main);
    printf("function : %s\n", PyUnicode_1BYTE_DATA(str));
    Py_DECREF(str);

    // RUN THE FUNCTION
    PyDict_SetItemString(python_env()->globals, "output_skeleton", PyDict_GetItemString(locals, "output_skeleton"));
    ret = PyObject_CallFunction(function_main, NULL);
    if(ret)
        Py_DECREF(ret);
    else
        printf("Failed to execute main");
    PyDict_DelItemString(python_env()->globals, "output_skeleton");

    ret = PyObject_CallMethod(PyDict_GetItemString(locals, "output_skeleton"), "bone_count", NULL);
    if(ret)
    {
        printf("bone_count : %d\n", (int)PyLong_AS_LONG(ret));
        Py_DECREF(ret);
    }
    else
        printf("Failed to execute bone_count");


    // keys = PyDict_Keys(python_env()->globals);
    // count = PyList_Size(keys);
    // for (Py_ssize_t i =0; i < count; ++i)
    // {
    //     PyObject* key = PyList_GetItem(keys, i);
    //     PyObject* str = PyObject_Str(key);
    //     printf("global key : %s\n", PyUnicode_1BYTE_DATA(str));
    //     Py_DECREF(str);
    // }
    // Py_DECREF(keys);

    // Py_DECREF(locals);


    deinit_python();
    
    return 0;
}


const char* script = "\
output_skeleton = pbt.OutputSkeletonBuffer()            \n\
                                                        \n\
def main():                                             \n\
    output_skeleton.names=['Hips', 'Spine', 'Chest']    \n\
                                                        \n\
";
