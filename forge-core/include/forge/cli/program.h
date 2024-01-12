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
#include <forge/cli/command.h>

typedef struct {
    const char* name;
    const char* binary_name;
    const char* pos_args_name;
    int version_major;
    int version_minor;
    int version_patch;
    const char* version_label;
    GString* version_details;
    void* callback;
    frg_cli_option_set_t* global_options;
    GList* commands;
    GHashTable* commands_by_name;
} frg_cli_program_t;

typedef int (*frg_cli_program_callback_t)(
    frg_message_buffer_t* message_buffer,
    const frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
);

frg_cli_program_t* frg_cli_program_new(
    const char* name,
    const char* binary_name,
    const char* pos_args_name,
    int version_major,
    int version_minor,
    int version_patch,
    const char* version_label,
    GString* version_details,
    frg_cli_program_callback_t callback
);

void frg_cli_program_destroy(frg_cli_program_t** program);

void frg_cli_program_add_command(
    frg_cli_program_t* program,
    frg_cli_command_t* command
);

frg_cli_command_t* frg_cli_program_get_command_by_name(
    frg_cli_program_t* program,
    const char* name
);

bool frg_cli_program_try_print_help(
    frg_message_buffer_t* message_buffer,
    const frg_cli_program_t* program,
    const char* command_name
);

void frg_cli_program_print_version_long(
    const frg_cli_program_t* program
);

void frg_cli_program_print_version_short(
    const frg_cli_program_t* program
);

int frg_cli_program_parse(
    frg_message_buffer_t* message_buffer,
    const frg_cli_program_t* program,
    int argc,
    const char** argv,
    void* user_data
);
