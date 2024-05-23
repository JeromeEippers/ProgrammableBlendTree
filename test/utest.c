#include "utest.h"

void run_tests(test_fn_t* tests, int test_count) {
    fprintf(stdout, "\nRunning %d Tests ::\n\n", test_count);
    int curr_err_count = 0;
    for (int i = 0; i < test_count; ++i) {
        test_return_t output = tests[i]();
        if (output.success) {
            fprintf(stdout, "...\x1B[32m OK \x1B[0m\n");
        }
        else {
            fprintf(stdout, "...\x1B[31m NOK \x1B[0m\n");
            fprintf(stdout, "%s", output.error_messages);
            curr_err_count++;
        }
    }

    fprintf(stdout, "\nRan %d tests: %d failed\n\n", test_count, curr_err_count);
}

struct _mock_struct_size{
    void * ptr;
    uint8_t b;
    uint16_t a;
};

/* simple test example */
DECLARE_TEST(test_types) {
    BEGIN_TEST;
    ASSERT_IEQ(1, (uint32_t)sizeof(int8_t), "int8 is 1bits");
    ASSERT_IEQ(2, (uint32_t)sizeof(int16_t), "int16 is 2bits");
    ASSERT_IEQ(4, (uint32_t)sizeof(int32_t), "int32 is 4bits");
    ASSERT_IEQ(8, (uint32_t)sizeof(int64_t), "int32 is 8bits");
    ASSERT_IEQ(4, (uint32_t)sizeof(float), "float is 4bits");
    ASSERT_IEQ(8, (uint32_t)sizeof(double), "double is 8bits");
    ASSERT_IEQ(16, (uint32_t)sizeof(struct _mock_struct_size), "struct is aligned on 16bits");
    END_TEST;
}
