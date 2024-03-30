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

#include <forge/memory.h>
#include <forge/streams/output.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_stream(frg_stream_output_t* mut_stream) {
  frg_stream_output_write_character(mut_stream, 'a');
  frg_stream_output_write_string(mut_stream, "b");
  frg_stream_output_write_string_with_length(mut_stream, "cd", 1);
  frg_stream_output_write_printf(mut_stream, "%i", 1);
}

void test_stream_buffer() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  test_stream(stream);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("abc1", buffer->str);

  g_string_free(buffer, TRUE);
}

void test_stream_file() {
  char* buffer = frg_malloc(1024);
  memset(buffer, 0, 1024);

  FILE* file = fmemopen(buffer, 1024, "w+");

  frg_stream_output_t* stream
    = frg_stream_output_new_file(file, FRG_STREAM_OUTPUT_FLAG_NONE);

  test_stream(stream);

  frg_stream_output_destroy(stream);

  fclose(file);

  TEST_ASSERT_EQUAL_STRING("abc1", buffer);

  frg_free(buffer);
}

void test_color_enabled() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_COLOR);

  frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_BLUE);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_GREATER_THAN(0, buffer->len);

  g_string_free(buffer, TRUE);
}

void test_color_disabled() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_BLUE);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL(0, buffer->len);

  g_string_free(buffer, TRUE);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_stream_buffer);
  RUN_TEST(test_stream_file);
  RUN_TEST(test_color_enabled);
  RUN_TEST(test_color_disabled);
  return UNITY_END();
}
