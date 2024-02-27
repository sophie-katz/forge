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

#include <forge/ast/visitor.h>

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_type_function(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_union(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_property(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_interface(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_function_argument(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_function(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_value_character(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_value_string(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_value_call_keyword_argument(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_value_call(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_operator(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

void frg_verification_supported_language_features_add_handlers(
  frg_ast_visitor_t* mut_visitor);
