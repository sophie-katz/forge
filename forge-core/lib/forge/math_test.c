// Copyright (c) 2023-2024 Sophie Katz
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

#include <forge/math.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_get_uint32_digit_count() {
    TEST_ASSERT_EQUAL(1, frg_get_uint32_digit_count(0));
    TEST_ASSERT_EQUAL(1, frg_get_uint32_digit_count(1));
    TEST_ASSERT_EQUAL(1, frg_get_uint32_digit_count(9));
    TEST_ASSERT_EQUAL(2, frg_get_uint32_digit_count(10));
    TEST_ASSERT_EQUAL(2, frg_get_uint32_digit_count(99));
    TEST_ASSERT_EQUAL(3, frg_get_uint32_digit_count(100));
    TEST_ASSERT_EQUAL(3, frg_get_uint32_digit_count(999));
    TEST_ASSERT_EQUAL(4, frg_get_uint32_digit_count(1000));
    TEST_ASSERT_EQUAL(10, frg_get_uint32_digit_count(UINT32_MAX));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_get_uint32_digit_count);
    return UNITY_END();
}
