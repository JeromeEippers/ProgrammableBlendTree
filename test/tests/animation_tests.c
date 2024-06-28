#include "../utest.h"
#include "../../src/pbt/python/pyskeleton.h"
#include "../../src/pbt/python/pyanimation.h"
#include "../../src/pbt/animation/animstack.h"
#include <stdio.h>


DECLARE_TEST(pbttest_create_bindpose_animation) {
    BEGIN_TEST;

    PbtSkeleton * skeleton = pbt_python_create_skeleton_from_file("test/tests/simple_test_skeleton.py");
    PbtAnimation * animation = pbt_python_create_animation_from_file("test/tests/bindpose_anim_test.py", skeleton);
    
    ASSERT_IEQ(1, animation->frame_count, "we should have one frame");

    PbtAnimStack * animstack = pbt_create_animstack(1, skeleton->bone_count);

    ASSERT_IEQ(0, pbt_animstack_stack_count(animstack), "we should have an empty stack");

    pbt_animation_push_on_stack(animstack, animation, 0);
    ASSERT_IEQ(1, pbt_animstack_stack_count(animstack), "we should have one pose on the stack");

    // test positions
    ASSERT_F4(pbt_float4(0,0,0,1), pbt_animstack_peek_pos(animstack, 1, 0), "Hips at identity");
    ASSERT_F4(pbt_float4(10,2,3,1), pbt_animstack_peek_pos(animstack, 1, 3), "Spine should be offset");

    // test rotations
    ASSERT_F4(pbt_float4(1,0,0,0), pbt_animstack_peek_quat(animstack, 1, 0), "Hips at identity");
    ASSERT_F4(pbt_float4(-.707,0,.707,0), pbt_animstack_peek_quat(animstack, 1, 1), "LeftUpLeg should be offset");
    ASSERT_F4(pbt_float4(.707,0,.707,0), pbt_animstack_peek_quat(animstack, 1, 2), "LeftUpLeg should be offset");

    pbt_animstack_pop(animstack, NULL, NULL);
    ASSERT_IEQ(0, pbt_animstack_stack_count(animstack), "we should have an empty stack");

    TEAR_DOWN

    pbt_animstack_delete(animstack);
    pbt_animation_delete(animation);
    pbt_skeleton_delete(skeleton);

    END_TEST;
}
