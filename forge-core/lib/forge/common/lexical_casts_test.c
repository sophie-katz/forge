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

#include <forge/common/lexical_casts.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_format_char_null(void) {
    GString* formatted = frg_format_char(0);
    TEST_ASSERT_EQUAL_STRING("'\\0'", formatted->str);
}

void test_format_char_unprintable_ascii(void) {
    GString* formatted = frg_format_char(0x04);
    TEST_ASSERT_EQUAL_STRING("'\\x04'", formatted->str);
}

void test_format_char_printable_ascii(void) {
    GString* formatted = frg_format_char('a');
    TEST_ASSERT_EQUAL_STRING("'a'", formatted->str);
}

void test_format_char_backslash(void) {
    GString* formatted = frg_format_char('\\');
    TEST_ASSERT_EQUAL_STRING("'\\\\'", formatted->str);
}

void test_format_char_single_quote(void) {
    GString* formatted = frg_format_char('\'');
    TEST_ASSERT_EQUAL_STRING("'\\''", formatted->str);
}

void test_format_char_double_quote(void) {
    GString* formatted = frg_format_char('"');
    TEST_ASSERT_EQUAL_STRING("'\"'", formatted->str);
}

void test_format_char_alert(void) {
    GString* formatted = frg_format_char('\a');
    TEST_ASSERT_EQUAL_STRING("'\\a'", formatted->str);
}

void test_format_char_backspace(void) {
    GString* formatted = frg_format_char('\b');
    TEST_ASSERT_EQUAL_STRING("'\\b'", formatted->str);
}

void test_format_char_ascii_escape(void) {
    GString* formatted = frg_format_char('\033');
    TEST_ASSERT_EQUAL_STRING("'\\e'", formatted->str);
}

void test_format_char_form_feed(void) {
    GString* formatted = frg_format_char('\f');
    TEST_ASSERT_EQUAL_STRING("'\\f'", formatted->str);
}

void test_format_char_newline(void) {
    GString* formatted = frg_format_char('\n');
    TEST_ASSERT_EQUAL_STRING("'\\n'", formatted->str);
}

void test_format_char_carriage_return(void) {
    GString* formatted = frg_format_char('\r');
    TEST_ASSERT_EQUAL_STRING("'\\r'", formatted->str);
}

void test_format_char_horizontal_tab(void) {
    GString* formatted = frg_format_char('\t');
    TEST_ASSERT_EQUAL_STRING("'\\t'", formatted->str);
}

void test_format_char_vertical_tab(void) {
    GString* formatted = frg_format_char('\v');
    TEST_ASSERT_EQUAL_STRING("'\\v'", formatted->str);
}

void test_format_char_unprintable_unicode(void) {
    GString* formatted = frg_format_char(0x97);
    TEST_ASSERT_EQUAL_STRING("'\\u{97}'", formatted->str);
}

void test_format_char_printable_unicode(void) {
    GString* formatted = frg_format_char(0x1f600);
    TEST_ASSERT_EQUAL_STRING("'😀'", formatted->str);
}

void _test_parse_char(frg_char_t expected, const char* formatted) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t actual = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &start,
        formatted
    );

    bool result = frg_parse_char(message_buffer, &actual, reader);

    frg_message_buffer_print(
        frg_stream_output_get_stderr(),
        message_buffer,
        NULL,
        FRG_MESSAGE_SEVERITY_DEBUG,
        0
    );

    TEST_ASSERT(result);

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL(expected, actual);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_null(void) {
    _test_parse_char(0, "'\\0'");
}

void test_parse_char_unprintable_ascii(void) {
    _test_parse_char('\x04', "'\\x04'");
}

void test_parse_char_printable_ascii(void) {
    _test_parse_char('a', "'a'");
}

void test_parse_char_backslash(void) {
    _test_parse_char('\\', "'\\\\'");
}

void test_parse_char_single_quote(void) {
    _test_parse_char('\'', "'\\''");
}

void test_parse_char_double_quote(void) {
    _test_parse_char('"', "'\"'");
}

void test_parse_char_alert(void) {
    _test_parse_char('\a', "'\\a'");
}

void test_parse_char_backspace(void) {
    _test_parse_char('\b', "'\\b'");
}

void test_parse_char_ascii_escape(void) {
    _test_parse_char('\033', "'\\e'");
}

