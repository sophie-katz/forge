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
#include <forge/ast/declaration_name_getters.h>
#include <forge/memory.h>

const char* frg_ast_declaration_name_getter_union(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_UNION);

  return ((const frg_ast_node_declaration_union_t*)node)->name->str;
}

const char* frg_ast_declaration_name_getter_structure(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_STRUCTURE);

  return ((const frg_ast_node_declaration_structure_t*)node)->name->str;
}

const char* frg_ast_declaration_name_getter_property(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);

  return ((const frg_ast_node_declaration_property_t*)node)->name->str;
}

const char* frg_ast_declaration_name_getter_interface(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_INTERFACE);

  return ((const frg_ast_node_declaration_interface_t*)node)->name->str;
}

const char* frg_ast_declaration_name_getter_function(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);

  return ((const frg_ast_node_declaration_function_t*)node)->name->str;
}

const char* frg_ast_declaration_name_getter_variable(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_VARIABLE);

  return ((const frg_ast_node_declaration_property_t*)(((const frg_ast_node_declaration_variable_t*)
                                                          node)
                                                         ->property))
    ->name->str;
}
