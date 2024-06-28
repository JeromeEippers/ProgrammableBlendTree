#include "pyanimation.h"
#include "pyskeleton.h"
#include "../core/log.h"
#include <malloc.h>

static void push_skeleton_in_python(const char* name, PyObject* instance, void* data);
static void read_animation_from_python(const char* name, PyObject* instance, void* data);

struct anim_skel_callback_data{
    PbtSkeleton* skeleton;
    PbtAnimation* animation;
};


PbtAnimation * pbt_python_create_animation_from_file(const char* python_script_path, PbtSkeleton* skeleton)
{
    FILE *fp = fopen(python_script_path, "r");
    assert(fp != NULL);

    fseek(fp, 0 , SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0 , SEEK_SET);
    char *text = calloc(sizeof(char), fileSize+1);
    fread(text, sizeof(char), fileSize, fp);

    fclose(fp);

    PbtAnimation * animation = pbt_python_create_animation_from_string(text, skeleton);
    free(text);

    return animation;
}

PbtAnimation * pbt_python_create_animation_from_string(const char* python_script, PbtSkeleton* skeleton)
{
    PbtAnimation* animation = NULL;

    if (pbt_python_begin_script_execution(python_script))
    {
        pbt_python_foreach_instance(push_skeleton_in_python, pbt_python_env()->skeleton_read_class, (void*)skeleton);
        if(pbt_python_call_main())
        {
            animation = calloc(1, sizeof(PbtAnimation));
            struct anim_skel_callback_data data;
            data.animation = animation;
            data.skeleton = skeleton;
            pbt_python_foreach_instance(read_animation_from_python, pbt_python_env()->animation_write_class, (void*)&data);
        }
        pbt_python_end_script_execution();
    }

    return animation;
}

bool bpt_python_fill_animation_struct_from_output_buffer(PyObject* instance, PbtSkeleton* skeleton, PbtAnimation* animation)
{
    assert(animation->frame_count==0);

    PyObject * ret;

    ret = PyObject_CallMethod(instance, "bone_count", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call bone_count method");
        pbt_log_python_error();
        pbt_animation_free(animation);
        return false;
    }

    uint32_t bone_count = (uint32_t)PyLong_AS_LONG(ret);
    Py_DECREF(ret);

    if (skeleton->bone_count != bone_count)
    {
        pbt_log_error("animation bone count does not match skeleton bone count");
        pbt_animation_free(animation);
        return false;
    }

    ret = PyObject_CallMethod(instance, "frame_count", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call frame_count method");
        pbt_log_python_error();
        pbt_animation_free(animation);
        return false;
    }

    animation->frame_count = (uint32_t)PyLong_AS_LONG(ret);
    Py_DECREF(ret);

    if (animation->frame_count == 0)
    {
        pbt_log_warn("empty animation");
        pbt_animation_free(animation);
        return false;
    }


    // get the positions
    ret = PyObject_CallMethod(instance, "_get_positions", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call _get_positions method");
        pbt_log_python_error();
        pbt_animation_free(animation);
        return false;
    }

    Py_ssize_t number_of_positions = PyList_Size(ret);
    if((int)number_of_positions != skeleton->bone_count * animation->frame_count * 3)
    {
        pbt_log_error("The position list does not match the number of frames and bones");
        Py_DECREF(ret);
        pbt_animation_free(animation);
        return false;
    }
    
    animation->pos = malloc(sizeof(float) * skeleton->bone_count * animation->frame_count * 3);
    assert((long)animation->pos % 16 == 0); //make sure we are aligned to simd
    
    for(Py_ssize_t i=0 ; i<number_of_positions; ++i)
    {
        PyObject * value = PyList_GetItem(ret, i);
        animation->pos[i] = (float)PyFloat_AsDouble(value);
    }

    // decref position list
    Py_DECREF(ret);


    // get the rotations
    ret = PyObject_CallMethod(instance, "_get_quats", NULL);
    if(ret == NULL)
    {
        pbt_log_error("Could not call _get_quats method");
        pbt_log_python_error();
        pbt_animation_free(animation);
        return false;
    }

    Py_ssize_t number_of_quaternions = PyList_Size(ret);
    if((int)number_of_quaternions != skeleton->bone_count * animation->frame_count * 4)
    {
        pbt_log_error("The rotation list does not match the number of frames and bones");
        Py_DECREF(ret);
        pbt_animation_free(animation);
        return false;
    }
    
    animation->quats = malloc(sizeof(float) * skeleton->bone_count * animation->frame_count * 4);
    assert((long)animation->quats % 16 == 0); //make sure we are aligned to simd
    
    for(Py_ssize_t i=0 ; i<number_of_quaternions; ++i)
    {
        PyObject * value = PyList_GetItem(ret, i);
        animation->quats[i] = (float)PyFloat_AsDouble(value);
    }

    // decref position list
    Py_DECREF(ret);
    
    
    return true;
}


static void push_skeleton_in_python(const char* name, PyObject* instance, void* data)
{
    PbtSkeleton *skeleton = (PbtSkeleton*) data;
    assert(skeleton->bone_count != 0);
    bpt_python_fill_input_buffer_with_skeleton(skeleton, instance);
}


static void read_animation_from_python(const char* name, PyObject* instance, void* data)
{
    struct anim_skel_callback_data* _data = (struct anim_skel_callback_data*)data;
    assert(_data->skeleton->bone_count != 0);
    assert(_data->animation->frame_count == 0);

    bpt_python_fill_animation_struct_from_output_buffer(instance, _data->skeleton, _data->animation);
}