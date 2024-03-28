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

#include <forge/lexical_casts/character.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

void test_format_char_null() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, 0);
  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\0'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_unprintable_ascii() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, 0x04);

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\x04'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_printable_ascii() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, 'a');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'a'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_backslash() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\\');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\\\'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_single_quote() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\'');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\''", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_double_quote() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '"');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\"'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_alert() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\a');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\a'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_backspace() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\b');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\b'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_ascii_escape() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\033');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\e'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_form_feed() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\f');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\f'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_newline() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\n');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\n'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_carriage_return() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\r');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\r'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_horizontal_tab() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\t');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\t'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_vertical_tab() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, '\v');

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\v'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_unprintable_unicode() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, 0x97);

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'\\u{97}'", formatted->str);
  g_string_free(formatted, TRUE);
}

void test_format_char_printable_unicode() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
  frg_print_character(stream, 0x1f600);

  GString* formatted = frg_stream_output_destroy_take_buffer(stream);
  TEST_ASSERT_EQUAL_STRING("'😀'", formatted->str);
  g_string_free(formatted, TRUE);
}

void _test_parse_character(frg_character_t expected, const char* formatted) {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_character_t actual               = 0;
  frg_parsing_location_t start
    = { .path = "--", .line_number = 1, .column_number = 1, .offset = 0 };

  frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, formatted);

  bool result = frg_parse_character(&actual, message_buffer, reader);

  frg_message_buffer_print(frg_stream_output_get_stderr(),
                           NULL,
                           message_buffer,
                           FRG_MESSAGE_SEVERITY_DEBUG,
                           0);

  TEST_ASSERT(result);

  frg_parsing_token_reader_destroy(reader);

  TEST_ASSERT_EQUAL(expected, actual);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
}

void test_parse_char_null() {
  _test_parse_character(0, "'\\0'");
}

void test_parse_char_unprintable_ascii() {
  _test_parse_character('\x04', "'\\x04'");
}

void test_parse_char_printable_ascii() {
  _test_parse_character('a', "'a'");
}

void test_parse_char_backslash() {
  _test_parse_character('\\', "'\\\\'");
}

void test_parse_char_single_quote() {
  _test_parse_character('\'', "'\\''");
}

void test_parse_char_double_quote() {
  _test_parse_character('"', "'\"'");
}

void test_parse_char_alert() {
  _test_parse_character('\a', "'\\a'");
}

void test_parse_char_backspace() {
  _test_parse_character('\b', "'\\b'");
}

void test_parse_char_ascii_escape() {
  _test_parse_character('\033', "'\\e'");
}

void test_parse_char_form_feed() {
  _test_parse_character('\f', "'\\f'");
}

void test_parse_char_newline() {
  _test_parse_character('\n', "'\\n'");
}

void test_parse_char_carriage_return() {
  _test_parse_character('\r', "'\\r'");
}

void test_parse_char_horizontal_tab() {
  _test_parse_character('\t', "'\\t'");
}

void test_parse_char_vertical_tab() {
  _test_parse_character('\v', "'\\v'");
}

void test_parse_char_unprintable_unicode() {
  _test_parse_character(0x97, "'\\u{97}'");
}

void test_parse_char_printable_unicode() {
  _test_parse_character(0x1f600, "'😀'");
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_format_char_null);
  RUN_TEST(test_format_char_unprintable_ascii);
  RUN_TEST(test_format_char_printable_ascii);
  RUN_TEST(test_format_char_backslash);
  RUN_TEST(test_format_char_single_quote);
  RUN_TEST(test_format_char_double_quote);
  RUN_TEST(test_format_char_alert);
  RUN_TEST(test_format_char_backspace);
  RUN_TEST(test_format_char_ascii_escape);
  RUN_TEST(test_format_char_form_feed);
  RUN_TEST(test_format_char_newline);
  RUN_TEST(test_format_char_carriage_return);
  RUN_TEST(test_format_char_horizontal_tab);
  RUN_TEST(test_format_char_vertical_tab);
  RUN_TEST(test_format_char_unprintable_unicode);
  RUN_TEST(test_format_char_printable_unicode);
  RUN_TEST(test_parse_char_null);
  RUN_TEST(test_parse_char_unprintable_ascii);
  RUN_TEST(test_parse_char_printable_ascii);
  RUN_TEST(test_parse_char_backslash);
  RUN_TEST(test_parse_char_single_quote);
  RUN_TEST(test_parse_char_double_quote);
  RUN_TEST(test_parse_char_alert);
  RUN_TEST(test_parse_char_backspace);
  RUN_TEST(test_parse_char_ascii_escape);
  RUN_TEST(test_parse_char_form_feed);
  RUN_TEST(test_parse_char_newline);
  RUN_TEST(test_parse_char_carriage_return);
  RUN_TEST(test_parse_char_horizontal_tab);
  RUN_TEST(test_parse_char_vertical_tab);
  RUN_TEST(test_parse_char_unprintable_unicode);
  RUN_TEST(test_parse_char_printable_unicode);
  return UNITY_END();
}
