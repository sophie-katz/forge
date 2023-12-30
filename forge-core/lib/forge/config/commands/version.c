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
#include <forge/common/log.h>
#include <forge/config/config.h>

frg_status_t _frg_config_commands_callback_version(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    const frg_config_t* config = (const frg_config_t*)user_data;

    if (config->version_short) {
        return frg_cli_program_print_version_short((frg_cli_program_t*)program);
    } else {
        return frg_cli_program_print_version_long((frg_cli_program_t*)program);
    }
}

frg_status_t _frg_config_commands_option_callback_version_short(
    void* user_data,
    const char* value
) {
    frg_config_t* config = (frg_config_t*)user_data;
    config->version_short = true;

    return FRG_STATUS_OK;
}

frg_status_t frg_config_commands_new_version(frg_cli_command_t** command) {
    frg_status_t result = frg_cli_command_new(
        command,
        "version",
        NULL,
        "Display version information.",
        _frg_config_commands_callback_version
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(
        &option,
        "short",
        "Use short format (<major>.<minor>.<patch>-<label>). This is intended for scripting use.",
        _frg_config_commands_option_callback_version_short
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI option: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_option_set_add_option(
        (*command)->option_set,
        option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI option to command: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}
