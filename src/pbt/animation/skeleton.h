#ifndef _INC_BPT_SKELETON_
#define _INC_BPT_SKELETON_

#include "../core/math.h"

typedef struct PbtSkeletonStruct
{
    uint32_t     bone_count;
    char        *names;
    uint32_t     *names_offset;
    int32_t     *parents;
    float       *pos;
    float       *quats;
} PbtSkeleton;


void pbt_skeleton_free(PbtSkeleton* skeleton);
void pbt_skeleton_delete(PbtSkeleton* skeleton);

uint32_t pbt_skeleton_bone_count(PbtSkeleton* skeleton);
const char* pbt_skeleton_bone_name(PbtSkeleton* skeleton, uint32_t index);
int32_t pbt_skeleton_bone_parent(PbtSkeleton* skeleton, uint32_t index);

Pbtfloat4 pbt_skeleton_bone_pos(PbtSkeleton* skeleton, uint32_t index);
Pbtfloat4 pbt_skeleton_bone_quat(PbtSkeleton* skeleton, uint32_t index);

#endif