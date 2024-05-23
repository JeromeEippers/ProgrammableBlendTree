#include "utest.h"

// declare all the tests
DECLARE_TEST(test_types);


int main(void) {
    // register all the tests
	test_fn_t tests[] = {
			test_types,
	};

    // run tests
	run_tests(tests, sizeof(tests)/sizeof(test_fn_t));
	return 0;
}