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

#include <forge/ast/node.h>
#include <forge/streams/output.h>

void frg_formatting_formatted_printer_type_void(frg_stream_output_t* mut_stream,
                                                const frg_ast_node_t* node,
                                                frg_indentation_t indentation);

void frg_formatting_formatted_printer_type_bool(frg_stream_output_t* mut_stream,
                                                const frg_ast_node_t* node,
                                                frg_indentation_t indentation);

void frg_formatting_formatted_printer_type_int(frg_stream_output_t* mut_stream,
                                               const frg_ast_node_t* node,
                                               frg_indentation_t indentation);

void frg_formatting_formatted_printer_type_float(frg_stream_output_t* mut_stream,
                                                 const frg_ast_node_t* node,
                                                 frg_indentation_t indentation);

void frg_formatting_formatted_printer_type_symbol(frg_stream_output_t* mut_stream,
                                                  const frg_ast_node_t* node,
                                                  frg_indentation_t indentation);

void frg_formatting_formatted_printer_type_pointer(frg_stream_output_t* mut_stream,
                                                   const frg_ast_node_t* node,
                                                   frg_indentation_t indentation);

void frg_formatting_formatted_printer_type_array(frg_stream_output_t* mut_stream,
                                                 const frg_ast_node_t* node,
                                                 frg_indentation_t indentation);

void frg_formatting_formatted_printer_type_function(frg_stream_output_t* mut_stream,
                                                    const frg_ast_node_t* node,
                                                    frg_indentation_t indentation);

void frg_formatting_formatted_printer_declaration_property(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  frg_indentation_t indentation);

void frg_formatting_formatted_printer_declaration_function_argument(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  frg_indentation_t indentation);
