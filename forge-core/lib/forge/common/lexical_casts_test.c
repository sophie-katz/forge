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

#include <forge/common/lexical_casts.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_escape_char_null(void) {
    GString* escaped = frg_escape_char(0);
    TEST_ASSERT_EQUAL_STRING("'\\0'", escaped->str);
}

void test_escape_char_unprintable_ascii(void) {
    GString* escaped = frg_escape_char(0x04);
    TEST_ASSERT_EQUAL_STRING("'\\x04'", escaped->str);
}

void test_escape_char_printable_ascii(void) {
    GString* escaped = frg_escape_char('a');
    TEST_ASSERT_EQUAL_STRING("'a'", escaped->str);
}

void test_escape_char_backslash(void) {
    GString* escaped = frg_escape_char('\\');
    TEST_ASSERT_EQUAL_STRING("'\\\\'", escaped->str);
}

void test_escape_char_single_quote(void) {
    GString* escaped = frg_escape_char('\'');
    TEST_ASSERT_EQUAL_STRING("'\\''", escaped->str);
}

void test_escape_char_double_quote(void) {
    GString* escaped = frg_escape_char('"');
    TEST_ASSERT_EQUAL_STRING("'\"'", escaped->str);
}

void test_escape_char_alert(void) {
    GString* escaped = frg_escape_char('\a');
    TEST_ASSERT_EQUAL_STRING("'\\a'", escaped->str);
}

void test_escape_char_backspace(void) {
    GString* escaped = frg_escape_char('\b');
    TEST_ASSERT_EQUAL_STRING("'\\b'", escaped->str);
}

void test_escape_char_ascii_escape(void) {
    GString* escaped = frg_escape_char('\033');
    TEST_ASSERT_EQUAL_STRING("'\\e'", escaped->str);
}

void test_escape_char_form_feed(void) {
    GString* escaped = frg_escape_char('\f');
    TEST_ASSERT_EQUAL_STRING("'\\f'", escaped->str);
}

void test_escape_char_newline(void) {
    GString* escaped = frg_escape_char('\n');
    TEST_ASSERT_EQUAL_STRING("'\\n'", escaped->str);
}

void test_escape_char_carriage_return(void) {
    GString* escaped = frg_escape_char('\r');
    TEST_ASSERT_EQUAL_STRING("'\\r'", escaped->str);
}

void test_escape_char_horizontal_tab(void) {
    GString* escaped = frg_escape_char('\t');
    TEST_ASSERT_EQUAL_STRING("'\\t'", escaped->str);
}

void test_escape_char_vertical_tab(void) {
    GString* escaped = frg_escape_char('\v');
    TEST_ASSERT_EQUAL_STRING("'\\v'", escaped->str);
}

void test_escape_char_unprintable_unicode(void) {
    GString* escaped = frg_escape_char(0x97);
    TEST_ASSERT_EQUAL_STRING("'\\u{97}'", escaped->str);
}

void test_escape_char_printable_unicode(void) {
    GString* escaped = frg_escape_char(0x1f600);
    TEST_ASSERT_EQUAL_STRING("'😀'", escaped->str);
}

void test_unescape_char_null(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\0'"));
    TEST_ASSERT_EQUAL(0, value);
}

void test_unescape_char_unprintable_ascii(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\x04'"));
    TEST_ASSERT_EQUAL('\x04', value);
}

void test_unescape_char_printable_ascii(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'a'"));
    TEST_ASSERT_EQUAL('a', value);
}

void test_unescape_char_backslash(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\\\'"));
    TEST_ASSERT_EQUAL('\\', value);
}

void test_unescape_char_single_quote(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\\''"));
    TEST_ASSERT_EQUAL('\'', value);
}

void test_unescape_char_double_quote(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\"'"));
    TEST_ASSERT_EQUAL('"', value);
}

void test_unescape_char_alert(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\a'"));
    TEST_ASSERT_EQUAL('\a', value);
}

void test_unescape_char_backspace(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\b'"));
    TEST_ASSERT_EQUAL('\b', value);
}

