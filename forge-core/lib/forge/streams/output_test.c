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

#include <forge/streams/output.h>
#include <forge/common/memory.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_stream(frg_stream_output_t* stream) {
    frg_stream_output_write_char(stream, 'a');
    frg_stream_output_write_string(stream, "b");
    frg_stream_output_write_string_with_length(stream, "cd", 1);
    frg_stream_output_write_printf(stream, "%i", 1);
}

void test_stream_buffer(void) {
    frg_stream_output_t* stream = frg_stream_output_new_buffer(
        FRG_STREAM_OUTPUT_FLAG_NONE
    );

    test_stream(stream);

    GString* buffer = frg_stream_output_destroy_take_buffer(&stream);

    TEST_ASSERT_EQUAL_STRING("abc1", buffer->str);

    g_string_free(buffer, TRUE);
}

void test_stream_file(void) {
    char* buffer = frg_safe_malloc(1024);
    memset(buffer, 0, 1024);

    FILE* file = fmemopen(buffer, 1024, "w+");

    frg_stream_output_t* stream = frg_stream_output_new_file(
        file,
        false
    );

    test_stream(stream);

    frg_stream_output_destroy(&stream);

    fclose(file);

    TEST_ASSERT_EQUAL_STRING("abc1", buffer);

    frg_safe_free((void**)&buffer);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_stream_buffer);
    RUN_TEST(test_stream_file);
    return UNITY_END();
}
