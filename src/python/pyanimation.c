#include "pyanimation.h"
#include "../log.h"
#include <malloc.h>

static void push_skeleton_in_python(const char* name, PyObject* instance, void* data);
static void read_animation_from_python(const char* name, PyObject* instance, void* data);



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
        pbt_python_foreach_instance(push_skeleton_in_python, pbt_python_env()->input_skeleton_buffer_class, (void*)skeleton);
        if(pbt_python_call_main())
        {
            animation = calloc(1, sizeof(PbtAnimation));
            pbt_python_foreach_instance(read_animation_from_python, pbt_python_env()->output_skeleton_buffer_class, (void*)animation);
        }
        pbt_python_end_script_execution();
    }

    return animation;
}


static void push_skeleton_in_python(const char* name, PyObject* instance, void* data)
{
    PbtSkeleton *skeleton = (PbtSkeleton*) data;
    assert(skeleton->bone_count == 0);
    bpt_python_fill_input_buffer_with_skeleton(skeleton, instance);
}


static void read_animation_from_python(const char* name, PyObject* instance, void* data)
{
    PbtAnimation *animation = (PbtAnimation*) data;
    assert(animation->frame_count == 0);
    //todo
}