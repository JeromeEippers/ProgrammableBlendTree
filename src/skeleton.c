#include <malloc.h>
#include "skeleton.h"


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