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
#include <forge/configuration/commands/version.h>
#include <forge/assert.h>
#include <forge/configuration/options.h>

int _frg_configuration_command_callback_version(
    frg_message_buffer_t* mut_message_buffer,
    void* mut_user_data,
    const frg_cli_program_t* program,
    const GList* positional_arguments
) {
    const frg_configuration_options_t* options = (const frg_configuration_options_t*)mut_user_data;

    if (options->version_short) {
        frg_cli_program_print_version_short(
            frg_stream_output_get_stdout(),
            (frg_cli_program_t*)program
        );
    } else {
        frg_cli_program_print_version_long(
            frg_stream_output_get_stdout(),
            (frg_cli_program_t*)program
        );
    }

    return 0;
}

bool _frg_configuration_option_callback_version_short(
    frg_message_buffer_t* mut_message_buffer,
    void* mut_user_data,
    const char* value
) {
    frg_configuration_options_t* options = (frg_configuration_options_t*)mut_user_data;

    options->version_short = true;

    return true;
}

frg_cli_command_t* frg_configuration_new_command_version() {
    frg_cli_command_t* command = frg_cli_command_new(
        "version",
        NULL,
        "Display version information.",
        _frg_configuration_command_callback_version
    );

    frg_cli_option_set_add_option(
        command->option_set,
        frg_cli_option_new_flag(
            "short",
            "Use short format (<major>.<minor>.<patch>-<label>). This is intended for scripting use.",
            _frg_configuration_option_callback_version_short
        )
    );

    return command;
}
