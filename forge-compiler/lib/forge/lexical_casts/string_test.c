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

#include <forge/lexical_casts/string.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

void test_format_string_empty() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_null() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\0");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_unprintable_ascii() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\x04");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\x04\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_printable_ascii() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "a");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"a\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_backslash() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\\");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\\\\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_single_quote() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "'");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"'\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_double_quote() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\"");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\\"\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_alert() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\a");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\a\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_backspace() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\b");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\b\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_ascii_escape() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\033");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\e\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_form_feed() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\f");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\f\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_newline() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\n");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\n\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_carriage_return() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\r");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\r\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_horizontal_tab() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\t");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\t\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_vertical_tab() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\v");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\v\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_unprintable_unicode() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\x97");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\u{97}\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_printable_unicode() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "😀");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"😀\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_multiple_characters_ascii() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "hello, world");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"hello, world\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_multiple_characters_unprintable_unicode() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "\x97\x98");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"\\u{97}\\u{98}\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_string_multiple_characters_printable_unicode() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_string(stream, "😀😘");

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("\"😀😘\"", formatted->str);
  g_string_free(formatted, TRUE);
}

void _test_parse_string(const char* expected, const char* formatted) {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  GString* actual                      = NULL;
  frg_parsing_location_t start
    = { .path = "--", .line_number = 1, .column_number = 1, .offset = 0 };

  frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, formatted);

  bool result = frg_parse_string(&actual, message_buffer, reader);

  frg_message_buffer_print(frg_stream_output_get_stderr(),
                           NULL,
                           message_buffer,
                           FRG_MESSAGE_SEVERITY_DEBUG,
                           0);

  TEST_ASSERT(result);

  frg_parsing_token_reader_destroy(reader);

  TEST_ASSERT_EQUAL_STRING(expected, actual->str);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  g_string_free(actual, TRUE);
  frg_message_buffer_destroy(message_buffer);
}

void test_parse_string_empty() {
  _test_parse_string("", "\"\"");
}

void test_parse_string_null() {
  _test_parse_string("", "\"\\0\"");
}

void test_parse_string_unprintable_ascii() {
  _test_parse_string("\x04", "\"\\x04\"");
}

void test_parse_string_printable_ascii() {
  _test_parse_string("a", "\"a\"");
}

void test_parse_string_backslash() {
  _test_parse_string("\\", "\"\\\\\"");
}

void test_parse_string_single_quote() {
  _test_parse_string("'", "\"'\"");
}

void test_parse_string_double_quote() {
  _test_parse_string("\"", "\"\\\"\"");
}

void test_parse_string_alert() {
  _test_parse_string("\a", "\"\\a\"");
}

void test_parse_string_backspace() {
  _test_parse_string("\b", "\"\\b\"");
}

void test_parse_string_ascii_escape() {
  _test_parse_string("\033", "\"\\e\"");
}

void test_parse_string_form_feed() {
  _test_parse_string("\f", "\"\\f\"");
}

void test_parse_string_newline() {
  _test_parse_string("\n", "\"\\n\"");
}

void test_parse_string_carriage_return() {
  _test_parse_string("\r", "\"\\r\"");
}

void test_parse_string_horizontal_tab() {
  _test_parse_string("\t", "\"\\t\"");
}

void test_parse_string_vertical_tab() {
  _test_parse_string("\v", "\"\\v\"");
}

void test_parse_string_unprintable_unicode() {
  _test_parse_string("\x97", "\"\\u{97}\"");
}

void test_parse_string_printable_unicode() {
  _test_parse_string("😀", "\"😀\"");
}

void test_parse_string_multiple_characters_ascii() {
  _test_parse_string("hello, world", "\"hello, world\"");
}

void test_parse_string_multiple_characters_unprintable_unicode() {
  _test_parse_string("\x97\x98", "\"\\u{97}\\u{98}\"");
}

void test_parse_string_multiple_characters_printable_unicode() {
  _test_parse_string("😀😘", "\"😀😘\"");
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_format_string_empty);
  RUN_TEST(test_format_string_null);
  RUN_TEST(test_format_string_unprintable_ascii);
  RUN_TEST(test_format_string_printable_ascii);
  RUN_TEST(test_format_string_backslash);
  RUN_TEST(test_format_string_single_quote);
  RUN_TEST(test_format_string_double_quote);
  RUN_TEST(test_format_string_alert);
  RUN_TEST(test_format_string_backspace);
  RUN_TEST(test_format_string_ascii_escape);
  RUN_TEST(test_format_string_form_feed);
  RUN_TEST(test_format_string_newline);
  RUN_TEST(test_format_string_carriage_return);
  RUN_TEST(test_format_string_horizontal_tab);
  RUN_TEST(test_format_string_vertical_tab);
  RUN_TEST(test_format_string_unprintable_unicode);
  RUN_TEST(test_format_string_printable_unicode);
  RUN_TEST(test_format_string_multiple_characters_ascii);
  RUN_TEST(test_format_string_multiple_characters_unprintable_unicode);
  RUN_TEST(test_format_string_multiple_characters_printable_unicode);
  RUN_TEST(test_parse_string_empty);
  RUN_TEST(test_parse_string_null);
  RUN_TEST(test_parse_string_unprintable_ascii);
  RUN_TEST(test_parse_string_printable_ascii);
  RUN_TEST(test_parse_string_backslash);
  RUN_TEST(test_parse_string_single_quote);
  RUN_TEST(test_parse_string_double_quote);
  RUN_TEST(test_parse_string_alert);
  RUN_TEST(test_parse_string_backspace);
  RUN_TEST(test_parse_string_ascii_escape);
  RUN_TEST(test_parse_string_form_feed);
  RUN_TEST(test_parse_string_newline);
  RUN_TEST(test_parse_string_carriage_return);
  RUN_TEST(test_parse_string_horizontal_tab);
  RUN_TEST(test_parse_string_vertical_tab);
  RUN_TEST(test_parse_string_unprintable_unicode);
  RUN_TEST(test_parse_string_printable_unicode);
  RUN_TEST(test_parse_string_multiple_characters_ascii);
  RUN_TEST(test_parse_string_multiple_characters_unprintable_unicode);
  RUN_TEST(test_parse_string_multiple_characters_printable_unicode);
  return UNITY_END();
}
