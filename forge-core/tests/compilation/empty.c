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

void setUp() {
    frg_stream_output_init();
}

void tearDown() {
    frg_stream_output_cleanup();
}

void _callback_empty(
    const frg_message_buffer_t* message_buffer,
    void* mut_shared_library
) {
    TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
    TEST_ASSERT_EQUAL(0, message_buffer->error_count);
}

void test_empty() {
    frg_test_compilation(
        "empty",
        // Source
        "",
        // AST debug
        "[declaration-block]",
        // LLVM IR
        "; ModuleID = 'forge'\n"
        "source_filename = \"forge\"\n",
        // Callback
        _callback_empty
    );
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_empty);
    return UNITY_END();
}
