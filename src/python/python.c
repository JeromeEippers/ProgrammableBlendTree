#include "python.h"
#include "../log.h"

PbtPythonEnv environment;

static int is_buffer_instance(PyObject * object);

bool pbt_init_python()
{
    environment.globals = NULL;
    environment.main_module = NULL;
    environment.locals = NULL;

    PyStatus status;
    PyConfig config;

    PyConfig_InitPythonConfig(&config);
    
    status = PyConfig_SetBytesString(&config, &config.program_name, "ProgrammableBlendTree");
    if(PyStatus_Exception(status))
    {
        pbt_log_error("Failed to set the python program name");
        goto fail;
    }

    status = PyConfig_Read(&config);
    if(PyStatus_Exception(status))
    {
        pbt_log_error("Failed to load the config");
        goto fail;
    }

    status = PyWideStringList_Append(&config.module_search_paths, L"python_venv/lib64/python3.10/site-packages");
    if(PyStatus_Exception(status))
    {
        pbt_log_error("Failed setting the paths for python");
        goto fail;
    }

    status = PyWideStringList_Append(&config.module_search_paths, L"python/pbt/src");
    if(PyStatus_Exception(status))
    {
        pbt_log_error("Failed setting the paths for python");
        goto fail;
    }

    Py_InitializeFromConfig(&config);
    if(PyStatus_Exception(status))
    {
        pbt_log_error("Failed initializing python");
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
        pbt_log_error("could not import numpy");
        goto fail;
    }

    ret = PyRun_String("import pbt", Py_file_input, environment.globals, environment.globals);
    if (ret)
        Py_DECREF(ret);
    else
    {
        pbt_log_error("could not import pbt");
        goto fail;
    }

    //************************************
    // Get classes
    //************************************
    PyObject *pbt_module = PyDict_GetItemString(environment.globals, "pbt");
    environment.input_animation_buffer_class = PyObject_GetAttrString(pbt_module, "InputAnimBuffer");
    environment.output_animation_buffer_class = PyObject_GetAttrString(pbt_module, "OutputAnimBuffer");
    environment.input_skeleton_buffer_class = PyObject_GetAttrString(pbt_module, "InputSkeletonBuffer");
    environment.output_skeleton_buffer_class = PyObject_GetAttrString(pbt_module, "OutputSkeletonBuffer");

    PyConfig_Clear(&config);
    return true;

fail:
    PyConfig_Clear(&config);
    return false;
}


void pbt_deinit_python()
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


PbtPythonEnv * pbt_python_env()
{
    return &environment;
}

bool pbt_python_begin_script_execution(const char * script)
{
    assert(environment.locals == NULL);
    PyObject *ret;

    environment.locals = PyDict_New();

    ret = PyRun_String(script, Py_file_input, environment.globals, environment.locals);
    if (ret)
    {
        Py_DECREF(ret);

        // copy all the buffers from the locals to the globals
        PyObject *key, *value;
        Py_ssize_t pos = 0;

        while (PyDict_Next(environment.locals, &pos, &key, &value))
        {
            if(is_buffer_instance(value))
            {
                PyDict_SetItem(environment.globals, key, value);
            }
        }

        // make sure we didn't reimport the libraries
        assert(PyDict_GetItemString(environment.locals, "pbt") == NULL);
        assert(PyDict_GetItemString(environment.locals, "np") == NULL);
    }
    else
    {
        pbt_log_error("Could not parse python");
        pbt_log_python_error();
        Py_DECREF(environment.locals);
        environment.locals = NULL;
        return false;
    }

    return true;
}

bool pbt_python_call_main()
{
    assert(environment.locals != NULL);

    PyObject *function_main = PyDict_GetItemString(environment.locals, "main");
    if(function_main == NULL)
    {
        pbt_log_error("Missing main function");
        return false;
    }

    PyObject* ret = PyObject_CallFunction(function_main, NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not execute main function");
        pbt_log_python_error();
        return false;
    }
    
    Py_DECREF(ret);
    return true;
}

void pbt_python_end_script_execution()
{
    assert(environment.locals != NULL);

    // delete all the buffers from the globals
    PyObject *key, *value;
    Py_ssize_t pos = 0;

    while (PyDict_Next(environment.locals, &pos, &key, &value))
    {
        if(is_buffer_instance(value))
        {
            PyDict_DelItem(environment.globals, key);
        }
    }

    Py_DECREF(environment.locals);
    environment.locals = NULL;
}

void pbt_python_foreach_instance(py_instance_runner_t callback, PyObject* classtype, void* data)
{
    // loop for the search of the instance
    PyObject *key, *value;
    Py_ssize_t pos = 0;

    while (PyDict_Next(environment.globals, &pos, &key, &value))
    {
        if(PyObject_IsInstance(value, classtype))
        {
            PyObject* str = PyObject_Str(key);
            callback((const char *)PyUnicode_1BYTE_DATA(str), value, data);
            Py_DECREF(str);
        }
    }
}


static int is_buffer_instance(PyObject * object)
{
    return 
        PyObject_IsInstance(object, environment.input_skeleton_buffer_class) ||
        PyObject_IsInstance(object, environment.output_skeleton_buffer_class) ||
        PyObject_IsInstance(object, environment.input_animation_buffer_class) ||
        PyObject_IsInstance(object, environment.output_animation_buffer_class );
}