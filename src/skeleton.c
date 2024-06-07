#include <malloc.h>
#include "skeleton.h"
#include "python/python.h"
#include "python/pyskeleton.h"

// callback to call the python function
static void read_skeleton_from_python(const char* name, PyObject* instance, void* data)
{
    PbtSkeleton *skeleton = (PbtSkeleton*) data;
    assert(skeleton->bone_count == 0);
    bpt_python_fill_skeleton_struct_from_output_buffer(instance, skeleton);
}


PbtSkeleton * pbt_create_skeleton_from_file(const char* python_script_path)
{
    FILE *fp = fopen(python_script_path, "r");
    assert(fp != NULL);

    fseek(fp, 0 , SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0 , SEEK_SET);
    char *text = calloc(sizeof(char), fileSize+1);
    fread(text, sizeof(char), fileSize, fp);

    fclose(fp);

    PbtSkeleton * skeleton = pbt_create_skeleton_from_string(text);
    free(text);

    return skeleton;
}


PbtSkeleton * pbt_create_skeleton_from_string(const char* python_script)
{
    PbtSkeleton* skeleton = NULL;

    if (pbt_python_begin_script_execution(python_script))
    {
        if(pbt_python_call_main())
        {
            skeleton = (PbtSkeleton*)calloc(1, sizeof(PbtSkeleton));
            pbt_python_foreach_instance(read_skeleton_from_python, pbt_python_env()->output_skeleton_buffer_class, (void*)skeleton);
        }
        pbt_python_end_script_execution();
    }

    return skeleton;
}

void pbt_skeleton_free(PbtSkeleton* skeleton)
{
    if(skeleton->names) free(skeleton->names);
    if(skeleton->names_offset) free(skeleton->names_offset);
    if(skeleton->parents) free(skeleton->parents);
    if(skeleton->pos) free(skeleton->pos);
    if(skeleton->quats) free(skeleton->quats);

    skeleton->names = NULL;
    skeleton->names_offset = NULL;
    skeleton->parents = NULL;
    skeleton->pos = NULL;
    skeleton->quats = NULL;
    skeleton->bone_count = 0;
}

void pbt_skeleton_delete(PbtSkeleton* skeleton)
{
    pbt_skeleton_free(skeleton);
    free(skeleton);
}

uint32_t pbt_skeleton_bone_count(PbtSkeleton* skeleton)
{
    return skeleton->bone_count;
}

const char* pbt_skeleton_bone_name(PbtSkeleton* skeleton, uint32_t index)
{
    return (char*)(skeleton->names + skeleton->names_offset[index]);
}

int32_t pbt_skeleton_bone_parent(PbtSkeleton* skeleton, uint32_t index)
{
    return *(skeleton->parents + index);
}

Pbtfloat4 pbt_skeleton_bone_pos(PbtSkeleton* skeleton, uint32_t index)
{
    Pbtfloat4 pos;
    float* ptr = skeleton->pos + index*3;
    pos.values[0] = *(ptr++);
    pos.values[1] = *(ptr++);
    pos.values[2] = *(ptr);
    pos.values[3] = 1.0f;
    return pos;
}

Pbtfloat4 pbt_skeleton_bone_quat(PbtSkeleton* skeleton, uint32_t index)
{
    Pbtfloat4 quats;
    float* ptr = skeleton->quats + index*4;
    quats.values[0] = *(ptr++);
    quats.values[1] = *(ptr++);
    quats.values[2] = *(ptr++);
    quats.values[3] = *(ptr);
    return quats;
}