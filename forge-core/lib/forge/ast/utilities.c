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

#include <forge/assert.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>
#include <forge/debug.h>

frg_ast_node_t* frg_ast_clone(const frg_ast_node_t* node) {
  if (node == NULL) {
    return NULL;
  }

  return frg_ast_node_kind_info_get(node->kind)->_cloner(node);
}

const char* frg_ast_declaration_name_get(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_DECLARATION);

  frg_ast_declaration_name_getter_t declaration_name_getter
    = frg_ast_node_kind_info_get(node->kind)->_declaration_name_getter;

  frg_assert_pointer_non_null(declaration_name_getter);

  return declaration_name_getter(node);
}

const char* frg_ast_declaration_function_argument_name_get(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);

  return ((const frg_ast_node_declaration_property_t*)(((const frg_ast_node_declaration_function_argument_t*)
                                                          node)
                                                         ->property))
    ->name->str;
}