void test_parse_char_form_feed(void) {
    _test_parse_char('\f', "'\\f'");
}

void test_parse_char_newline(void) {
    _test_parse_char('\n', "'\\n'");
}

void test_parse_char_carriage_return(void) {
    _test_parse_char('\r', "'\\r'");
}

void test_parse_char_horizontal_tab(void) {
    _test_parse_char('\t', "'\\t'");
}

void test_parse_char_vertical_tab(void) {
    _test_parse_char('\v', "'\\v'");
}

void test_parse_char_unprintable_unicode(void) {
    _test_parse_char(0x97, "'\\u{97}'");
}

void test_parse_char_printable_unicode(void) {
    _test_parse_char(0x1f600, "'😀'");
}

void test_format_str_empty(void) {
    GString* formatted = frg_format_str("");
    TEST_ASSERT_EQUAL_STRING("\"\"", formatted->str);
}

void test_format_str_null(void) {
    GString* formatted = frg_format_str("\0");
    TEST_ASSERT_EQUAL_STRING("\"\"", formatted->str);
}

void test_format_str_unprintable_ascii(void) {
    GString* formatted = frg_format_str("\x04");
    TEST_ASSERT_EQUAL_STRING("\"\\x04\"", formatted->str);
}

void test_format_str_printable_ascii(void) {
    GString* formatted = frg_format_str("a");
    TEST_ASSERT_EQUAL_STRING("\"a\"", formatted->str);
}

void test_format_str_backslash(void) {
    GString* formatted = frg_format_str("\\");
    TEST_ASSERT_EQUAL_STRING("\"\\\\\"", formatted->str);
}

void test_format_str_single_quote(void) {
    GString* formatted = frg_format_str("'");
    TEST_ASSERT_EQUAL_STRING("\"'\"", formatted->str);
}

void test_format_str_double_quote(void) {
    GString* formatted = frg_format_str("\"");
    TEST_ASSERT_EQUAL_STRING("\"\\\"\"", formatted->str);
}

void test_format_str_alert(void) {
    GString* formatted = frg_format_str("\a");
    TEST_ASSERT_EQUAL_STRING("\"\\a\"", formatted->str);
}

void test_format_str_backspace(void) {
    GString* formatted = frg_format_str("\b");
    TEST_ASSERT_EQUAL_STRING("\"\\b\"", formatted->str);
}

void test_format_str_ascii_escape(void) {
    GString* formatted = frg_format_str("\033");
    TEST_ASSERT_EQUAL_STRING("\"\\e\"", formatted->str);
}

void test_format_str_form_feed(void) {
    GString* formatted = frg_format_str("\f");
    TEST_ASSERT_EQUAL_STRING("\"\\f\"", formatted->str);
}

void test_format_str_newline(void) {
    GString* formatted = frg_format_str("\n");
    TEST_ASSERT_EQUAL_STRING("\"\\n\"", formatted->str);
}

void test_format_str_carriage_return(void) {
    GString* formatted = frg_format_str("\r");
    TEST_ASSERT_EQUAL_STRING("\"\\r\"", formatted->str);
}

void test_format_str_horizontal_tab(void) {
    GString* formatted = frg_format_str("\t");
    TEST_ASSERT_EQUAL_STRING("\"\\t\"", formatted->str);
}

void test_format_str_vertical_tab(void) {
    GString* formatted = frg_format_str("\v");
    TEST_ASSERT_EQUAL_STRING("\"\\v\"", formatted->str);
}

void test_format_str_unprintable_unicode(void) {
    GString* formatted = frg_format_str("\x97");
    TEST_ASSERT_EQUAL_STRING("\"\\u{97}\"", formatted->str);
}

void test_format_str_printable_unicode(void) {
    GString* formatted = frg_format_str("😀");
    TEST_ASSERT_EQUAL_STRING("\"😀\"", formatted->str);
}

void test_format_str_multiple_chars_ascii(void) {
    GString* formatted = frg_format_str("hello, world");
    TEST_ASSERT_EQUAL_STRING("\"hello, world\"", formatted->str);
}

void test_format_str_multiple_chars_unprintable_unicode(void) {
    GString* formatted = frg_format_str("\x97\x98");
    TEST_ASSERT_EQUAL_STRING("\"\\u{97}\\u{98}\"", formatted->str);
}

