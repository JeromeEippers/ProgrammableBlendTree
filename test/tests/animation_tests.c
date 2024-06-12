#include "../utest.h"
#include "../../src/python/pyskeleton.h"
#include "../../src/python/pyanimation.h"
#include <stdio.h>


DECLARE_TEST(pbttest_create_bindpose_animation) {
    BEGIN_TEST;

    PbtSkeleton * skeleton = pbt_python_create_skeleton_from_file("test/tests/simple_test_skeleton.py");
    PbtAnimation * animation = pbt_python_create_animation_from_file("test/tests/bindpose_anim_test.py", skeleton);
    
    ASSERT_IEQ(1, animation->frame_count, "we should have one frame");

    TEAR_DOWN

    pbt_animation_delete(animation);
    pbt_skeleton_delete(skeleton);

    END_TEST;
}
