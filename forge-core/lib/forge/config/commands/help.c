// Copyright (c) 2023 Sophie Katz
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
#include <forge/config/config.h>
#include <forge/common/log.h>

frg_status_t _frg_config_commands_callback_help(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    if (pos_args == NULL) {
        return frg_cli_program_print_help((frg_cli_program_t*)program, NULL);
    } else {
        return frg_cli_program_print_help((frg_cli_program_t*)program, (const char*)pos_args->data);
    }
}

frg_status_t frg_config_commands_new_help(frg_cli_command_t** command) {
    frg_status_t result = frg_cli_command_new(
        command,
        "help",
        NULL,
        "Display help information. Use 'help <command>' for help with that command.",
        _frg_config_commands_callback_help
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}
