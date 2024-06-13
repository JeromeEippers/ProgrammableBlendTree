#include "animstack.h"
#include <memory.h>
#include <malloc.h>
#include <assert.h>

PbtAnimStack* pbt_create_animstack(uint32_t stack_size, uint32_t bone_count)
{
    assert(bone_count>0);
    assert(stack_size>0);

    PbtAnimStack* stack = calloc(sizeof(PbtAnimStack), 1);
    stack->pos = malloc(sizeof(float)*stack_size*bone_count*3);
    stack->quats = malloc(sizeof(float)*stack_size*bone_count*4);
    stack->bone_count = bone_count;
    stack->stack_size = stack_size;

    return stack;
}

void pbt_animstack_delete(PbtAnimStack* animstack)
{
    if(animstack->pos != NULL) free(animstack->pos);
    if(animstack->quats != NULL) free(animstack->quats);

    animstack->bone_count = 0;
    animstack->stack_size = 0;
    animstack->pos = NULL;
    animstack->quats = NULL;
}

uint32_t pbt_animstack_stack_count(PbtAnimStack* animstack)
{
    return animstack->current_stack_id;
}

void pbt_animstack_push(PbtAnimStack* animstack, const float* pos, const float* quats)
{
    assert(animstack->current_stack_id < animstack->stack_size);
    float* ptr;

    ptr = animstack->pos + animstack->current_stack_id * animstack->bone_count * 3;
    memcpy(ptr, pos, sizeof(float) * animstack->bone_count * 3);

    ptr = animstack->quats + animstack->current_stack_id * animstack->bone_count * 4;
    memcpy(ptr, quats, sizeof(float) * animstack->bone_count * 4);

    animstack->current_stack_id++;
}

void pbt_animstack_pop(PbtAnimStack* animstack, float* pos, float* quats)
{
    assert(animstack->current_stack_id > 0);
    animstack->current_stack_id--;
    float* ptr;

    if (pos != NULL)
    {
        ptr = animstack->pos + animstack->current_stack_id * animstack->bone_count * 3;
        memcpy(pos, ptr, sizeof(float) * animstack->bone_count * 3);
    }

    if(quats != NULL)
    {
        ptr = animstack->quats + animstack->current_stack_id * animstack->bone_count * 4;
        memcpy(quats, ptr, sizeof(float) * animstack->bone_count * 4);
    }
}
    

void pbt_animstack_peek(PbtAnimStack* animstack, int32_t stack_offset, float* pos, float* quats)
{
    int stack_id = animstack->current_stack_id - stack_offset;
    assert(stack_id >= 0);
    assert(stack_id < animstack->stack_size);

    float* ptr;

    if (pos != NULL)
    {
        ptr = animstack->pos + stack_id * animstack->bone_count * 3;
        memcpy(pos, ptr, sizeof(float) * animstack->bone_count * 3);
    }

    if(quats != NULL)
    {
        ptr = animstack->quats + stack_id * animstack->bone_count * 4;
        memcpy(quats, ptr, sizeof(float) * animstack->bone_count * 4);
    }
}

Pbtfloat4 pbt_animstack_peek_pos(PbtAnimStack* animstack, int32_t stack_offset, uint32_t bone_index)
{
    int stack_id = animstack->current_stack_id - stack_offset;
    assert(stack_id >= 0);
    assert(stack_id < animstack->stack_size);

    Pbtfloat4 pos;
    float* ptr = animstack->pos + stack_id * animstack->bone_count * 3 + bone_index * 3;
    pos.values[0] = *(ptr++);
    pos.values[1] = *(ptr++);
    pos.values[2] = *(ptr);
    pos.values[3] = 1.0f;
    return pos;
}

Pbtfloat4 pbt_animstack_peek_quat(PbtAnimStack* animstack, int32_t stack_offset, uint32_t bone_index)
{
    int stack_id = animstack->current_stack_id - stack_offset;
    assert(stack_id >= 0);
    assert(stack_id < animstack->stack_size);

    Pbtfloat4 quats;
    float* ptr = animstack->quats + stack_id * animstack->bone_count * 4 + bone_index * 4;
    quats.values[0] = *(ptr++);
    quats.values[1] = *(ptr++);
    quats.values[2] = *(ptr++);
    quats.values[3] = *(ptr);
    return quats;
}