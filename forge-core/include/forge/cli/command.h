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
#include <forge/cli/option_set.h>

struct frg_cli_program_t;

typedef int (*frg_cli_command_callback_t)(
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
);

typedef struct {
    const char* name;
    const char* pos_args_name;
    const char* help;
    frg_cli_option_set_t* option_set;
    frg_cli_command_callback_t callback;
} frg_cli_command_t;

frg_cli_command_t* frg_cli_command_new(
    const char* name,
    const char* pos_args_name,
    const char* help,
    frg_cli_command_callback_t callback
);

void frg_cli_command_destroy(
    frg_cli_command_t** command
);

void frg_cli_command_print_help(
    const frg_cli_command_t* command
);
