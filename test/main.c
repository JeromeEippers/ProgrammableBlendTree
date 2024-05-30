#include "utest.h"
#include "../src/python/python.h"

// declare all the tests
DECLARE_TEST(pbttest_types);
DECLARE_TEST(pbttest_create_simple_skeleton);

int main(void) {
    // register all the tests
	test_fn_t tests[] = {
			pbttest_types,
			pbttest_create_simple_skeleton,
	};

	pbt_init_python();

    // run tests
	run_tests(tests, sizeof(tests)/sizeof(test_fn_t));

	pbt_deinit_python();
	return 0;
}