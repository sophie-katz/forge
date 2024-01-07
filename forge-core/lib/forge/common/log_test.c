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

#include <forge/common/log.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_debug_not_emitted(void) {
    frg_log_set_minimum_severity(FRG_LOG_SEVERITY_NOTE);
    frg_log_result_t result = frg_log_debug("hello, world");
    TEST_ASSERT_FALSE(result.emitted);
}

void test_debug_emitted(void) {
    frg_log_set_minimum_severity(FRG_LOG_SEVERITY_DEBUG);
    frg_log_result_t result = frg_log_debug("hello, world");
    TEST_ASSERT_TRUE(result.emitted);
}

void test_log_summary_if_errors(void) {
    TEST_ASSERT_FALSE(frg_log_summary_if_errors());
    frg_log_error("hello, world");
    TEST_ASSERT_TRUE(frg_log_summary_if_errors());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_debug_not_emitted);
    RUN_TEST(test_debug_emitted);
    RUN_TEST(test_log_summary_if_errors);
    return UNITY_END();
}
