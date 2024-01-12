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

#pragma once

#include <forge/common/enums.h>
#include <forge/cli/choice.h>
#include <forge/messages/message_buffer.h>
#include <glib.h>

#define FRG_CLI_OPTION_SHORT_NAME_NULL 0

bool frg_cli_is_valid_short_name(char short_name);

typedef bool (*frg_cli_option_callback_t)(
    frg_message_buffer_t* message_buffer,
    void* user_data,
    const char* value
);

typedef struct {
    char short_name;
    const char* long_name;
    const char* value_name;
    const char* help;
    GList* choices;
    frg_cli_option_callback_t callback;
} frg_cli_option_t;

frg_cli_option_t* frg_cli_option_new_flag(
    const char* long_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_cli_option_t* frg_cli_option_new_flag_short(
    char short_name,
    const char* long_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_cli_option_t* frg_cli_option_new_argument(
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_cli_option_t* frg_cli_option_new_argument_short(
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_cli_option_t* frg_cli_option_new_choice(
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
);

frg_cli_option_t* frg_cli_option_new_choice_short(
    char short_name,
    const char* long_name,
    const char* value_name,
    const char* help,
    frg_cli_option_callback_t callback
);

void frg_cli_option_destroy(
    frg_cli_option_t** option
);

void frg_cli_option_add_choice(
    frg_cli_option_t* option,
    frg_cli_choice_t* choice
);

void frg_cli_option_print_help(
    const frg_cli_option_t* option
);

bool frg_cli_option_parse_next(
    frg_message_buffer_t* message_buffer,
    const frg_cli_option_t* option,
    int* argi,
    int argc,
    const char** argv,
    void* user_data
);
