#include "../utest.h"
#include "../../src/skeleton.h"
#include <stdio.h>
#include <malloc.h>


DECLARE_TEST(pbttest_load_default_skeleton) {
    BEGIN_TEST;

    FILE *fp = fopen("data/character/AnimLabSkeleton.py", "r");
    ASSERT_TRUE(fp != NULL, "could not read file");
    char text[8192]; memset(text, 0, 8192);
    fread(text, sizeof(char), 8192, fp);
    fclose(fp);

    PbtSkeleton * skeleton = pbt_create_skeleton_from_string(text);

    ASSERT_IEQ(23, pbt_skeleton_bone_count(skeleton), "wrong number of bones");

    TEAR_DOWN;
    
    pbt_skeleton_delete(skeleton);

    END_TEST;
}

