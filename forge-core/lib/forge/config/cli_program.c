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

#include <forge/config/cli_program.h>
#include <forge/config/config.h>
#include <build_config.h>
#include <forge/common/log.h>
#include <forge/config/commands/compile.h>
#include <forge/config/commands/dump_ast.h>
#include <forge/config/commands/help.h>
#include <forge/config/commands/link.h>
#include <forge/config/commands/version.h>

frg_status_t _frg_config_cli_program_callback(
    int* exit_status,
    const frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    return frg_cli_program_print_help(program, NULL);
}

frg_status_t _frg_config_cli_option_callback_debug(void* user_data, const char* value) {
    frg_log_set_minimum_severity(FRG_LOG_SEVERITY_DEBUG);

    return FRG_STATUS_OK;
}

frg_status_t frg_config_cli_program_new(frg_cli_program_t** program) {
    GString* version_details = g_string_new(NULL);
    g_string_append_printf(version_details, "Git commit: %s\n", FRG_GIT_COMMIT_SHA);
    g_string_append_printf(version_details, "Compiled with: %s %s\n", FRG_COMPILER_ID, FRG_COMPILER_VERSION);
    g_string_append_printf(version_details, "Linked with: %s\n", FRG_LINKER_ID);
    g_string_append_printf(version_details, "Built for: %s (%s)", FRG_HOST_MACHINE_SYSTEM, FRG_HOST_MACHINE_CPU);

    frg_status_t result = frg_cli_program_new(
        program,
        "Forge",
        "forge",
        NULL,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        FRG_VERSION_LABEL,
        version_details,
        _frg_config_cli_program_callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI program: %s", frg_status_to_string(result));
        return result;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(
        &option,
        "debug",
        "Enable debug logging",
        _frg_config_cli_option_callback_debug
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI option: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_option_set_add_option(
        (*program)->global_options,
        option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI option to program: %s", frg_status_to_string(result));
        return result;
    }

    frg_cli_command_t* command = NULL;
    result = frg_config_commands_new_compile(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_add_command(
        *program,
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI command to program: %s", frg_status_to_string(result));
        return result;
    }

    command = NULL;
    result = frg_config_commands_new_dump_ast(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_add_command(
        *program,
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI command to program: %s", frg_status_to_string(result));
        return result;
    }

    command = NULL;
    result = frg_config_commands_new_help(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_add_command(
        *program,
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI command to program: %s", frg_status_to_string(result));
        return result;
    }

    command = NULL;
    result = frg_config_commands_new_link(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_add_command(
        *program,
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI command to program: %s", frg_status_to_string(result));
        return result;
    }

    command = NULL;
    result = frg_config_commands_new_version(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_add_command(
        *program,
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI command to program: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}