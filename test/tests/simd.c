#include "../utest.h"
#include <xmmintrin.h>

// quick tests to see the generate assembly code for smid



DECLARE_TEST(pbttest_simd_basic) {
    BEGIN_TEST;

    float values[8] = {1.0f, 2.0f, 3.0f, 4.0f,5.0f, 6.0f, 7.0f, 8.0f};

    __m128 a = _mm_load_ps(values);
    __m128 b = _mm_load_ps(values+4);
    __m128 result = _mm_mul_ps(a, b);
    _mm_store_ps(values, result);

    ASSERT_FEQ(5.f, values[0], "1 * 5");
    ASSERT_FEQ(12.f, values[1], "2 * 6");
    

    TEAR_DOWN


    END_TEST;
}
