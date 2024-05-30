#include "math.h"

Pbtfloat4 pbt_float4(float a, float b, float c, float d)
{
    Pbtfloat4 value;
    value.values[0] = a;
    value.values[1] = b;
    value.values[2] = c;
    value.values[3] = d;
    return value;
}