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

#include <forge/ast/ast.h>
#include <forge/cli/program.h>
#include <forge/config/commands/link.h>
#include <forge/config/cli_program.h>
#include <forge/common/error.h>
#include <forge/config/config.h>

int _frg_config_commands_callback_help(
    frg_message_buffer_t* message_buffer,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    frg_config_cli_program_banner();

    if (pos_args == NULL) {
        if (!frg_cli_program_try_print_help(
            message_buffer,
            (frg_cli_program_t*)program,
            NULL
        )) {
            return 1;
        }
    } else {
        if (!frg_cli_program_try_print_help(
            message_buffer,
            (frg_cli_program_t*)program,
            (const char*)pos_args->data
        )) {
            return 1;
        }
    }

    return 0;
}

frg_cli_command_t* frg_config_commands_new_help() {
    return frg_cli_command_new(
        "help",
        NULL,
        "Display help information. Use 'help <command>' for help with that command.",
        _frg_config_commands_callback_help
    );
}
