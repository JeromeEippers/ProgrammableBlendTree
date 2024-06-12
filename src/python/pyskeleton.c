#include "pyskeleton.h"
#include "../log.h"
#include <malloc.h>



// callback to call the python function
static void read_skeleton_from_python(const char* name, PyObject* instance, void* data);



PbtSkeleton * pbt_python_create_skeleton_from_file(const char* python_script_path)
{
    FILE *fp = fopen(python_script_path, "r");
    assert(fp != NULL);

    fseek(fp, 0 , SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0 , SEEK_SET);
    char *text = calloc(sizeof(char), fileSize+1);
    fread(text, sizeof(char), fileSize, fp);

    fclose(fp);

    PbtSkeleton * skeleton = pbt_python_create_skeleton_from_string(text);
    free(text);

    return skeleton;
}


PbtSkeleton * pbt_python_create_skeleton_from_string(const char* python_script)
{
    PbtSkeleton* skeleton = NULL;

    if (pbt_python_begin_script_execution(python_script))
    {
        if(pbt_python_call_main())
        {
            skeleton = calloc(1, sizeof(PbtSkeleton));
            pbt_python_foreach_instance(read_skeleton_from_python, pbt_python_env()->output_skeleton_buffer_class, (void*)skeleton);
        }
        pbt_python_end_script_execution();
    }

    return skeleton;
}



bool bpt_python_fill_skeleton_struct_from_output_buffer(PyObject* instance, PbtSkeleton* skeleton)
{
    assert(skeleton->bone_count == 0);

    PyObject * ret;

    ret = PyObject_CallMethod(instance, "bone_count", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call bone_count method");
        pbt_log_python_error();
        return false;
    }

    skeleton->bone_count = (int)PyLong_AS_LONG(ret);
    Py_DECREF(ret);

    if (skeleton->bone_count == 0)
    {
        pbt_log_warn("empty skeleton");
        pbt_skeleton_free(skeleton);
        return false;
    }
    
    // get the names
    ret = PyObject_CallMethod(instance, "_get_names", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call _get_names method");
        pbt_log_python_error();
        pbt_skeleton_free(skeleton);
        return false;
    }
    
    Py_ssize_t number_of_names = PyList_Size(ret);
    
    // check that we have the same number of names as bones
    if((uint32_t)number_of_names != skeleton->bone_count)
    {
        pbt_log_error("The number of bones does not match the number of names");
        Py_DECREF(ret);
        pbt_skeleton_free(skeleton);
        return false;
    }

    // names offset 
    skeleton->names_offset = malloc(sizeof(uint32_t) * skeleton->bone_count);
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
    skeleton->names = calloc(full_length, sizeof(char));
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
        pbt_log_python_error();
        pbt_skeleton_free(skeleton);
        return false;
    }

    Py_ssize_t number_of_parents = PyList_Size(ret);
    // check that we have the same number of names as bones
    if((uint32_t)number_of_parents != skeleton->bone_count)
    {
        pbt_log_error("The number of bones does not match the number of parents");
        Py_DECREF(ret);
        pbt_skeleton_free(skeleton);
        return false;
    }

    skeleton->parents = malloc(sizeof(int32_t) * skeleton->bone_count);

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
        pbt_log_python_error();
        pbt_skeleton_free(skeleton);
        return false;
    }

    Py_ssize_t number_of_positions = PyList_Size(ret);
    // check that we have the same number of pos as bones
    if((int)number_of_positions != skeleton->bone_count * 3)
    {
        pbt_log_error("The number of bones does not match the number of positions");
        Py_DECREF(ret);
        pbt_skeleton_free(skeleton);
        return false;
    }
    
    skeleton->pos = malloc(sizeof(float) * skeleton->bone_count * 3);
    assert((long)skeleton->pos % 16 == 0); //make sure we are aligned to simd
    
    for(Py_ssize_t i=0 ; i<number_of_positions; ++i)
    {
        PyObject * value = PyList_GetItem(ret, i);
        skeleton->pos[i] = (float)PyFloat_AsDouble(value);
    }

    // decref position list
    Py_DECREF(ret);


    // get the quaternions
    ret = PyObject_CallMethod(instance, "_get_quats", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call _get_quats method");
        pbt_log_python_error();
        pbt_skeleton_free(skeleton);
        return false;
    }

    Py_ssize_t number_of_rotations = PyList_Size(ret);
    // check that we have the same number of pos as bones
    if((int)number_of_rotations != skeleton->bone_count * 4)
    {
        pbt_log_error("The number of bones does not match the number of rotations");
        Py_DECREF(ret);
        pbt_skeleton_free(skeleton);
        return false;
    }
    
    skeleton->quats = malloc(sizeof(float) * skeleton->bone_count * 4);
    assert((long)skeleton->quats % 16 == 0); //make sure we are aligned to simd
    
    for(Py_ssize_t i=0 ; i<number_of_rotations; ++i)
    {
        PyObject * value = PyList_GetItem(ret, i);
        skeleton->quats[i] = (float)PyFloat_AsDouble(value);
    }

    // decref rotations list
    Py_DECREF(ret);

    return true;
}


