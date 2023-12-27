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

#pragma once

#include <glib.h>
#include <forge/common/enums.h>

/// \brief The maximum number of option short names possible.
///
/// There are 26 for lowercase ASCII letters and 26 more for uppercase.
#define FRG_CLI_OPTION_SHORT_NAME_MAX_COUNT 52

#define FRG_CLI_OPTION_SHORT_NAME_NULL 0

typedef frg_status_t (*frg_cli_option_callback_t)(void* user_data, const char* value);

typedef struct {
    const char* name;
    const char* help;
} frg_cli_option_choice_t;

frg_status_t frg_cli_option_choice_new(
    frg_cli_option_choice_t** choice,
    const char* name,
    const char* help
);

frg_status_t frg_cli_option_choice_destroy(
    frg_cli_option_choice_t** choice
);

frg_status_t frg_cli_option_choice_print_help(
    const frg_cli_option_choice_t* choice
);

typedef struct {
    char short_name;
    const char* long_name;
    const char* value_name;
    const char* help;
    GList* choices;
    frg_cli_option_callback_t callback;
} frg_cli_option_t;

frg_status_t frg_cli_option_new_flag(
    frg_cli_option_t** option,
    const char* long_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_status_t frg_cli_option_new_flag_short(
    frg_cli_option_t** option,
    char short_name,
    const char* long_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_status_t frg_cli_option_new_argument(
    frg_cli_option_t** option,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_status_t frg_cli_option_new_argument_short(
    frg_cli_option_t** option,
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_status_t frg_cli_option_new_choice(
    frg_cli_option_t** option,
    const char* long_name,
    const char* value_name,
    const char* help,
    GList* choices,
    frg_cli_option_callback_t callback
);

frg_status_t frg_cli_option_new_choice_short(
    frg_cli_option_t** option,
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    GList* choices,
    frg_cli_option_callback_t callback
);

frg_status_t frg_cli_option_destroy(
    frg_cli_option_t** option
);

frg_status_t frg_cli_option_print_help(
    const frg_cli_option_t* option
);

typedef struct {
    GList* options;
    frg_cli_option_t* options_by_short_name[FRG_CLI_OPTION_SHORT_NAME_MAX_COUNT];
    GHashTable* options_by_long_name;
} frg_cli_option_set_t;

frg_status_t frg_cli_option_set_new(
    frg_cli_option_set_t** option_set
);

frg_status_t frg_cli_option_set_destroy(
    frg_cli_option_set_t** option_set
);

frg_status_t frg_cli_option_set_add(
    frg_cli_option_set_t* option_set,
    frg_cli_option_t* option
);

frg_status_t frg_cli_option_set_print_help(
    const frg_cli_option_set_t* option_set
);

bool frg_cli_option_set_parse_next(
    frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data
);

typedef frg_status_t (*frg_cli_command_callback_t)(int* exit_status, void* user_data, GList* pos_args);

typedef struct {
    const char* name;
    const char* help;
    frg_cli_option_set_t* option_set;
    frg_cli_command_callback_t callback;
} frg_cli_command_t;

frg_status_t frg_cli_command_new(
    frg_cli_command_t** command,
    const char* name,
    const char* help,
    frg_cli_command_callback_t callback
);

frg_status_t frg_cli_command_destroy(
    frg_cli_command_t** command
);

frg_status_t frg_cli_command_print_help(
    const frg_cli_command_t* command
);

typedef struct {
    const char* name;
    const char* binary_name;
    const char* pos_args_name;
    int version_major;
    int version_minor;
    int version_patch;
    const char* version_label;
    GString* version_details;
    frg_cli_option_set_t* global_options;
    GList* commands;
    GHashTable* commands_by_name;
} frg_cli_program_t;

frg_status_t frg_cli_program_new(
    frg_cli_program_t** program,
    const char* name,
    const char* binary_name,
    const char* pos_args_name,
    int version_major,
    int version_minor,
    int version_patch,
    const char* version_label,
    GString* version_details
);

frg_status_t frg_cli_program_destroy(
    frg_cli_program_t** program
);

frg_status_t frg_cli_program_add_command(
    frg_cli_program_t* program,
    frg_cli_command_t* command
);

frg_status_t frg_cli_program_print_help(
    const frg_cli_program_t* program,
    const char* command_name
);

frg_status_t frg_cli_program_print_version_long(
    const frg_cli_program_t* program
);

frg_status_t frg_cli_program_print_version_short(
    const frg_cli_program_t* program
);

bool frg_cli_program_parse(
    frg_cli_program_t* program,
    int argc,
    const char** argv,
    void* user_data
);
