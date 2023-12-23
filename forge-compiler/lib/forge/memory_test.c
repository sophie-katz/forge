#include <unity.h>
#include <forge/memory.h>

void setUp(void) {}

void tearDown(void) {}

void test_1_byte(void) {
    void* ptr = NULL;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_safe_malloc(&ptr, 1));
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_safe_free(&ptr));
    TEST_ASSERT_NULL(ptr);
}

void test_0_bytes(void) {
    void* ptr = NULL;
    TEST_ASSERT_EQUAL(FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE, frg_safe_malloc(&ptr, 0));
}

void test_malloc_null_ptr(void) {
    TEST_ASSERT_EQUAL(FRG_STATUS_ERROR_NULL_ARGUMENT, frg_safe_malloc(NULL, 1));
}

void test_free_null_ptr(void) {
    TEST_ASSERT_EQUAL(FRG_STATUS_ERROR_NULL_ARGUMENT, frg_safe_free(NULL));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_1_byte);
    RUN_TEST(test_0_bytes);
    RUN_TEST(test_malloc_null_ptr);
    RUN_TEST(test_free_null_ptr);
    return UNITY_END();
}
