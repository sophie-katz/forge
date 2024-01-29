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

#include <unity.h>
#include <forge/streams/output.h>

#include "test.h"

void setUp(void) {
    frg_stream_output_init();
}

void tearDown(void) {
    frg_stream_output_cleanup();
}

void _callback_empty(
    const frg_message_buffer_t* message_buffer,
    void* shared_library
) {
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_warning_count(message_buffer));
    TEST_ASSERT_EQUAL(0, frg_message_buffer_get_error_count(message_buffer));
}

void test_empty(void) {
    frg_test_compilation(
        "empty",
        // Source
        "",
        // AST debug
        "[decl-block]",
        // LLVM IR
        "; ModuleID = 'forge'\n"
        "source_filename = \"forge\"\n",
        // Callback
        _callback_empty
    );
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty);
    return UNITY_END();
}
