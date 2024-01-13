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

#include <forge/streams/input.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

static char _buffer[] = "hello, world\n";

frg_stream_input_t* create_stream_file() {
    FILE* file = fmemopen(_buffer, sizeof(_buffer), "r");

    return frg_stream_input_new_file(file, true);
}

frg_stream_input_t* create_stream_buffer() {
    return frg_stream_input_new_buffer(_buffer, false);
}

void test_stream(frg_stream_input_t* stream) {
    TEST_ASSERT_EQUAL(0, frg_stream_input_get_offset(stream));
    TEST_ASSERT_FALSE(frg_stream_input_is_at_end(stream));
    TEST_ASSERT_FALSE(frg_stream_input_has_error(stream));

    TEST_ASSERT_EQUAL('h', frg_stream_input_peek_char(stream));
    TEST_ASSERT_EQUAL(0, frg_stream_input_get_offset(stream));
    TEST_ASSERT_FALSE(frg_stream_input_is_at_end(stream));
    TEST_ASSERT_FALSE(frg_stream_input_has_error(stream));

    TEST_ASSERT_EQUAL('h', frg_stream_input_read_char(stream));
    TEST_ASSERT_EQUAL(1, frg_stream_input_get_offset(stream));
    TEST_ASSERT_FALSE(frg_stream_input_is_at_end(stream));
    TEST_ASSERT_FALSE(frg_stream_input_has_error(stream));

    TEST_ASSERT_EQUAL('e', frg_stream_input_read_char(stream));
    TEST_ASSERT_EQUAL(2, frg_stream_input_get_offset(stream));
    TEST_ASSERT_FALSE(frg_stream_input_is_at_end(stream));
    TEST_ASSERT_FALSE(frg_stream_input_has_error(stream));

    frg_stream_input_seek_to_offset(stream, 0);
    TEST_ASSERT_EQUAL(0, frg_stream_input_get_offset(stream));
    TEST_ASSERT_FALSE(frg_stream_input_is_at_end(stream));
    TEST_ASSERT_FALSE(frg_stream_input_has_error(stream));

    char buffer[14];
    TEST_ASSERT_EQUAL(14, frg_stream_input_read_buffer(stream, buffer, 14));
    TEST_ASSERT_EQUAL_STRING("hello, world\n", buffer);
    TEST_ASSERT_EQUAL(14, frg_stream_input_get_offset(stream));
    TEST_ASSERT_FALSE(frg_stream_input_has_error(stream));

    frg_stream_input_seek_to_offset(stream, 5);
    TEST_ASSERT_EQUAL(5, frg_stream_input_get_offset(stream));
    TEST_ASSERT_FALSE(frg_stream_input_is_at_end(stream));
    TEST_ASSERT_FALSE(frg_stream_input_has_error(stream));

    buffer[2] = 0;
    TEST_ASSERT_EQUAL(2, frg_stream_input_read_buffer(stream, buffer, 2));
    TEST_ASSERT_EQUAL_STRING(", ", buffer);
    TEST_ASSERT_EQUAL(7, frg_stream_input_get_offset(stream));
    TEST_ASSERT_FALSE(frg_stream_input_is_at_end(stream));
    TEST_ASSERT_FALSE(frg_stream_input_has_error(stream));
}

void test_stream_buffer(void) {
    frg_stream_input_t* stream = create_stream_buffer();

    test_stream(stream);

    frg_stream_input_destroy(&stream);
}

void test_stream_file(void) {
    frg_stream_input_t* stream = create_stream_file();

    test_stream(stream);

    frg_stream_input_destroy(&stream);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_stream_buffer);
    RUN_TEST(test_stream_file);
    return UNITY_END();
}