bool bpt_python_fill_input_buffer_with_skeleton(PbtSkeleton* skeleton, PyObject* instance)
{
    PyObject * ret, *func_name;
    Py_ssize_t bone_count = (Py_ssize_t)pbt_skeleton_bone_count(skeleton);

    // names 
    PyObject * namelist = PyList_New(bone_count);
    for(Py_ssize_t i=0; i < bone_count; ++i){
        PyList_SetItem(namelist, i, PyUnicode_FromString(pbt_skeleton_bone_name(skeleton, (uint32_t)i)));
    }

    func_name = PyUnicode_FromString("_set_names");
    ret = PyObject_CallMethodObjArgs(instance, func_name, namelist, NULL);
    Py_DECREF(func_name);
    Py_DECREF(namelist);

    if(ret == NULL)
    {
        pbt_log_error("Could not call _set_names method");
        pbt_log_python_error();
        return false;
    }
    Py_DECREF(ret);

    // parents
    PyObject * parentlist = PyList_New(bone_count);
    for(Py_ssize_t i=0; i < bone_count; ++i){
        PyList_SetItem(parentlist, i, PyLong_FromLong(pbt_skeleton_bone_parent(skeleton, (uint32_t)i)));
    }

    func_name = PyUnicode_FromString("_set_parents");
    ret = PyObject_CallMethodObjArgs(instance, func_name, parentlist, NULL);
    Py_DECREF(func_name);
    Py_DECREF(parentlist);

    if(ret == NULL)
    {
        pbt_log_error("Could not call _set_parents method");
        pbt_log_python_error();
        return false;
    }
    Py_DECREF(ret);

    // pos
    PyObject * poslist = PyList_New(bone_count*3);
    for(Py_ssize_t i=0; i < bone_count; ++i){
        Pbtfloat4 quat = pbt_skeleton_bone_pos(skeleton, (uint32_t)i);
        PyList_SetItem(poslist, i*3 + 0, PyFloat_FromDouble(quat.values[0]));
        PyList_SetItem(poslist, i*3 + 1, PyFloat_FromDouble(quat.values[1]));
        PyList_SetItem(poslist, i*3 + 2, PyFloat_FromDouble(quat.values[2]));
    }

    func_name = PyUnicode_FromString("_set_positions");
    ret = PyObject_CallMethodObjArgs(instance, func_name, poslist, NULL);
    Py_DECREF(func_name);
    Py_DECREF(poslist);

    if(ret == NULL)
    {
        pbt_log_error("Could not call _set_positions method");
        pbt_log_python_error();
        return false;
    }
    Py_DECREF(ret);

    // quats
    PyObject * quatlist = PyList_New(bone_count*4);
    for(Py_ssize_t i=0; i < bone_count; ++i){
        Pbtfloat4 quat = pbt_skeleton_bone_quat(skeleton, (uint32_t)i);
        PyList_SetItem(quatlist, i*4 + 0, PyFloat_FromDouble(quat.values[0]));
        PyList_SetItem(quatlist, i*4 + 1, PyFloat_FromDouble(quat.values[1]));
        PyList_SetItem(quatlist, i*4 + 2, PyFloat_FromDouble(quat.values[2]));
        PyList_SetItem(quatlist, i*4 + 3, PyFloat_FromDouble(quat.values[3]));
    }

    func_name = PyUnicode_FromString("_set_quats");
    ret = PyObject_CallMethodObjArgs(instance, func_name, quatlist, NULL);
    Py_DECREF(func_name);
    Py_DECREF(quatlist);

    if(ret == NULL)
    {
        pbt_log_error("Could not call _set_quats method");
        pbt_log_python_error();
        return false;
    }
    Py_DECREF(ret);

    return true;
}


static void read_skeleton_from_python(const char* name, PyObject* instance, void* data)
{
    PbtSkeleton *skeleton = (PbtSkeleton*) data;
    bpt_python_fill_skeleton_struct_from_output_buffer(instance, skeleton);
}