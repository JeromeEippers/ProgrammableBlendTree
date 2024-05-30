#ifndef _INC_BPT_SKELETON_
#define _INC_BPT_SKELETON_

#include "math.h"

typedef struct PbtSkeleton
{
    int     bone_count;
    char    *names;
    int     *names_offset;
    int     *parents;
    float   *pos;
    float   *quats;
} PbtSkeleton;

PbtSkeleton * pbt_create_skeleton_from_string(const char* python_script);
void pbt_skeleton_delete(PbtSkeleton* skeleton);

int pbt_skeleton_bone_count(PbtSkeleton* skeleton);
const char* pbt_skeleton_bone_name(PbtSkeleton* skeleton, int index);
int pbt_skeleton_bone_parent(PbtSkeleton* skeleton, int index);

Pbtfloat4 pbt_skeleton_bone_pos(PbtSkeleton* skeleton, int index);
Pbtfloat4 pbt_skeleton_bone_quat(PbtSkeleton* skeleton, int index);

#endif