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

#include <forge/common/memory.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_1_byte(void) {
    void* ptr = frg_safe_malloc(1);
    TEST_ASSERT_NOT_NULL(ptr);
    frg_safe_free(&ptr);
    TEST_ASSERT_NULL(ptr);
}

void test_10_bytes(void) {
    void* ptr = frg_safe_malloc(10);
    TEST_ASSERT_NOT_NULL(ptr);
    frg_safe_free(&ptr);
    TEST_ASSERT_NULL(ptr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_1_byte);
    RUN_TEST(test_10_bytes);
    return UNITY_END();
}
