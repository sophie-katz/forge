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

#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/cli/command.h>

frg_cli_command_t* frg_cli_command_new(
    const char* name,
    const char* pos_args_name,
    const char* help,
    frg_cli_command_callback_t callback
) {
    frg_assert_string_non_empty(name);

    if (pos_args_name != NULL) {
        frg_assert_string_non_empty(pos_args_name);
    }

    frg_assert_string_non_empty(help);
    frg_assert_pointer_non_null(callback);

    frg_cli_command_t* command = frg_safe_malloc(sizeof(frg_cli_command_t));

    command->name = name;
    command->pos_args_name = pos_args_name;
    command->help = help;
    command->option_set = NULL;
    command->callback = callback;
    command->option_set = frg_cli_option_set_new();

    return command;
}

void frg_cli_command_destroy(
    frg_cli_command_t** command
) {
    frg_assert_pointer_non_null(command);
    frg_assert_pointer_non_null(*command);

    frg_cli_option_set_destroy(
        &(*command)->option_set
    );

    frg_safe_free((void**)command);
}

void frg_cli_command_print_help(
    frg_stream_output_t* stream,
    const frg_cli_command_t* command
) {
    frg_stream_output_write_printf(stream, "%s\n", command->help);
}
