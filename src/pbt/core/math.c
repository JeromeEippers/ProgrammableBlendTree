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


bool pbt_float4_eq(Pbtfloat4 a, Pbtfloat4 b)
{
    return (
        a.values[0] - b.values[0] < PBT_EPSILON && 
        a.values[0] - b.values[0] > -PBT_EPSILON &&

        a.values[1] - b.values[1] < PBT_EPSILON && 
        a.values[1] - b.values[1] > -PBT_EPSILON &&

        a.values[2] - b.values[2] < PBT_EPSILON && 
        a.values[2] - b.values[2] > -PBT_EPSILON &&

        a.values[3] - b.values[3] < PBT_EPSILON && 
        a.values[3] - b.values[3] > -PBT_EPSILON
    );
}