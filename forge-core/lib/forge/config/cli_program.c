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
#include <forge/common/check.h>
#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/config/commands/compile.h>
#include <forge/config/commands/dump_ast.h>
#include <forge/config/commands/dump_ir.h>
#include <forge/config/commands/help.h>
#include <forge/config/commands/link.h>
#include <forge/config/commands/version.h>

void frg_config_cli_program_banner(void) {
    frg_set_color(stdout, FRG_COLOR_ID_BOLD);
    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_YELLOW);
    printf("\n       \\ | / ,\n");
    printf("     `\n");
    frg_set_color(stdout, FRG_COLOR_ID_BRIGHT_RED);
    printf("---------------\n");
    printf("  ----.      /\n");
    printf("      /___^__\\\n\n");
    frg_set_color(stdout, FRG_COLOR_ID_RESET);
}

frg_status_t _frg_config_cli_program_callback(
    int* exit_status,
    const frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    frg_config_cli_program_banner();

    return frg_cli_program_print_help(program, NULL);
}

frg_status_t _frg_config_cli_option_callback_debug(void* user_data, const char* value) {
    frg_log_set_minimum_severity(FRG_LOG_SEVERITY_DEBUG);

    return FRG_STATUS_OK;
}

frg_status_t _frg_config_cli_option_callback_trace(void* user_data, const char* value) {
    frg_log_set_minimum_severity(FRG_LOG_SEVERITY_TRACE);

    return FRG_STATUS_OK;
}

frg_status_t _frg_config_cli_option_callback_color_mode(void* user_data, const char* value) {
    frg_status_t result;

    if (strcmp(value, "disabled") == 0) {
        result = frg_set_color_mode(FRG_COLOR_MODE_DISABLED);
    } else if (strcmp(value, "auto") == 0) {
        result = frg_set_color_mode(FRG_COLOR_MODE_AUTO);
    } else if (strcmp(value, "enabled") == 0) {
        result = frg_set_color_mode(FRG_COLOR_MODE_ENABLED);
    } else {
        frg_log_fatal_error("unknown color mode: %s", value);
        return FRG_STATUS_CLI_ERROR;
    }

    frg_check(result);

    return FRG_STATUS_OK;
}

frg_status_t frg_config_cli_program_new(frg_cli_program_t** program) {
    GString* version_details = g_string_new(NULL);
    g_string_append_printf(version_details, "Git commit: %s\n", FRG_GIT_COMMIT_SHA);
    g_string_append_printf(version_details, "Compiled with: %s %s\n", FRG_COMPILER_ID, FRG_COMPILER_VERSION);
    g_string_append_printf(version_details, "Linked with: %s\n", FRG_LINKER_ID);
    g_string_append_printf(version_details, "Built for: %s (%s)", FRG_HOST_MACHINE_SYSTEM, FRG_HOST_MACHINE_CPU);

    frg_check(
        frg_cli_program_new(
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
        )
    );

    frg_cli_option_t* option = NULL;
    frg_check(
        frg_cli_option_new_choice(
            &option,
            "color-mode",
            "mode",
            "Set the color mode",
            _frg_config_cli_option_callback_color_mode
        )
    );

    frg_cli_choice_t* choice = NULL;
    frg_check(
        frg_cli_choice_new(
            &choice,
            "disabled",
            "Disable color output"
        )
    );

    frg_check(
        frg_cli_option_add_choice(
            option,
            choice
        )
    );

    choice = NULL;
    frg_check(
        frg_cli_choice_new(
            &choice,
            "auto",
            "Automatically detect whether or not to use color output"
        )
    );

    frg_check(
        frg_cli_option_add_choice(
            option,
            choice
        )
    );
    
    choice = NULL;
    frg_check(
        frg_cli_choice_new(
            &choice,
            "enabled",
            "Force color output to be enabled"
        )
    );

    frg_check(
        frg_cli_option_add_choice(
            option,
            choice
        )
    );

    frg_check(
        frg_cli_option_set_add_option(
            (*program)->global_options,
            option
        )
    );

    option = NULL;
    frg_check(
        frg_cli_option_new_flag(
            &option,
            "debug",
            "Enable debug logging",
            _frg_config_cli_option_callback_debug
        )
    );

    frg_check(
        frg_cli_option_set_add_option(
            (*program)->global_options,
            option
        )
    );

    option = NULL;
    frg_check(
        frg_cli_option_new_flag(
            &option,
            "trace",
            "Enable trace logging",
            _frg_config_cli_option_callback_trace
        )
    );

    frg_check(
        frg_cli_option_set_add_option(
            (*program)->global_options,
            option
        )
    );

    frg_cli_command_t* command = NULL;
    frg_check(
        frg_config_commands_new_compile(&command)
    );

    frg_check(
        frg_cli_program_add_command(
            *program,
            command
        )
    );

    command = NULL;
    frg_check(
        frg_config_commands_new_dump_ast(&command)
    );

    frg_check(
        frg_cli_program_add_command(
            *program,
            command
        )
    );

    command = NULL;
    frg_check(
        frg_config_commands_new_dump_ir(&command)
    );

    frg_check(
        frg_cli_program_add_command(
            *program,
            command
        )
    );

    command = NULL;
    frg_check(
        frg_config_commands_new_help(&command)
    );

    frg_check(
        frg_cli_program_add_command(
            *program,
            command
        )
    );

    command = NULL;
    frg_check(
        frg_config_commands_new_link(&command)
    );

    frg_check(
        frg_cli_program_add_command(
            *program,
            command
        )
    );

    command = NULL;
    frg_check(
        frg_config_commands_new_version(&command)
    );

    frg_check(
        frg_cli_program_add_command(
            *program,
            command
        )
    );

    return FRG_STATUS_OK;
}