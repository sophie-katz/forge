#include <forge/common/color.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_get_color_mode_smoke(void) {
    frg_color_mode_t mode;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_get_color_mode(&mode));
    TEST_ASSERT_TRUE(mode == FRG_COLOR_MODE_ENABLED || mode == FRG_COLOR_MODE_DISABLED);
}

void test_set_and_get(void) {
    frg_color_mode_t mode;
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_set_color_mode(FRG_COLOR_MODE_ENABLED));
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_get_color_mode(&mode));
    TEST_ASSERT_EQUAL(FRG_COLOR_MODE_ENABLED, mode);
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_set_color_mode(FRG_COLOR_MODE_DISABLED));
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_get_color_mode(&mode));
    TEST_ASSERT_EQUAL(FRG_COLOR_MODE_DISABLED, mode);
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_set_color_mode(FRG_COLOR_MODE_AUTO));
    TEST_ASSERT_EQUAL(FRG_STATUS_OK, frg_get_color_mode(&mode));
    TEST_ASSERT_TRUE(mode == FRG_COLOR_MODE_ENABLED || mode == FRG_COLOR_MODE_DISABLED);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_color_mode_smoke);
    RUN_TEST(test_set_and_get);
    return UNITY_END();
}
