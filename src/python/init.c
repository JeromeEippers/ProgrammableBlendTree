#include "init.h"

PythonEnv environment;

bool init_python()
{
    environment.globals = NULL;
    environment.main_module = NULL;

    PyStatus status;
    PyConfig config;

    PyConfig_InitPythonConfig(&config);
    
    status = PyConfig_SetBytesString(&config, &config.program_name, "ProgrammableBlendTree");
    if(PyStatus_Exception(status))
    {
        printf("Failed to set the python program name\n");
        goto fail;
    }

    status = PyConfig_Read(&config);
    if(PyStatus_Exception(status))
    {
        printf("Failed to load the config\n");
        goto fail;
    }

    status = PyWideStringList_Append(&config.module_search_paths, L"python_venv/lib/python3.10/site-packages");
    if(PyStatus_Exception(status))
    {
        printf("Failed setting the paths for python\n");
        goto fail;
    }
    status = PyWideStringList_Append(&config.module_search_paths, L"python/src");
    if(PyStatus_Exception(status))
    {
        printf("Failed setting the paths for python\n");
        goto fail;
    }

    Py_InitializeFromConfig(&config);
    if(PyStatus_Exception(status))
    {
        printf("Failed initializing python\n");
        goto fail;
    }

    environment.main_module = PyImport_ImportModule("__main__");
    environment.globals = PyModule_GetDict(environment.main_module);

    PyObject* ret;
    ret = PyRun_String("import numpy as np", Py_file_input, environment.globals, environment.globals);
    if (ret)
        Py_DECREF(ret);
    else
    {
        printf("could not import numpy\n");
        goto fail;
    }

    ret = PyRun_String("import pbt", Py_file_input, environment.globals, environment.globals);
    if (ret)
        Py_DECREF(ret);
    else
    {
        printf("could not import pbt\n");
        goto fail;
    }

    //************************************
    // Get classes
    //************************************
    PyObject *pbt_module = PyDict_GetItemString(environment.globals, "pbt");
    environment.input_animation_buffer_class = PyObject_GetAttrString(pbt_module, "InputAnimBuffer");
    environment.output_animation_buffer_class = PyObject_GetAttrString(pbt_module, "OutputAnimBuffer");
    environment.input_skeleton_buffer_class = PyObject_GetAttrString(pbt_module, "InputSkeletonBuffer");
    environment.output_animation_buffer_class = PyObject_GetAttrString(pbt_module, "OutputSkeletonBuffer");

    // str = PyObject_Str(input_anim_buffer_class);
    // printf("pbt : %s\n", PyUnicode_1BYTE_DATA(str));
    // Py_DECREF(str);

    PyConfig_Clear(&config);
    return true;

fail:
    PyConfig_Clear(&config);
    return false;
}


void deinit_python()
{
    if (environment.input_animation_buffer_class)
        Py_DECREF(environment.input_animation_buffer_class);
    if (environment.output_animation_buffer_class)
        Py_DECREF(environment.output_animation_buffer_class);
    if (environment.input_skeleton_buffer_class)
        Py_DECREF(environment.input_skeleton_buffer_class);
    if (environment.output_animation_buffer_class)
        Py_DECREF(environment.output_animation_buffer_class);
    if (environment.main_module)
        Py_DECREF(environment.main_module);
    Py_Finalize();
}


PythonEnv * python_env()
{
    return &environment;
}