#ifndef _INC_BPT_ANIMATION_
#define _INC_BPT_ANIMATION_

#include "math.h"

typedef struct PbtAnimation
{
    uint32_t    frame_count;
    float       *pos;
    float       *quats;
}PbtAnimation;


void pbt_animation_free(PbtAnimation* animation);
void pbt_animation_delete(PbtAnimation* animation);

uint32_t pbt_animation_frame_count(PbtAnimation* animation);

#endif