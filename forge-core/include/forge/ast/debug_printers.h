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
#include <forge/ast/print_debug.h>

void frg_ast_debug_printer_type_int(frg_stream_output_t* mut_stream,
                                    const frg_ast_node_t* node,
                                    const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_type_float(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_type_symbol(frg_stream_output_t* mut_stream,
                                       const frg_ast_node_t* node,
                                       const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_type_pointer(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_type_array(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_type_function(frg_stream_output_t* mut_stream,
                                         const frg_ast_node_t* node,
                                         const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_declaration_union(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_declaration_structure(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_declaration_property(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_declaration_interface(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_declaration_function_argument(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_declaration_function(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_declaration_assignment(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_declaration_block(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_statement_return(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_statement_if_conditional_clause(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_statement_if(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_statement_while(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_statement_block(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_bool(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_int(frg_stream_output_t* mut_stream,
                                     const frg_ast_node_t* node,
                                     const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_float(frg_stream_output_t* mut_stream,
                                       const frg_ast_node_t* node,
                                       const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_character(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_string(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_array(frg_stream_output_t* mut_stream,
                                       const frg_ast_node_t* node,
                                       const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_array_repeated(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_structure(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_symbol(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_call_keyword_argument(
  frg_stream_output_t* mut_stream,
  const frg_ast_node_t* node,
  const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_call(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_cast(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_t* node,
                                      const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_unary(frg_stream_output_t* mut_stream,
                                       const frg_ast_node_t* node,
                                       const frg_ast_print_debug_options_t* options);

void frg_ast_debug_printer_value_binary(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options);
