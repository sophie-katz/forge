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

#include <forge/ast/node.h>
#include <forge/ast/visitor.h>

frg_ast_node_t* frg_ast_clone(const frg_ast_node_t* node);

bool frg_ast_compare(const frg_ast_node_t* a, const frg_ast_node_t* b);

const char* frg_ast_declaration_name_get(const frg_ast_node_t* node);

const char* frg_ast_declaration_function_argument_name_get(const frg_ast_node_t* node);

void frg_ast_type_int_print(frg_stream_output_t* mut_stream,
                            const frg_ast_node_type_int_t* node);

void frg_ast_value_int_print(frg_stream_output_t* mut_stream,
                             const frg_ast_node_value_int_t* node,
                             frg_base_t base,
                             frg_column_number_t separate_every_n);

void frg_ast_value_int_print_signed(frg_stream_output_t* mut_stream,
                                    const frg_ast_node_value_int_t* node,
                                    frg_base_t base,
                                    frg_column_number_t separate_every_n);

void frg_ast_value_int_print_unsigned(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_value_int_t* node,
                                      frg_base_t base,
                                      frg_column_number_t separate_every_n);

void frg_ast_type_float_print(frg_stream_output_t* mut_stream,
                              const frg_ast_node_type_float_t* node);

void frg_ast_value_float_print(frg_stream_output_t* mut_stream,
                               const frg_ast_node_value_float_t* node,
                               frg_base_t base,
                               frg_column_number_t separate_every_n);

frg_int_attributes_case_t frg_get_case_for_type_int(
  const frg_ast_node_type_int_t* type);

const frg_ast_node_t* frg_ast_get_surrounding_node_with_kind(const GList* parents,
                                                             frg_ast_node_kind_t kind);