void test_format_str_multiple_chars_printable_unicode(void) {
    GString* formatted = frg_format_str("😀😘");
    TEST_ASSERT_EQUAL_STRING("\"😀😘\"", formatted->str);
}

void _test_parse_str(const char* expected, const char* formatted) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* actual = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &start,
        formatted
    );

    bool result = frg_parse_str(message_buffer, &actual, reader);

    frg_message_buffer_print(
        frg_stream_output_get_stderr(),
        message_buffer,
        NULL,
        FRG_MESSAGE_SEVERITY_DEBUG,
        0
    );

    TEST_ASSERT(result);

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING(expected, actual->str);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_empty(void) {
    _test_parse_str("", "\"\"");
}

void test_parse_str_null(void) {
    _test_parse_str("", "\"\\0\"");
}

void test_parse_str_unprintable_ascii(void) {
    _test_parse_str("\x04", "\"\\x04\"");
}

void test_parse_str_printable_ascii(void) {
    _test_parse_str("a", "\"a\"");
}

void test_parse_str_backslash(void) {
    _test_parse_str("\\", "\"\\\\\"");
}

void test_parse_str_single_quote(void) {
    _test_parse_str("'", "\"'\"");
}

void test_parse_str_double_quote(void) {
    _test_parse_str("\"", "\"\\\"\"");
}

void test_parse_str_alert(void) {
    _test_parse_str("\a", "\"\\a\"");
}

void test_parse_str_backspace(void) {
    _test_parse_str("\b", "\"\\b\"");
}

void test_parse_str_ascii_escape(void) {
    _test_parse_str("\033", "\"\\e\"");
}

void test_parse_str_form_feed(void) {
    _test_parse_str("\f", "\"\\f\"");
}

void test_parse_str_newline(void) {
    _test_parse_str("\n", "\"\\n\"");
}

void test_parse_str_carriage_return(void) {
    _test_parse_str("\r", "\"\\r\"");
}

void test_parse_str_horizontal_tab(void) {
    _test_parse_str("\t", "\"\\t\"");
}

void test_parse_str_vertical_tab(void) {
    _test_parse_str("\v", "\"\\v\"");
}

void test_parse_str_unprintable_unicode(void) {
    _test_parse_str("\x97", "\"\\u{97}\"");
}

void test_parse_str_printable_unicode(void) {
    _test_parse_str("😀", "\"😀\"");
}

void test_parse_str_multiple_chars_ascii(void) {
    _test_parse_str("hello, world", "\"hello, world\"");
}

void test_parse_str_multiple_chars_unprintable_unicode(void) {
    _test_parse_str("\x97\x98", "\"\\u{97}\\u{98}\"");
}

void test_parse_str_multiple_chars_printable_unicode(void) {
    _test_parse_str("😀😘", "\"😀😘\"");
}

