#include "animation.h"
#include <malloc.h>



void pbt_animation_free(PbtAnimation* animation)
{
    if(animation->pos != NULL) free(animation->pos);
    if(animation->quats != NULL) free(animation->quats);

    animation->frame_count = 0;
    animation->pos = NULL;
    animation->quats = NULL;
}

void pbt_animation_delete(PbtAnimation* animation)
{ 
    pbt_animation_free(animation);
    free(animation);
}

uint32_t pbt_animation_frame_count(PbtAnimation* animation)
{
    return animation->frame_count;
}

void pbt_animation_push_on_stack(PbtAnimStack* stack, PbtAnimation* animation, uint32_t frame)
{
    float *pos = animation->pos + stack->bone_count * frame * 3;
    float *quats = animation->quats + stack->bone_count * frame * 4;
    pbt_animstack_push(stack, pos, quats);
}