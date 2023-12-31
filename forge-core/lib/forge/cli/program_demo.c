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

#include <forge/cli/program.h>
#include <forge/common/log.h>

frg_status_t callback_command(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    return FRG_STATUS_OK;
}

frg_status_t callback_option(void* user_data, const char* value) {
    return FRG_STATUS_OK;
}

int no_commands(void) {
    frg_cli_program_t* program = NULL;
    frg_status_t result = frg_cli_program_new(
        &program,
        "Forge",
        "forge",
        "source files",
        0,
        1,
        0,
        "alpha",
        g_string_new("Some more version\ninformation"),
        NULL
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create program: %s", frg_status_to_string(result));
        return 1;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(&option, "debug", "Enable debug mode", callback_option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(program->global_options, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to global option set: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_print_help(program, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print program help: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_destroy(&program);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy program: %s", frg_status_to_string(result));
        return 1;
    }

    return 0;
}

int one_command_global(void) {
    frg_cli_program_t* program = NULL;
    frg_status_t result = frg_cli_program_new(
        &program,
        "Forge",
        "forge",
        NULL,
        0,
        1,
        0,
        "alpha",
        g_string_new("Some more version\ninformation"),
        NULL
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create program: %s", frg_status_to_string(result));
        return 1;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(&option, "debug", "Enable debug mode", callback_option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(program->global_options, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to global option set: %s", frg_status_to_string(result));
        return 1;
    }

    frg_cli_command_t* command = NULL;
    result = frg_cli_command_new(
        &command,
        "command",
        NULL,
        "A command",
        callback_command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create command: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_flag(&option, "option", "An option", callback_option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(command->option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_add_command(program, command);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add command to program: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_print_help(program, NULL);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print program help: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_destroy(&program);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy program: %s", frg_status_to_string(result));
        return 1;
    }

    return 0;
}

int one_command_command(void) {
    frg_cli_program_t* program = NULL;
    frg_status_t result = frg_cli_program_new(
        &program,
        "Forge",
        "forge",
        NULL,
        0,
        1,
        0,
        "alpha",
        g_string_new("Some more version\ninformation"),
        NULL
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create program: %s", frg_status_to_string(result));
        return 1;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(&option, "debug", "Enable debug mode", callback_option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(program->global_options, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to global option set: %s", frg_status_to_string(result));
        return 1;
    }

    frg_cli_command_t* command = NULL;
    result = frg_cli_command_new(
        &command,
        "command",
        "positional argument",
        "A command",
        callback_command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create command: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_flag(&option, "option", "An option", callback_option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(command->option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_add_command(program, command);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add command to program: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_print_help(program, "command");
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print program help: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_destroy(&program);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy program: %s", frg_status_to_string(result));
        return 1;
    }

    return 0;
}

int main(void) {
    printf("=== NO COMMANDS ===\n\n");
    
    int result = no_commands();
    if (result != 0) {
        return result;
    }

    printf("\n\n=== ONE COMMAND (GLOBAL) ===\n\n");
    
    result = one_command_global();
    if (result != 0) {
        return result;
    }

    printf("\n\n=== ONE COMMAND (COMMAND) ===\n\n");
    
    result = one_command_command();
    if (result != 0) {
        return result;
    }
    
    return 0;
}
