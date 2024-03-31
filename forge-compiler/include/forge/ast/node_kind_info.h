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
#include <forge/ast/print_debug.h>
#include <forge/ast/scope.h>
#include <forge/ast/visitor.h>

typedef void (*frg_ast_destructor_t)(frg_ast_node_t* node);

typedef frg_ast_node_t* (*frg_ast_cloner_t)(const frg_ast_node_t* node);

typedef bool (*frg_ast_comparer_t)(const frg_ast_node_t* a, const frg_ast_node_t* b);

typedef const char* (*frg_ast_declaration_name_getter_t)(const frg_ast_node_t* node);

typedef void (*frg_ast_debug_printer_t)(frg_stream_output_t* mut_stream,
                                        const frg_ast_node_t* node,
                                        const frg_ast_print_debug_options_t* options);

typedef frg_ast_visitor_status_t (*frg_ast_visitor_acceptor_t)(
  frg_ast_node_t** mut_node, const frg_ast_visitor_t* visitor, const GList* parents);

typedef void (*frg_ast_formatting_formatted_printer_t)(frg_stream_output_t* mut_stream,
                                                       const frg_ast_node_t* node,
                                                       frg_indentation_t indentation);

typedef frg_ast_node_t* (*frg_ast_type_resolver_t)(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

typedef struct {
  const char* name;
  frg_ast_node_kind_flags_t flags;
  const char* operator_symbol;
  frg_ast_destructor_t _destructor;
  frg_ast_cloner_t _cloner;
  frg_ast_comparer_t _comparer;
  frg_ast_declaration_name_getter_t _declaration_name_getter;
  frg_ast_debug_printer_t _debug_printer;
  frg_ast_formatting_formatted_printer_t _formatted_printer;
  frg_ast_visitor_acceptor_t _visitor_acceptor;
  frg_ast_type_resolver_t _type_resolver;
} frg_ast_node_kind_info_t;

const frg_ast_node_kind_info_t* frg_ast_node_kind_info_get(frg_ast_node_kind_t kind);
