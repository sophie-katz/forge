// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#pragma once

#include <forge/cli/option_set.h>
#include <forge/enums.h>
#include <forge/streams/output.h>
#include <glib.h>

typedef struct {
  const char* name;
  const char* binary_name;
  const char* positional_arguments_name;
  int version_major;
  int version_minor;
  int version_patch;
  const char* version_label;
  GString* version_details;
  void* _callback;
  frg_cli_option_set_t* global_options;
  GList* _commands;
  GHashTable* _commands_index_by_name;
} frg_cli_program_t;

typedef int (*frg_cli_program_callback_t)(frg_message_buffer_t* mut_message_buffer,
                                          void* mut_user_data,
                                          const frg_cli_program_t* program,
                                          const GList* positional_arguments);
