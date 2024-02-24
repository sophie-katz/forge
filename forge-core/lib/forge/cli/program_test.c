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

#include <forge/assert.h>
#include <forge/cli/program.h>
#include <unity.h>

int callback_command(frg_message_buffer_t* mut_message_buffer,
    void* mut_user_data,
    const frg_cli_program_t* program,
    const GList* positional_arguments
) {
    return 0;
}

void setUp() {}

void tearDown() {}

void test_get_command() {
    frg_cli_program_t* program = frg_cli_program_new(
        "Test",
        "test",
        NULL,
        0,
        1,
        0,
        NULL,
        NULL,
        NULL
    );

    frg_cli_program_add_command(
        program,
        frg_cli_command_new(
            "a",
            NULL,
            "A description of the command",
            callback_command
        )
    );

    frg_cli_program_add_command(
        program,
        frg_cli_command_new(
            "asdf",
            NULL,
            "A description of the command",
            callback_command
        )
    );

    TEST_ASSERT_NOT_NULL(frg_cli_program_get_command_by_name(program, "a"));
    TEST_ASSERT_NOT_NULL(frg_cli_program_get_command_by_name(program, "asdf"));
    TEST_ASSERT_NULL(frg_cli_program_get_command_by_name(program, "asd"));
    TEST_ASSERT_NULL(frg_cli_program_get_command_by_name(program, "b"));

    frg_cli_program_destroy(program);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_get_command);
    return UNITY_END();
}
