// Copyright (c) 2023 Sophie Katz
//
// This file is part of Forge.
//
// Forge is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with Forge.
// If not, see <https://www.gnu.org/licenses/>.

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
