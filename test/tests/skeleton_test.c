#include "../utest.h"
#include "../../src/skeleton.h"
#include <stdio.h>


DECLARE_TEST(pbttest_create_simple_skeleton) {
    BEGIN_TEST;

    FILE *fp = fopen("test/tests/simple_test_skeleton.py", "r");
    char text[8192];
    fread(text, 8192, 8192, fp);
    fclose(fp);

    PbtSkeleton * skeleton = pbt_create_skeleton_from_string(text);
    
    ASSERT_IEQ(8, pbt_skeleton_bone_count(skeleton), "wrong number of bones");

    // test names
    ASSERT_SEQ("Hips", pbt_skeleton_bone_name(skeleton, 0), "names should match");
    ASSERT_SEQ("LeftUpLeg", pbt_skeleton_bone_name(skeleton, 1), "names should match");
    ASSERT_SEQ("RightUpLeg", pbt_skeleton_bone_name(skeleton, 2), "names should match");
    ASSERT_SEQ("Spine", pbt_skeleton_bone_name(skeleton, 3), "names should match");
    ASSERT_SEQ("Chest", pbt_skeleton_bone_name(skeleton, 4), "names should match");
    ASSERT_SEQ("LeftArm", pbt_skeleton_bone_name(skeleton, 5), "names should match");
    ASSERT_SEQ("RightArm", pbt_skeleton_bone_name(skeleton, 6), "names should match");
    ASSERT_SEQ("Head", pbt_skeleton_bone_name(skeleton, 7), "names should match");

    // test parents
    ASSERT_IEQ(-1, pbt_skeleton_bone_parent(skeleton, 0), "wrong parent");
    ASSERT_SEQ("Hips", pbt_skeleton_bone_name(skeleton, pbt_skeleton_bone_parent(skeleton, 1)), "wrong parent");
    ASSERT_SEQ("Hips", pbt_skeleton_bone_name(skeleton, pbt_skeleton_bone_parent(skeleton, 2)), "wrong parent");
    ASSERT_SEQ("Hips", pbt_skeleton_bone_name(skeleton, pbt_skeleton_bone_parent(skeleton, 3)), "wrong parent");
    ASSERT_SEQ("Spine", pbt_skeleton_bone_name(skeleton, pbt_skeleton_bone_parent(skeleton, 4)), "wrong parent");
    ASSERT_SEQ("Chest", pbt_skeleton_bone_name(skeleton, pbt_skeleton_bone_parent(skeleton, 5)), "wrong parent");
    ASSERT_SEQ("Chest", pbt_skeleton_bone_name(skeleton, pbt_skeleton_bone_parent(skeleton, 6)), "wrong parent");
    ASSERT_SEQ("Chest", pbt_skeleton_bone_name(skeleton, pbt_skeleton_bone_parent(skeleton, 7)), "wrong parent");

    // test positions
    ASSERT_F4(pbt_float4(0,0,0,1), pbt_skeleton_bone_pos(skeleton, 0), "Hips at identity");
    ASSERT_F4(pbt_float4(10,2,3,1), pbt_skeleton_bone_pos(skeleton, 3), "Spine should be offset");

    // test rotations
    ASSERT_F4(pbt_float4(1,0,0,0), pbt_skeleton_bone_quat(skeleton, 0), "Hips at identity");
    ASSERT_F4(pbt_float4(-.707,0,.707,0), pbt_skeleton_bone_quat(skeleton, 1), "LeftUpLeg should be offset");
    ASSERT_F4(pbt_float4(.707,0,.707,0), pbt_skeleton_bone_quat(skeleton, 2), "LeftUpLeg should be offset");
    TEAR_DOWN;
    
    pbt_skeleton_delete(skeleton);

    END_TEST;
}

