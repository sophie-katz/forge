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

#include <forge/ast/node.h>
#include <forge/cli/program.h>
#include <forge/configuration/commands/help.h>
#include <forge/configuration/cli_program.h>
#include <forge/assert.h>
#include <forge/configuration/cli_banner.h>

int _frg_configuration_command_callback_help(
    frg_message_buffer_t* mut_message_buffer,
    void* mut_user_data,
    const frg_cli_program_t* program,
    const GList* positional_arguments
) {
    frg_configuration_print_cli_banner();

    if (positional_arguments == NULL) {
        if (!frg_cli_program_try_print_help(
            frg_stream_output_get_stdout(),
            mut_message_buffer,
            (frg_cli_program_t*)program,
            NULL
        )) {
            return 1;
        }
    } else {
        if (!frg_cli_program_try_print_help(
            frg_stream_output_get_stdout(),
            mut_message_buffer,
            (frg_cli_program_t*)program,
            (const char*)positional_arguments->data
        )) {
            return 1;
        }
    }

    return 0;
}

frg_cli_command_t* frg_configuration_new_command_help() {
    return frg_cli_command_new(
        "help",
        NULL,
        "Display help information. Use 'help <command>' for help with that command.",
        _frg_configuration_command_callback_help
    );
}
