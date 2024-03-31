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

#include <forge/parsing/token_reader.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_simple() {
  frg_parsing_location_t start
    = { .path = "--", .offset = 0, .line_number = 1, .column_number = 1 };

  frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "abc");

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(0, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    1, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('a', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(1, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    2, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('b', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(2, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    3, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('c', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(3, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    4, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL(0, frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(3, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    4, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL(0, frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_destroy(reader);
}

void test_multiline() {
  frg_parsing_location_t start
    = { .path = "--", .offset = 0, .line_number = 1, .column_number = 1 };

  frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "abc\ndef");

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(0, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    1, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('a', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(1, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    2, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('b', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(2, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    3, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('c', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(3, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    4, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('\n', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(4, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(2,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    1, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('d', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(5, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(2,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    2, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('e', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(6, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(2,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    3, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('f', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(7, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(2,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    4, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL(0, frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_destroy(reader);
}

void test_save_and_restore() {
  frg_parsing_location_t start
    = { .path = "--", .offset = 0, .line_number = 1, .column_number = 1 };

  frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "abc");

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(0, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    1, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('a', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  frg_parsing_token_reader_t* state = frg_parsing_token_reader_clone(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(1, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    2, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('b', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_step(reader);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(2, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    3, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('c', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_restore(reader, state);

  TEST_ASSERT_EQUAL_STRING("--",
                           frg_parsing_token_reader_get_current_location(reader)->path);
  TEST_ASSERT_EQUAL(1, frg_parsing_token_reader_get_current_location(reader)->offset);
  TEST_ASSERT_EQUAL(1,
                    frg_parsing_token_reader_get_current_location(reader)->line_number);
  TEST_ASSERT_EQUAL(
    2, frg_parsing_token_reader_get_current_location(reader)->column_number);
  TEST_ASSERT_EQUAL('b', frg_parsing_token_reader_get_current_character(reader));

  frg_parsing_token_reader_destroy(reader);
  frg_parsing_token_reader_destroy(state);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_simple);
  RUN_TEST(test_multiline);
  RUN_TEST(test_save_and_restore);
  return UNITY_END();
}
