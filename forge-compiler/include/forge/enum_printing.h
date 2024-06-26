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
#include <forge/streams/output.h>

void frg_message_severity_print(frg_stream_output_t* mut_stream,
                                frg_message_severity_t severity);

void frg_ast_node_type_int_flags_print(frg_stream_output_t* mut_stream,
                                       frg_ast_node_type_int_flags_t flags);

void frg_ast_node_type_pointer_flags_print(frg_stream_output_t* mut_stream,
                                           frg_ast_node_type_pointer_flags_t flags);

void frg_ast_node_declaration_property_flags_print(
  frg_stream_output_t* mut_stream, frg_ast_node_declaration_property_flags_t flags);

void frg_ast_node_declaration_interface_flags_print(
  frg_stream_output_t* mut_stream, frg_ast_node_declaration_interface_flags_t flags);

void frg_ast_node_declaration_function_argument_flags_print(
  frg_stream_output_t* mut_stream,
  frg_ast_node_declaration_function_argument_flags_t flags);

void frg_ast_node_declaration_function_flags_print(
  frg_stream_output_t* mut_stream, frg_ast_node_declaration_function_flags_t flags);
