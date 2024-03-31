// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#include <forge/lexical_casts/utilities.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_is_char_printable() {
  TEST_ASSERT(frg_is_char_printable('a'));
  TEST_ASSERT(frg_is_char_printable(' '));
  TEST_ASSERT_FALSE(frg_is_char_printable('\t'));
  TEST_ASSERT_FALSE(frg_is_char_printable('\033'));
  TEST_ASSERT_FALSE(frg_is_char_printable(0));
  TEST_ASSERT_FALSE(frg_is_char_printable(0x80));
  TEST_ASSERT(frg_is_char_printable(0xa3));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_is_char_printable);
  return UNITY_END();
}