void test_format_uint_0_base2(void) {
    GString* str = frg_format_uint(0, 2);
    TEST_ASSERT_EQUAL_STRING("0b0", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_1_base2(void) {
    GString* str = frg_format_uint(1, 2);
    TEST_ASSERT_EQUAL_STRING("0b1", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_10_base2(void) {
    GString* str = frg_format_uint(2, 2);
    TEST_ASSERT_EQUAL_STRING("0b10", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_11_base2(void) {
    GString* str = frg_format_uint(3, 2);
    TEST_ASSERT_EQUAL_STRING("0b11", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_100_base2(void) {
    GString* str = frg_format_uint(4, 2);
    TEST_ASSERT_EQUAL_STRING("0b100", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_111111111_base2(void) {
    GString* str = frg_format_uint(0xff, 2);
    TEST_ASSERT_EQUAL_STRING("0b11111111", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_0_base8(void) {
    GString* str = frg_format_uint(0, 8);
    TEST_ASSERT_EQUAL_STRING("0o0", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_7_base8(void) {
    GString* str = frg_format_uint(7, 8);
    TEST_ASSERT_EQUAL_STRING("0o7", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_10_base8(void) {
    GString* str = frg_format_uint(010, 8);
    TEST_ASSERT_EQUAL_STRING("0o10", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_77_base8(void) {
    GString* str = frg_format_uint(077, 8);
    TEST_ASSERT_EQUAL_STRING("0o77", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_100_base8(void) {
    GString* str = frg_format_uint(0100, 8);
    TEST_ASSERT_EQUAL_STRING("0o100", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_7777777_base8(void) {
    GString* str = frg_format_uint(07777777, 8);
    TEST_ASSERT_EQUAL_STRING("0o7777777", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_0_base10(void) {
    GString* str = frg_format_uint(0, 10);
    TEST_ASSERT_EQUAL_STRING("0", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_9_base10(void) {
    GString* str = frg_format_uint(9, 10);
    TEST_ASSERT_EQUAL_STRING("9", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_10_base10(void) {
    GString* str = frg_format_uint(10, 10);
    TEST_ASSERT_EQUAL_STRING("10", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_99_base10(void) {
    GString* str = frg_format_uint(99, 10);
    TEST_ASSERT_EQUAL_STRING("99", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_100_base10(void) {
    GString* str = frg_format_uint(100, 10);
    TEST_ASSERT_EQUAL_STRING("100", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_123456789_base10(void) {
    GString* str = frg_format_uint(123456789, 10);
    TEST_ASSERT_EQUAL_STRING("123456789", str->str);
    g_string_free(str, TRUE);
}








void test_format_uint_0_base16(void) {
    GString* str = frg_format_uint(0x0, 16);
    TEST_ASSERT_EQUAL_STRING("0x0", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_9_base16(void) {
    GString* str = frg_format_uint(0x9, 16);
    TEST_ASSERT_EQUAL_STRING("0x9", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_a_base16(void) {
    GString* str = frg_format_uint(0xa, 16);
    TEST_ASSERT_EQUAL_STRING("0xa", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_f_base16(void) {
    GString* str = frg_format_uint(0xf, 16);
    TEST_ASSERT_EQUAL_STRING("0xf", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_10_base16(void) {
    GString* str = frg_format_uint(0x10, 16);
    TEST_ASSERT_EQUAL_STRING("0x10", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_99_base16(void) {
    GString* str = frg_format_uint(0x99, 16);
    TEST_ASSERT_EQUAL_STRING("0x99", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_aa_base16(void) {
    GString* str = frg_format_uint(0xaa, 16);
    TEST_ASSERT_EQUAL_STRING("0xaa", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_ff_base16(void) {
    GString* str = frg_format_uint(0xff, 16);
    TEST_ASSERT_EQUAL_STRING("0xff", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_100_base16(void) {
    GString* str = frg_format_uint(0x100, 16);
    TEST_ASSERT_EQUAL_STRING("0x100", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_ffffffff_base16(void) {
    GString* str = frg_format_uint(0xffffffff, 16);
    TEST_ASSERT_EQUAL_STRING("0xffffffff", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_with_suffix_signed_32(void) {
    GString* str = frg_format_uint_with_suffix(
        0,
        10,
        true,
        32
    );
    TEST_ASSERT_EQUAL_STRING("0i32", str->str);
    g_string_free(str, TRUE);
}

void test_format_uint_with_suffix_unsigned_32(void) {
    GString* str = frg_format_uint_with_suffix(
        0,
        10,
        false,
        32
    );
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
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(
        &start,
        formatted
    );

    bool result = frg_parse_uint(
        message_buffer,
        &actual,
        reader
    );

    frg_message_buffer_print(
        frg_stream_output_get_stderr(),
        message_buffer,
        NULL,
        FRG_MESSAGE_SEVERITY_DEBUG,
        0
    );

    TEST_ASSERT(result);

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL(expected_is_signed, actual.is_signed);
    TEST_ASSERT_EQUAL(expected_bit_width, actual.bit_width);
    TEST_ASSERT_EQUAL(expected_value, actual.value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_uint_0(void) {
    _test_parse_uint(
        0,
        false,
        64,
        "0"
    );
}

void test_parse_uint_1(void) {
    _test_parse_uint(
        1,
        false,
        64,
        "1"
    );
}

void test_parse_uint_9(void) {
    _test_parse_uint(
        9,
        false,
        64,
        "9"
    );
}

void test_parse_uint_10(void) {
    _test_parse_uint(
        10,
        false,
        64,
        "10"
    );
}

void test_parse_uint_99(void) {
    _test_parse_uint(
        99,
        false,
        64,
        "99"
    );
}

void test_parse_uint_100(void) {
    _test_parse_uint(
        100,
        false,
        64,
        "100"
    );
}

void test_parse_uint_999(void) {
    _test_parse_uint(
        999,
        false,
        64,
        "999"
    );
}

void test_parse_uint_123456789(void) {
    _test_parse_uint(
        123456789,
        false,
        64,
        "123456789"
    );
}

void test_parse_uint_0b0(void) {
    _test_parse_uint(
        0,
        false,
        64,
        "0b0"
    );
}

void test_parse_uint_0b1(void) {
    // Hello there
    _test_parse_uint(
        1,
        false,
        64,
        "0b1"
    );
}

void test_parse_uint_0b10(void) {
    _test_parse_uint(
        2,
        false,
        64,
        "0b10"
    );
}

void test_parse_uint_0b11(void) {
    _test_parse_uint(
        3,
        false,
        64,
        "0b11"
    );
}

void test_parse_uint_0b11111111(void) {
    _test_parse_uint(
        0xff,
        false,
        64,
        "0b11111111"
    );
}

void test_parse_uint_0o0(void) {
    _test_parse_uint(
        0,
        false,
        64,
        "0o0"
    );
}

void test_parse_uint_0o7(void) {
    _test_parse_uint(
        07,
        false,
        64,
        "0o7"
    );
}

void test_parse_uint_0o10(void) {
    _test_parse_uint(
        010,
        false,
        64,
        "0o10"
    );
}

void test_parse_uint_0o77777777(void) {
    _test_parse_uint(
        077777777,
        false,
        64,
        "0o77777777"
    );
}

void test_parse_uint_0i8(void) {
    _test_parse_uint(
        0,
        true,
        8,
        "0i8"
    );
}

void test_parse_uint_0u32(void) {
    _test_parse_uint(
        0,
        false,
        32,
        "0u32"
    );
}

// TODO: Add in many more tests. This coverage is very limited.

int main(void) {
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
    RUN_TEST(test_format_str_empty);
    RUN_TEST(test_format_str_null);
    RUN_TEST(test_format_str_unprintable_ascii);
    RUN_TEST(test_format_str_printable_ascii);
    RUN_TEST(test_format_str_backslash);
    RUN_TEST(test_format_str_single_quote);
    RUN_TEST(test_format_str_double_quote);
    RUN_TEST(test_format_str_alert);
    RUN_TEST(test_format_str_backspace);
    RUN_TEST(test_format_str_ascii_escape);
    RUN_TEST(test_format_str_form_feed);
    RUN_TEST(test_format_str_newline);
    RUN_TEST(test_format_str_carriage_return);
    RUN_TEST(test_format_str_horizontal_tab);
    RUN_TEST(test_format_str_vertical_tab);
    RUN_TEST(test_format_str_unprintable_unicode);
    RUN_TEST(test_format_str_printable_unicode);
    RUN_TEST(test_format_str_multiple_chars_ascii);
    RUN_TEST(test_format_str_multiple_chars_unprintable_unicode);
    RUN_TEST(test_format_str_multiple_chars_printable_unicode);
    RUN_TEST(test_parse_str_empty);
    RUN_TEST(test_parse_str_null);
    RUN_TEST(test_parse_str_unprintable_ascii);
    RUN_TEST(test_parse_str_printable_ascii);
    RUN_TEST(test_parse_str_backslash);
    RUN_TEST(test_parse_str_single_quote);
    RUN_TEST(test_parse_str_double_quote);
    RUN_TEST(test_parse_str_alert);
    RUN_TEST(test_parse_str_backspace);
    RUN_TEST(test_parse_str_ascii_escape);
    RUN_TEST(test_parse_str_form_feed);
    RUN_TEST(test_parse_str_newline);
    RUN_TEST(test_parse_str_carriage_return);
    RUN_TEST(test_parse_str_horizontal_tab);
    RUN_TEST(test_parse_str_vertical_tab);
    RUN_TEST(test_parse_str_unprintable_unicode);
    RUN_TEST(test_parse_str_printable_unicode);
    RUN_TEST(test_parse_str_multiple_chars_ascii);
    RUN_TEST(test_parse_str_multiple_chars_unprintable_unicode);
    RUN_TEST(test_parse_str_multiple_chars_printable_unicode);
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
