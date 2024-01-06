// Copyright (c) 2024 Sophie Katz
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
    TEST_ASSERT_TRUE(frg_color_mode_get() == FRG_COLOR_MODE_ENABLED || frg_color_mode_get() == FRG_COLOR_MODE_DISABLED);
}

void test_set_and_get(void) {
    TEST_ASSERT_EQUAL(FRG_COLOR_MODE_ENABLED, frg_color_mode_set(FRG_COLOR_MODE_ENABLED));
    TEST_ASSERT_EQUAL(FRG_COLOR_MODE_ENABLED, frg_color_mode_get());
    TEST_ASSERT_EQUAL(FRG_COLOR_MODE_DISABLED, frg_color_mode_set(FRG_COLOR_MODE_DISABLED));
    TEST_ASSERT_EQUAL(FRG_COLOR_MODE_DISABLED, frg_color_mode_get());
    frg_color_mode_set(FRG_COLOR_MODE_AUTO);
    TEST_ASSERT_TRUE(frg_color_mode_get() == FRG_COLOR_MODE_ENABLED || frg_color_mode_get() == FRG_COLOR_MODE_DISABLED);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_color_mode_smoke);
    RUN_TEST(test_set_and_get);
    return UNITY_END();
}
