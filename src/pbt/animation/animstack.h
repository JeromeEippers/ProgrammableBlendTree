#ifndef _INC_BPT_ANIMATIONSTACK_
#define _INC_BPT_ANIMATIONSTACK_

#include "../core/math.h"

typedef struct PbtAnimStack
{
    uint32_t    stack_size;
    uint32_t    bone_count;
    uint32_t    current_stack_id;
    float       *pos;
    float       *quats;
}PbtAnimStack;


PbtAnimStack* pbt_create_animstack(uint32_t stack_size, uint32_t bone_count);
void pbt_animstack_delete(PbtAnimStack* animstack);

uint32_t pbt_animstack_stack_count(PbtAnimStack* animstack);

void pbt_animstack_push(PbtAnimStack* animstack, const float* pos, const float* quats);
void pbt_animstack_pop(PbtAnimStack* animstack, float* pos, float* quats);
void pbt_animstack_peek(PbtAnimStack* animstack, int32_t stack_offset, float* pos, float* quats);

Pbtfloat4 pbt_animstack_peek_pos(PbtAnimStack* animstack, int32_t stack_offset, uint32_t bone_index);
Pbtfloat4 pbt_animstack_peek_quat(PbtAnimStack* animstack, int32_t stack_offset, uint32_t bone_index);

#endif