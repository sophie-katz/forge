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

#include <forge/cli/program.h>
#include <forge/common/log.h>

int callback_command(
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    return 0;
}

bool callback_option(void* user_data, const char* value) {
    return true;
}

int no_commands(void) {
    frg_cli_program_t* program = frg_cli_program_new(
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

    frg_cli_option_t* option = frg_cli_option_new_flag("debug", "Enable debug mode", callback_option);

    frg_cli_option_set_add_option(program->global_options, option);

    frg_cli_program_print_help(program, NULL);

    frg_cli_program_destroy(&program);

    return 0;
}

int one_command_global(void) {
    frg_cli_program_t* program = frg_cli_program_new(
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

    frg_cli_option_t* option = frg_cli_option_new_flag("debug", "Enable debug mode", callback_option);

    frg_cli_option_set_add_option(program->global_options, option);

    frg_cli_command_t* command = frg_cli_command_new(
        "command",
        NULL,
        "A command",
        callback_command
    );

    option = frg_cli_option_new_flag("option", "An option", callback_option);

    frg_cli_option_set_add_option(command->option_set, option);

    frg_cli_program_add_command(program, command);

    frg_cli_program_print_help(program, NULL);

    frg_cli_program_destroy(&program);

    return 0;
}

int one_command_command(void) {
    frg_cli_program_t* program = frg_cli_program_new(
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

    frg_cli_option_t* option = frg_cli_option_new_flag("debug", "Enable debug mode", callback_option);

    frg_cli_option_set_add_option(program->global_options, option);

    frg_cli_command_t* command = frg_cli_command_new(
        "command",
        "positional argument",
        "A command",
        callback_command
    );

    option = frg_cli_option_new_flag("option", "An option", callback_option);

    frg_cli_option_set_add_option(command->option_set, option);

    frg_cli_program_add_command(program, command);

    frg_cli_program_print_help(program, "command");

    frg_cli_program_destroy(&program);

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
