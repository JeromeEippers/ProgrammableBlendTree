#include "utest.h"
#include "../src/python/init.h"

// declare all the tests
DECLARE_TEST(test_types);


int main(void) {
    // register all the tests
	test_fn_t tests[] = {
			test_types,
	};

	init_python();

    // run tests
	run_tests(tests, sizeof(tests)/sizeof(test_fn_t));

	deinit_python();
	return 0;
}