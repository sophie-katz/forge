#include <forge/common/stream.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_stdout(void) {
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_validate_console_stream(stdout));
}

void test_stderr(void) {
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_validate_console_stream(stderr));
}

void test_stdin(void) {
    TEST_ASSERT_EQUAL(FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE, frg_validate_console_stream(stdin));
}

void test_null(void) {
    TEST_ASSERT_EQUAL(FRG_STATUS_ERROR_NULL_ARGUMENT, frg_validate_console_stream(NULL));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_stdout);
    RUN_TEST(test_stderr);
    RUN_TEST(test_stdin);
    RUN_TEST(test_null);
    return UNITY_END();
}