void test_unescape_char_ascii_escape(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\e'"));
    TEST_ASSERT_EQUAL('\033', value);
}

void test_unescape_char_form_feed(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\f'"));
    TEST_ASSERT_EQUAL('\f', value);
}

void test_unescape_char_newline(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\n'"));
    TEST_ASSERT_EQUAL('\n', value);
}

void test_unescape_char_carriage_return(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\r'"));
    TEST_ASSERT_EQUAL('\r', value);
}

void test_unescape_char_horizontal_tab(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\t'"));
    TEST_ASSERT_EQUAL('\t', value);
}

void test_unescape_char_vertical_tab(void) {
    frg_char_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\v'"));
    TEST_ASSERT_EQUAL('\v', value);
}

// void test_unescape_char_unprintable_unicode(void) {
//     frg_char_t value = 0;
//     TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'\\u{97}'"));
//     TEST_ASSERT_EQUAL('\x97', value);
// }

// TODO: Fix this bug
// void test_unescape_char_printable_unicode(void) {
//     frg_char_t value = 0;
//     TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_unescape_char(&value, "'😀'"));
//     TEST_ASSERT_EQUAL(0x1f600, value);
// }

void test_uint_to_str_10_base10(void) {
    GString* str = frg_uint_to_str(10, 10);
    TEST_ASSERT_EQUAL_STRING("10", str->str);
}

void test_str_to_uint_10_base10(void) {
    uint64_t value = 0;
    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_str_to_uint(&value, "10"));
    TEST_ASSERT_EQUAL(10, value);
}

// void test_float_to_str_10_5_base10(void) {
//     GString* str = frg_float_to_str(10.5, 10);
//     TEST_ASSERT_EQUAL_STRING("10.5", str->str);
// }

// void test_str_to_float_10_5_base10(void) {
//     double value = 0;
//     TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_str_to_float(&value, "10.5"));
//     TEST_ASSERT_EQUAL(10.5, value);
// }

// TODO: Add in many more tests. This coverage is very limited.

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_escape_char_null);
    RUN_TEST(test_escape_char_unprintable_ascii);
    RUN_TEST(test_escape_char_printable_ascii);
    RUN_TEST(test_escape_char_backslash);
    RUN_TEST(test_escape_char_single_quote);
    RUN_TEST(test_escape_char_double_quote);
    RUN_TEST(test_escape_char_alert);
    RUN_TEST(test_escape_char_backspace);
    RUN_TEST(test_escape_char_ascii_escape);
    RUN_TEST(test_escape_char_form_feed);
    RUN_TEST(test_escape_char_newline);
    RUN_TEST(test_escape_char_carriage_return);
    RUN_TEST(test_escape_char_horizontal_tab);
    RUN_TEST(test_escape_char_vertical_tab);
    RUN_TEST(test_escape_char_unprintable_unicode);
    RUN_TEST(test_escape_char_printable_unicode);
    RUN_TEST(test_unescape_char_printable_ascii);
    RUN_TEST(test_unescape_char_backslash);
    RUN_TEST(test_unescape_char_single_quote);
    RUN_TEST(test_unescape_char_double_quote);
    RUN_TEST(test_unescape_char_alert);
    RUN_TEST(test_unescape_char_backspace);
    RUN_TEST(test_unescape_char_ascii_escape);
    RUN_TEST(test_unescape_char_form_feed);
    RUN_TEST(test_unescape_char_newline);
    RUN_TEST(test_unescape_char_carriage_return);
    RUN_TEST(test_unescape_char_horizontal_tab);
    RUN_TEST(test_unescape_char_vertical_tab);
    RUN_TEST(test_unescape_char_unprintable_unicode);
    // RUN_TEST(test_unescape_char_printable_unicode);
    RUN_TEST(test_uint_to_str_10_base10);
    RUN_TEST(test_str_to_uint_10_base10);
    // RUN_TEST(test_float_to_str_10_5_base10);
    // RUN_TEST(test_str_to_float_10_5_base10);
    return UNITY_END();
}
