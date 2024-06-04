#ifndef _INC_BPT_UTEST_
#define _INC_BPT_UTEST_

#include <stdint.h>
#include <stdio.h>
#include <memory.h>


typedef struct test_return {
	char error_messages[8192];
	uint8_t success;
}test_return_t;

typedef test_return_t(*test_fn_t)(void);
void run_tests(test_fn_t* tests, int test_count);

#define DECLARE_TEST(NAME) test_return_t NAME(void)

/* begin test */
#define BEGIN_TEST  \
	test_return_t _return_value_;\
	_return_value_.success = 1;\
	memset(_return_value_.error_messages, 0, 8192);\
	uint32_t _error_stream_cnt_ = 0;\
	int _error_stream_cnt_inc_ = 0;\
	fprintf(stdout, "Testing %s ", __func__ + 8);\

#define TEAR_DOWN _tear_down_jump:

/* end test */
#define END_TEST  \
	return _return_value_; 

/* asserts*/
#define ASSERT_TRUE(VALUE, MESSAGE){\
	if ((VALUE) == 0) {\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "  line %d: %s... Failed\n", __LINE__, MESSAGE);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_return_value_.success = 0;\
		goto _tear_down_jump;\
	}\
}

#define ASSERT_IEQ(EXPECTED, ACTUAL, MESSAGE){\
	if (EXPECTED != ACTUAL) {\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "  line %d: %s... Failed\n", __LINE__, MESSAGE);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    expected: %d\n", EXPECTED);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    actual: %d\n", ACTUAL);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_return_value_.success = 0;\
		goto _tear_down_jump;\
	}\
}


#define ASSERT_I64EQ(EXPECTED, ACTUAL, MESSAGE){\
	if (EXPECTED != ACTUAL) {\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "  line %d: %s... Failed\n", __LINE__, MESSAGE);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    expected: %lld\n", EXPECTED);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    actual: %lld\n", ACTUAL);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_return_value_.success = 0;\
		goto _tear_down_jump;\
	}\
}

#define ASSERT_FEQ(EXPECTED, ACTUAL, MESSAGE){\
	if (EXPECTED != ACTUAL) {\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "  line %d: %s... Failed\n", __LINE__, MESSAGE);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    expected: %f\n", EXPECTED);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    actual: %f\n", ACTUAL);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_return_value_.success = 0;\
		goto _tear_down_jump;\
	}\
}

#define ASSERT_SEQ(EXPECTED, ACTUAL, MESSAGE){\
	if (strcmp(EXPECTED, ACTUAL) != 0) {\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "  line %d: %s... Failed\n", __LINE__, MESSAGE);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    expected: %s\n", EXPECTED);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    actual: %s\n", ACTUAL);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_return_value_.success = 0;\
		goto _tear_down_jump;\
	}\
}

#define ASSERT_F4(EXPECTED, ACTUAL, MESSAGE){\
	Pbtfloat4 _a_ = EXPECTED; \
	Pbtfloat4 _b_ = ACTUAL; \
	if (pbt_float4_eq(_a_, _b_) == false) {\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "  line %d: %s... Failed\n", __LINE__, MESSAGE);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    expected: %f,%f,%f,%f\n", _a_.values[0], _a_.values[1], _a_.values[2], _a_.values[3]);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_error_stream_cnt_inc_ = sprintf(_return_value_.error_messages + _error_stream_cnt_, "    actual: %f,%f,%f,%f\n", _b_.values[0], _b_.values[1], _b_.values[2], _b_.values[3]);\
		_error_stream_cnt_ += (_error_stream_cnt_inc_ > 0) ? _error_stream_cnt_inc_ : 0;\
		_return_value_.success = 0;\
		goto _tear_down_jump;\
	}\
}

#endif