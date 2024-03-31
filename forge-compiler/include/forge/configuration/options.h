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

#include <forge/enums.h>
#include <forge/messages/message_buffer.h>
#include <glib.h>

typedef struct {
  bool version_short;
  frg_message_severity_t minimum_message_severity;

  struct {
    bool print_ast;
    bool codegen;
    bool print_ir;
    bool write_object_file;
    const char* object_file_path;
  } compilation;
} frg_configuration_options_t;

frg_configuration_options_t* frg_configuration_options_new_default();

void frg_configuration_options_destroy(frg_configuration_options_t* options);

int frg_configuration_options_parse_cli(frg_message_buffer_t* mut_message_buffer,
                                        frg_configuration_options_t* mut_options,
                                        int argc,
                                        const char** argv);

bool frg_configuration_options_parse_shell_environment(
  frg_message_buffer_t* mut_message_buffer, frg_configuration_options_t* mut_options);

void frg_configuration_options_emit_message_debug(
  frg_message_buffer_t* mut_message_buffer, const frg_configuration_options_t* options);
