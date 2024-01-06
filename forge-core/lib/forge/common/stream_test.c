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

#include <forge/common/stream.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_stdout(void) {
    TEST_ASSERT_TRUE(frg_is_console_stream(stdout));
}

void test_stderr(void) {
    TEST_ASSERT_TRUE(frg_is_console_stream(stderr));
}

void test_stdin(void) {
    TEST_ASSERT_FALSE(frg_is_console_stream(stdin));
}

void test_null(void) {
    TEST_ASSERT_FALSE(frg_is_console_stream(NULL));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_stdout);
    RUN_TEST(test_stderr);
    RUN_TEST(test_stdin);
    RUN_TEST(test_null);
    return UNITY_END();
}
