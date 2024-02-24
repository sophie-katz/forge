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

#include <forge/lexical_casts.h>
#include <unity.h>

void setUp() {
    frg_stream_output_init();
}

void tearDown() {
    frg_stream_output_cleanup();
}

void test_format_char_null() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, 0);
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\0'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_unprintable_ascii() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, 0x04);
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\x04'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_printable_ascii() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, 'a');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'a'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_backslash() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\\');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\\\'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_single_quote() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\'');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\''", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_double_quote() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '"');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\"'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_alert() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\a');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\a'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_backspace() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\b');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\b'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_ascii_escape() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\033');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\e'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_form_feed() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\f');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\f'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_newline() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\n');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\n'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_carriage_return() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\r');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\r'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_horizontal_tab() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\t');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\t'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_vertical_tab() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, '\v');
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\v'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_unprintable_unicode() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, 0x97);
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'\\u{97}'", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_char_printable_unicode() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_character(stream, 0x1f600);
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("'😀'", formatted->str);
    g_string_free(formatted, TRUE);
}

void _test_parse_character(frg_character_t expected, const char* formatted) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_character_t actual = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .line_number = 1,
        .column_number = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &start,
        formatted
    );

    bool result = frg_parse_character(&actual, message_buffer, reader);

    frg_message_buffer_print(
        frg_stream_output_get_stderr(),
        NULL,
        message_buffer,
        FRG_MESSAGE_SEVERITY_DEBUG,
        0
    );

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

void test_format_string_empty() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_null() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\0");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_unprintable_ascii() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\x04");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\x04\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_printable_ascii() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "a");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"a\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_backslash() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\\");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\\\\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_single_quote() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "'");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"'\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_double_quote() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\"");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\\"\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_alert() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\a");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\a\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_backspace() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\b");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\b\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_ascii_escape() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\033");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\e\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_form_feed() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\f");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\f\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_newline() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\n");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\n\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_carriage_return() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\r");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\r\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_horizontal_tab() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\t");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\t\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_vertical_tab() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\v");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\v\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_unprintable_unicode() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\x97");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\u{97}\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_printable_unicode() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "😀");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"😀\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_multiple_characters_ascii() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "hello, world");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"hello, world\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_multiple_characters_unprintable_unicode() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "\x97\x98");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"\\u{97}\\u{98}\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void test_format_string_multiple_characters_printable_unicode() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_string(stream, "😀😘");
    
    GString* formatted = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("\"😀😘\"", formatted->str);
    g_string_free(formatted, TRUE);
}

void _test_parse_string(const char* expected, const char* formatted) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* actual = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .line_number = 1,
        .column_number = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &start,
        formatted
    );

    bool result = frg_parse_string(&actual, message_buffer, reader);

    frg_message_buffer_print(
        frg_stream_output_get_stderr(),
        NULL,
        message_buffer,
        FRG_MESSAGE_SEVERITY_DEBUG,
        0
    );

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

void test_format_uint_0_base2() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0, 2);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0b0", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_1_base2() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 1, 2);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0b1", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_10_base2() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 2, 2);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0b10", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_11_base2() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 3, 2);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0b11", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_100_base2() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 4, 2);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0b100", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_111111111_base2() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0xff, 2);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0b11111111", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_0_base8() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0, 8);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0o0", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_7_base8() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 7, 8);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0o7", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_10_base8() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 010, 8);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0o10", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_77_base8() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 077, 8);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0o77", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_100_base8() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0100, 8);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0o100", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_7777777_base8() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 07777777, 8);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0o7777777", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_0_base10() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0, 10);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_9_base10() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 9, 10);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("9", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_10_base10() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 10, 10);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("10", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_99_base10() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 99, 10);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("99", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_100_base10() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 100, 10);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("100", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_123456789_base10() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 123456789, 10);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("123456789", str->str);
    g_string_free(str, TRUE);
}








void test_format_uint_0_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0x0, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0x0", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_9_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0x9, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0x9", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_a_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0xa, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0xa", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_f_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0xf, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0xf", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_10_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0x10, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0x10", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_99_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0x99, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0x99", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_aa_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0xaa, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0xaa", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_ff_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0xff, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0xff", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_100_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0x100, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0x100", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_ffffffff_base16() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint(stream, 0xffffffff, 16);
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0xffffffff", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_with_suffix_signed_32() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint_with_suffix(
        stream, 
        0,
        10,
        true,
        32
    );
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0i32", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_with_suffix_unsigned_32() {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);
    frg_print_uint_with_suffix(
        stream, 
        0,
        10,
        false,
        32
    );
    GString* str = frg_stream_output_destroy_take_buffer(stream);
    TEST_ASSERT_EQUAL_STRING("0u32", str->str);
    g_string_free(str, TRUE);
}

void _test_parse_uint(
    uint64_t expected_value,
    bool expected_is_signed,
    frg_bit_width_t expected_bit_width,
    const char* formatted
) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_parse_uint_result_t actual;
    frg_parsing_location_t start = {
        .path = "--",
        .line_number = 1,
        .column_number = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &start,
        formatted
    );

    bool result = frg_parse_uint(
        &actual,
        message_buffer,
        reader
    );

    frg_message_buffer_print(
        frg_stream_output_get_stderr(),
        NULL,
        message_buffer,
        FRG_MESSAGE_SEVERITY_DEBUG,
        0
    );

    TEST_ASSERT(result);

    frg_parsing_token_reader_destroy(reader);

    TEST_ASSERT_EQUAL(expected_is_signed, actual.is_signed);
    TEST_ASSERT_EQUAL(expected_bit_width, actual.bit_width);
    TEST_ASSERT_EQUAL(expected_value, actual.value);

    TEST_ASSERT_EQUAL(0, message_buffer->message_count);

    frg_message_buffer_destroy(message_buffer);
}

