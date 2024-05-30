#include "pyskeleton.h"
#include "../log.h"
#include <malloc.h>

bool bpt_python_fill_skeleton_struct_from_input_buffer(PyObject* instance, PbtSkeleton* skeleton)
{
    PyObject * ret;

    ret = PyObject_CallMethod(instance, "bone_count", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call bone_count method");
        return false;
    }

    skeleton->bone_count = (int)PyLong_AS_LONG(ret);
    Py_DECREF(ret);

    if (skeleton->bone_count == 0)
    {
        pbt_log_warn("empty skeleton");
    }
    
    // get the names
    ret = PyObject_CallMethod(instance, "_get_names", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call _get_names method");
        pbt_skeleton_delete(skeleton);
        return false;
    }
    
    Py_ssize_t number_of_names = PyList_Size(ret);
    
    // check that we have the same number of names as bones
    if((int)number_of_names != skeleton->bone_count)
    {
        pbt_log_error("The number of bones does not match the number of names");
        Py_DECREF(ret);
        pbt_skeleton_delete(skeleton);
        return false;
    }

    // names offset 
    skeleton->names_offset = (int*)malloc(sizeof(int) * skeleton->bone_count);
    skeleton->names_offset[0] = 0;
    size_t full_length = 0;
    for(Py_ssize_t i=0 ; i<number_of_names; ++i)
    {
        PyObject * name = PyList_GetItem(ret, i);
        size_t len = strlen((const char *)PyUnicode_1BYTE_DATA(name))+1;
        full_length += len;
        if(i < number_of_names-1)
        {
            skeleton->names_offset[i+1] = full_length;
        }
    }

    // names
    skeleton->names = (char*)calloc(full_length, sizeof(char));
    for(Py_ssize_t i=0 ; i<number_of_names; ++i)
    {
        PyObject * name = PyList_GetItem(ret, i);
        strcpy((char*)(skeleton->names + skeleton->names_offset[i]), (const char *)PyUnicode_1BYTE_DATA(name));
    }

    // decref name list
    Py_DECREF(ret);


    // get the parents
    ret = PyObject_CallMethod(instance, "_get_parents", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call _get_parents method");
        pbt_skeleton_delete(skeleton);
        return false;
    }

    Py_ssize_t number_of_parents = PyList_Size(ret);
    // check that we have the same number of names as bones
    if((int)number_of_parents != skeleton->bone_count)
    {
        pbt_log_error("The number of bones does not match the number of parents");
        Py_DECREF(ret);
        pbt_skeleton_delete(skeleton);
        return false;
    }

    skeleton->parents = (int*)malloc(sizeof(int) * skeleton->bone_count);

    for(Py_ssize_t i=0 ; i<number_of_parents; ++i)
    {
        PyObject * value = PyList_GetItem(ret, i);
        skeleton->parents[i] = (int)PyLong_AsLong(value);
    }

    // decref parent list
    Py_DECREF(ret);

    // get the positions
    ret = PyObject_CallMethod(instance, "_get_positions", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call _get_positions method");
        pbt_skeleton_delete(skeleton);
        return false;
    }

    Py_ssize_t number_of_positions = PyList_Size(ret);
    // check that we have the same number of pos as bones
    if((int)number_of_positions != skeleton->bone_count * 3)
    {
        pbt_log_error("The number of bones does not match the number of positions");
        Py_DECREF(ret);
        pbt_skeleton_delete(skeleton);
        return false;
    }
    
    skeleton->pos = (float*)malloc(sizeof(float) * skeleton->bone_count * 3);
    assert((long)skeleton->pos % 16 == 0); //make sure we are aligned to simd
    
    assert(false); //TODO fill position and quats


    // decref position list
    Py_DECREF(ret);

    return true;
}