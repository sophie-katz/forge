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

#include <forge/enums.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_get_enum_mask_from_max() {
  TEST_ASSERT_EQUAL(1, frg_get_enum_mask_from_max(1));
  TEST_ASSERT_EQUAL(3, frg_get_enum_mask_from_max(2));
  TEST_ASSERT_EQUAL(3, frg_get_enum_mask_from_max(3));
  TEST_ASSERT_EQUAL(7, frg_get_enum_mask_from_max(4));
  TEST_ASSERT_EQUAL(7, frg_get_enum_mask_from_max(7));
  TEST_ASSERT_EQUAL(15, frg_get_enum_mask_from_max(8));
  TEST_ASSERT_EQUAL(15, frg_get_enum_mask_from_max(15));
  TEST_ASSERT_EQUAL(31, frg_get_enum_mask_from_max(16));
  TEST_ASSERT_EQUAL(31, frg_get_enum_mask_from_max(31));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_get_enum_mask_from_max);
  return UNITY_END();
}
