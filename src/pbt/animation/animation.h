#ifndef _INC_BPT_ANIMATION_
#define _INC_BPT_ANIMATION_

#include "../core/math.h"
#include "animstack.h"

typedef struct PbtAnimation
{
    uint32_t    frame_count;
    float       *pos;
    float       *quats;
}PbtAnimation;


void pbt_animation_free(PbtAnimation* animation);
void pbt_animation_delete(PbtAnimation* animation);

uint32_t pbt_animation_frame_count(PbtAnimation* animation);

void pbt_animation_push_on_stack(PbtAnimStack* stack, PbtAnimation* animation, uint32_t frame);

#endif