void test_parse_uint_0() {
    _test_parse_uint(
        0,
        false,
        64,
        "0"
    );
}

void test_parse_uint_1() {
    _test_parse_uint(
        1,
        false,
        64,
        "1"
    );
}

void test_parse_uint_9() {
    _test_parse_uint(
        9,
        false,
        64,
        "9"
    );
}

void test_parse_uint_10() {
    _test_parse_uint(
        10,
        false,
        64,
        "10"
    );
}

void test_parse_uint_99() {
    _test_parse_uint(
        99,
        false,
        64,
        "99"
    );
}

void test_parse_uint_100() {
    _test_parse_uint(
        100,
        false,
        64,
        "100"
    );
}

void test_parse_uint_999() {
    _test_parse_uint(
        999,
        false,
        64,
        "999"
    );
}

void test_parse_uint_123456789() {
    _test_parse_uint(
        123456789,
        false,
        64,
        "123456789"
    );
}

void test_parse_uint_0b0() {
    _test_parse_uint(
        0,
        false,
        64,
        "0b0"
    );
}

void test_parse_uint_0b1() {
    // Hello there
    _test_parse_uint(
        1,
        false,
        64,
        "0b1"
    );
}

void test_parse_uint_0b10() {
    _test_parse_uint(
        2,
        false,
        64,
        "0b10"
    );
}

void test_parse_uint_0b11() {
    _test_parse_uint(
        3,
        false,
        64,
        "0b11"
    );
}

void test_parse_uint_0b11111111() {
    _test_parse_uint(
        0xff,
        false,
        64,
        "0b11111111"
    );
}

void test_parse_uint_0o0() {
    _test_parse_uint(
        0,
        false,
        64,
        "0o0"
    );
}

void test_parse_uint_0o7() {
    _test_parse_uint(
        07,
        false,
        64,
        "0o7"
    );
}

void test_parse_uint_0o10() {
    _test_parse_uint(
        010,
        false,
        64,
        "0o10"
    );
}

void test_parse_uint_0o77777777() {
    _test_parse_uint(
        077777777,
        false,
        64,
        "0o77777777"
    );
}

void test_parse_uint_0i8() {
    _test_parse_uint(
        0,
        true,
        8,
        "0i8"
    );
}

void test_parse_uint_0u32() {
    _test_parse_uint(
        0,
        false,
        32,
        "0u32"
    );
}

// TODO: Add in many more tests. This coverage is very limited.

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
    RUN_TEST(test_format_uint_0_base2);
    RUN_TEST(test_format_uint_1_base2);
    RUN_TEST(test_format_uint_10_base2);
    RUN_TEST(test_format_uint_11_base2);
    RUN_TEST(test_format_uint_100_base2);
    RUN_TEST(test_format_uint_111111111_base2);
    RUN_TEST(test_format_uint_0_base8);
    RUN_TEST(test_format_uint_7_base8);
    RUN_TEST(test_format_uint_10_base8);
    RUN_TEST(test_format_uint_77_base8);
    RUN_TEST(test_format_uint_100_base8);
    RUN_TEST(test_format_uint_7777777_base8);
    RUN_TEST(test_format_uint_0_base10);
    RUN_TEST(test_format_uint_9_base10);
    RUN_TEST(test_format_uint_10_base10);
    RUN_TEST(test_format_uint_99_base10);
    RUN_TEST(test_format_uint_100_base10);
    RUN_TEST(test_format_uint_123456789_base10);
    RUN_TEST(test_format_uint_0_base16);
    RUN_TEST(test_format_uint_9_base16);
    RUN_TEST(test_format_uint_a_base16);
    RUN_TEST(test_format_uint_f_base16);
    RUN_TEST(test_format_uint_10_base16);
    RUN_TEST(test_format_uint_99_base16);
    RUN_TEST(test_format_uint_aa_base16);
    RUN_TEST(test_format_uint_ff_base16);
    RUN_TEST(test_format_uint_100_base16);
    RUN_TEST(test_format_uint_ffffffff_base16);
    RUN_TEST(test_format_uint_with_suffix_signed_32);
    RUN_TEST(test_format_uint_with_suffix_unsigned_32);
    RUN_TEST(test_parse_uint_0);
    RUN_TEST(test_parse_uint_1);
    RUN_TEST(test_parse_uint_9);
    RUN_TEST(test_parse_uint_10);
    RUN_TEST(test_parse_uint_99);
    RUN_TEST(test_parse_uint_100);
    RUN_TEST(test_parse_uint_999);
    RUN_TEST(test_parse_uint_123456789);
    RUN_TEST(test_parse_uint_0b0);
    RUN_TEST(test_parse_uint_0b1);
    RUN_TEST(test_parse_uint_0b10);
    RUN_TEST(test_parse_uint_0b11);
    RUN_TEST(test_parse_uint_0b11111111);
    RUN_TEST(test_parse_uint_0o0);
    RUN_TEST(test_parse_uint_0o7);
    RUN_TEST(test_parse_uint_0o10);
    RUN_TEST(test_parse_uint_0o77777777);
    RUN_TEST(test_parse_uint_0i8);
    RUN_TEST(test_parse_uint_0u32);
    return UNITY_END();
}
