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

#include <forge/ast/ast.h>
#include <forge/cli/program.h>
#include <forge/config/commands/link.h>
#include <forge/common/error.h>
#include <forge/config/config.h>

int _frg_config_commands_callback_version(
    frg_message_buffer_t* message_buffer,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    const frg_config_t* config = (const frg_config_t*)user_data;

    if (config->version_short) {
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

bool _frg_config_commands_option_callback_version_short(
    frg_message_buffer_t* message_buffer,
    void* user_data,
    const char* value
) {
    frg_config_t* config = (frg_config_t*)user_data;

    config->version_short = true;

    return true;
}

frg_cli_command_t* frg_config_commands_new_version() {
    frg_cli_command_t* command = frg_cli_command_new(
        "version",
        NULL,
        "Display version information.",
        _frg_config_commands_callback_version
    );

    frg_cli_option_set_add_option(
        command->option_set,
        frg_cli_option_new_flag(
            "short",
            "Use short format (<major>.<minor>.<patch>-<label>). This is intended for scripting use.",
            _frg_config_commands_option_callback_version_short
        )
    );

    return command;
}
