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

#include <glib.h>
#include <forge/common/enums.h>
#include <forge/messages/message_buffer.h>
#include <forge/cli/option.h>

/// \brief The maximum number of option short names possible.
///
/// There are 26 for lowercase ASCII letters and 26 more for uppercase.
#define FRG_CLI_OPTION_SHORT_NAME_MAX_COUNT 52

typedef struct {
    GList* options;
    frg_cli_option_t* options_by_short_name[FRG_CLI_OPTION_SHORT_NAME_MAX_COUNT];
    GHashTable* options_by_long_name;
} frg_cli_option_set_t;

frg_cli_option_set_t* frg_cli_option_set_new();

void frg_cli_option_set_destroy(
    frg_cli_option_set_t** option_set
);

void frg_cli_option_set_add_option(
    frg_cli_option_set_t* option_set,
    frg_cli_option_t* option
);

frg_cli_option_t* frg_cli_option_set_get_option_by_long_name(
    frg_cli_option_set_t* option_set,
    const char* name
);

frg_cli_option_t* frg_cli_option_set_get_option_by_short_name(
    frg_cli_option_set_t* option_set,
    char name
);

void frg_cli_option_set_print_help(
    const frg_cli_option_set_t* option_set
);

bool frg_cli_option_set_parse_next(
    frg_message_buffer_t* message_buffer,
    const frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data
);
