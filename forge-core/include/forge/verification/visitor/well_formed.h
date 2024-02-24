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

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_symbol(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_pointer(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_function(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents);
