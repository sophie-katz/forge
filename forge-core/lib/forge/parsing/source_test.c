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

#include <forge/parsing/source.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_buffer_load_at_beginning_of_first_line(void) {
    char buffer[] = "hello, world";

    frg_stream_input_t* stream = frg_stream_input_new_buffer(
        buffer,
        false
    );

    frg_parsing_source_t* source = frg_parsing_source_new(
        "--",
        stream
    );

    frg_parsing_range_t range = {
        .length = 5,
        .start = {
            .path = "--",
            .offset = 0,
            .lineno = 1,
            .columnno = 1
        }
    };

    GString* loaded = frg_parsing_source_load_range(
        source,
        &range
    );

    TEST_ASSERT_EQUAL_STRING("hello, world", loaded->str);
    
    frg_parsing_source_destroy(&source);
    g_string_free(loaded, TRUE);
}

// TODO: This needs more testing, but since it's not critical whatevs

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_buffer_load_at_beginning_of_first_line);
    return UNITY_END();
}
