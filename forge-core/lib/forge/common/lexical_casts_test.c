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

void test_parse_char_null(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\0'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL(0, value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_unprintable_ascii(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\x04'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\x04', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_printable_ascii(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'a'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('a', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_backslash(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\\\'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\\', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_single_quote(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\''");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\'', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_double_quote(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\"'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('"', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_alert(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\a'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\a', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_backspace(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\b'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\b', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_ascii_escape(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\e'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\033', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_form_feed(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\f'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\f', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_newline(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\n'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\n', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_carriage_return(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\r'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\r', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_horizontal_tab(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\t'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\t', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_vertical_tab(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\v'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\v', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_unprintable_unicode(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'\\u{97}'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL('\x97', value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_char_printable_unicode(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    frg_char_t value = 0;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "'😀'");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_char(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL(0x1f600, value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
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
    TEST_ASSERT_EQUAL_STRING("\"\\'\"", formatted->str);
}

void test_format_str_double_quote(void) {
    GString* formatted = frg_format_str("\"");
    TEST_ASSERT_EQUAL_STRING("\"\"\"", formatted->str);
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

void test_parse_str_empty(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_null(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\0\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_unprintable_ascii(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\x04\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\x04", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_printable_ascii(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"a\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("a", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_backslash(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\\\\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\\", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_single_quote(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\'\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("'", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_double_quote(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\\"\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\"", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_alert(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\a\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\a", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_backspace(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\b\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\b", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_ascii_escape(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\e\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\033", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_form_feed(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\f\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\f", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_newline(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\n\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\n", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_carriage_return(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\r\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\r", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_horizontal_tab(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\t\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\t", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_vertical_tab(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\v\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\v", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_unprintable_unicode(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\u{97}\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL_STRING("\x97", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_printable_unicode(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"😀\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL("", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_multiple_chars_ascii(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"hello, world\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL("hello, world", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_multiple_chars_unprintable_unicode(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"\\u{97}\\u{98}\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL("\x97\x98", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_parse_str_multiple_chars_printable_unicode(void) {
    frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    GString* value = NULL;
    frg_parsing_location_t start = {
        .path = "--",
        .lineno = 1,
        .columnno = 1,
        .offset = 0
    };

    frg_parsing_token_reader_t* reader = frg_parsing_token_reader_new(&start, "\"😀😘\"");

    TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_parse_str(message_buffer, &value, reader));

    frg_parsing_token_reader_destroy(&reader);

    TEST_ASSERT_EQUAL("😀😘", value);

    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_message_count(message_buffer));

    frg_message_buffer_destroy(&message_buffer);
}

void test_frg_format_uint_0_base10(void) {
    GString* str = frg_format_uint(0, 10);
    TEST_ASSERT_EQUAL_STRING("0", str->str);
}

void test_frg_format_uint_9_base10(void) {
    GString* str = frg_format_uint(9, 10);
    TEST_ASSERT_EQUAL_STRING("9", str->str);
}

void test_frg_format_uint_10_base10(void) {
    GString* str = frg_format_uint(10, 10);
    TEST_ASSERT_EQUAL_STRING("10", str->str);
}

void test_frg_format_uint_99_base10(void) {
    GString* str = frg_format_uint(99, 10);
    TEST_ASSERT_EQUAL_STRING("99", str->str);
}

void test_frg_format_uint_100_base10(void) {
    GString* str = frg_format_uint(100, 10);
    TEST_ASSERT_EQUAL_STRING("100", str->str);
}

void test_frg_format_uint_123456789_base10(void) {
    GString* str = frg_format_uint(123456789, 10);
    TEST_ASSERT_EQUAL_STRING("123456789", str->str);
}

// void test_str_to_uint_10_base10(void) {
//     uint64_t value = 0;
//     TEST_ASSERT_EQUAL(FRG_RECOVERABLE_STATUS_OK, frg_str_to_uint(&value, "10"));
//     TEST_ASSERT_EQUAL(10, value);
// }

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
    // RUN_TEST(test_parse_char_printable_unicode);
    // RUN_TEST(test_format_str_empty);
    // RUN_TEST(test_format_str_null);
    // RUN_TEST(test_format_str_unprintable_ascii);
    // RUN_TEST(test_format_str_printable_ascii);
    // RUN_TEST(test_format_str_backslash);
    // RUN_TEST(test_format_str_single_quote);
    // RUN_TEST(test_format_str_double_quote);
    // RUN_TEST(test_format_str_alert);
    // RUN_TEST(test_format_str_backspace);
    // RUN_TEST(test_format_str_ascii_escape);
    // RUN_TEST(test_format_str_form_feed);
    // RUN_TEST(test_format_str_newline);
    // RUN_TEST(test_format_str_carriage_return);
    // RUN_TEST(test_format_str_horizontal_tab);
    // RUN_TEST(test_format_str_vertical_tab);
    // RUN_TEST(test_format_str_unprintable_unicode);
    // RUN_TEST(test_format_str_printable_unicode);
    // RUN_TEST(test_format_str_multiple_chars_ascii);
    // RUN_TEST(test_format_str_multiple_chars_unprintable_unicode);
    // RUN_TEST(test_format_str_multiple_chars_printable_unicode);
    // RUN_TEST(test_parse_str_empty);
    // RUN_TEST(test_parse_str_null);
    // RUN_TEST(test_parse_str_unprintable_ascii);
    // RUN_TEST(test_parse_str_printable_ascii);
    // RUN_TEST(test_parse_str_backslash);
    // RUN_TEST(test_parse_str_single_quote);
    // RUN_TEST(test_parse_str_double_quote);
    // RUN_TEST(test_parse_str_alert);
    // RUN_TEST(test_parse_str_backspace);
    // RUN_TEST(test_parse_str_ascii_escape);
    // RUN_TEST(test_parse_str_form_feed);
    // RUN_TEST(test_parse_str_newline);
    // RUN_TEST(test_parse_str_carriage_return);
    // RUN_TEST(test_parse_str_horizontal_tab);
    // RUN_TEST(test_parse_str_vertical_tab);
    // RUN_TEST(test_parse_str_unprintable_unicode);
    // RUN_TEST(test_parse_str_printable_unicode);
    // RUN_TEST(test_parse_str_multiple_chars_ascii);
    // RUN_TEST(test_parse_str_multiple_chars_unprintable_unicode);
    // RUN_TEST(test_parse_str_multiple_chars_printable_unicode);
    // RUN_TEST(test_frg_format_uint_0_base10);
    // RUN_TEST(test_frg_format_uint_9_base10);
    // RUN_TEST(test_frg_format_uint_10_base10);
    // RUN_TEST(test_frg_format_uint_99_base10);
    // RUN_TEST(test_frg_format_uint_100_base10);
    // RUN_TEST(test_frg_format_uint_123456789_base10);
    return UNITY_END();
}
