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
#include <forge/ast/scope.h>

frg_ast_node_t* frg_verification_type_resolver_declaration_union(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_declaration_structure(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_declaration_property(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_declaration_interface(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_declaration_function_argument(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_declaration_function(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_declaration_variable(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_as_type_bool(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_int(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_float(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_character(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_string(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_symbol(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_dereference(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_get_address(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_call(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_access(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_unary_ident(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_binary_containing(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);

frg_ast_node_t* frg_verification_type_resolver_value_binary_left(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node);
