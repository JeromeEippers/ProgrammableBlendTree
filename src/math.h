#ifndef _INC_PBT_MATH_
#define _INC_PBT_MATH_

#include <stdbool.h>
#include <stdint.h>

#define PBT_EPSILON 1.e-6

typedef struct Pbtfloat4
{
    float values[4];
} Pbtfloat4;


Pbtfloat4 pbt_float4(float a, float b, float c, float d);
Pbtfloat4 pbt_float4_copy(Pbtfloat4 a);

bool pbt_float4_eq(Pbtfloat4 a, Pbtfloat4 b);

#endif