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

#include <forge-testing/string_substitution.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void _test_helper(const frg_testing_string_substituter_t* substituter,
                  const char* input,
                  const char* output) {
  GString* result = frg_testing_string_substituter_run(substituter, input);
  TEST_ASSERT_EQUAL_STRING(output, result->str);
  g_string_free(result, TRUE);
}

void test_all() {
  frg_testing_string_substituter_t* substituter = frg_testing_string_substituter_new();

  frg_testing_string_substituter_add_str(substituter, "x", "123");
  frg_testing_string_substituter_add_string(substituter, "asdf", g_string_new("abc"));
  frg_testing_string_substituter_add_str(substituter, "empty", "");
  frg_testing_string_substituter_add_printf(substituter, "number", "%i", 5);

  _test_helper(substituter, "", "");
  _test_helper(substituter, "asdf", "asdf");
  _test_helper(substituter, "%(x)", "123");
  _test_helper(substituter, "%%(x)", "%(x)");
  _test_helper(substituter, "%(x", "");
  _test_helper(substituter, "%(y)", "");
  _test_helper(substituter, "%(asdf)", "abc");
  _test_helper(substituter, "%(asdfe)", "");
  _test_helper(substituter, "a%(x)", "a123");
  _test_helper(substituter, "%(x)b", "123b");
  _test_helper(substituter, "a%(x)b", "a123b");
  _test_helper(substituter, "%(x)%(asdf)", "123abc");
  _test_helper(substituter, "%(empty)", "");
  _test_helper(substituter, "a%(empty)b", "ab");
  _test_helper(substituter, "%(", "");
  _test_helper(substituter, "%a", "a");
  _test_helper(substituter, "%%", "%");
  _test_helper(substituter, "a%(number)b", "a5b");

  frg_testing_string_substituter_destroy(substituter);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_all);
  return UNITY_END();
}
