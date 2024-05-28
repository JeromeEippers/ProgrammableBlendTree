#include <Python.h>

#include "init.h"

bool init_python()
{
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

    Py_InitializeFromConfig(&config);
    if(PyStatus_Exception(status))
    {
        printf("Failed initializing python\n");
        goto fail;
    }

    PyConfig_Clear(&config);
    return true;

fail:
    PyConfig_Clear(&config);
    return false;
}


void deinit_python()
{
    Py_Finalize();
}
