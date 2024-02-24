// Copyright (c) 2023-2024 Sophie Katz
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

#include <forge/cli/command.h>
#include <forge/cli/program_forward.h>
#include <forge/enums.h>
#include <forge/streams/output.h>
#include <glib.h>

frg_cli_program_t* frg_cli_program_new(const char* name,
                                       const char* binary_name,
                                       const char* positional_arguments_name,
                                       int version_major,
                                       int version_minor,
                                       int version_patch,
                                       const char* version_label,
                                       GString* version_details,
                                       frg_cli_program_callback_t callback);

void frg_cli_program_destroy(frg_cli_program_t* program);

void frg_cli_program_add_command(frg_cli_program_t* mut_program,
                                 frg_cli_command_t* command);

frg_cli_command_t* frg_cli_program_get_command_by_name(frg_cli_program_t* mut_program,
                                                       const char* name);

bool frg_cli_program_try_print_help(frg_stream_output_t* mut_stream,
                                    frg_message_buffer_t* mut_message_buffer,
                                    const frg_cli_program_t* program,
                                    const char* command_name);

void frg_cli_program_print_version_long(frg_stream_output_t* mut_stream,
                                        const frg_cli_program_t* program);

void frg_cli_program_print_version_short(frg_stream_output_t* mut_stream,
                                         const frg_cli_program_t* program);

int frg_cli_program_parse(frg_message_buffer_t* mut_message_buffer,
                          void* mut_user_data,
                          const frg_cli_program_t* program,
                          int argc,
                          const char** argv);
