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
#include <forge/ast/destructors.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>
#include <forge/memory.h>

void _frg_ast_destroy_glist(GList* list) {
  if (list == NULL) {
    return;
  }

  for (GList* iter = list; iter != NULL; iter = iter->next) {
    frg_ast_node_destroy((frg_ast_node_t*)iter->data);
  }

  g_list_free(list);
}

void frg_ast_destructor_type_symbol(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);

  if (((frg_ast_node_type_symbol_t*)node)->name != NULL) {
    g_string_free(((frg_ast_node_type_symbol_t*)node)->name, TRUE);
  }
}

void frg_ast_destructor_type_pointer(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_POINTER);

  if (((frg_ast_node_type_pointer_t*)node)->value != NULL) {
    frg_ast_node_destroy((frg_ast_node_t*)((frg_ast_node_type_pointer_t*)node)->value);
  }
}

void frg_ast_destructor_type_function(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);

  _frg_ast_destroy_glist(((frg_ast_node_type_function_t*)node)->arguments);

  if (((frg_ast_node_type_function_t*)node)->variadic_positional_arguments != NULL) {
    frg_ast_node_destroy((frg_ast_node_t*)((frg_ast_node_type_function_t*)node)
                           ->variadic_positional_arguments);
  }

  if (((frg_ast_node_type_function_t*)node)->variadic_keyword_arguments != NULL) {
    frg_ast_node_destroy((frg_ast_node_t*)((frg_ast_node_type_function_t*)node)
                           ->variadic_keyword_arguments);
  }

  if (((frg_ast_node_type_function_t*)node)->return_type != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_type_function_t*)node)->return_type);
  }
}

void frg_ast_destructor_declaration_union(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_UNION);

  if (((frg_ast_node_declaration_union_t*)node)->name != NULL) {
    g_string_free(((frg_ast_node_declaration_union_t*)node)->name, TRUE);
  }

  _frg_ast_destroy_glist(((frg_ast_node_declaration_union_t*)node)->properties);
}

void frg_ast_destructor_declaration_structure(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_STRUCTURE);

  if (((frg_ast_node_declaration_structure_t*)node)->name != NULL) {
    g_string_free(((frg_ast_node_declaration_structure_t*)node)->name, TRUE);
  }

  _frg_ast_destroy_glist(((frg_ast_node_declaration_structure_t*)node)->declarations);
}

void frg_ast_destructor_declaration_property(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);

  if (((frg_ast_node_declaration_property_t*)node)->name != NULL) {
    g_string_free(((frg_ast_node_declaration_property_t*)node)->name, TRUE);
  }

  if (((frg_ast_node_declaration_property_t*)node)->type != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_declaration_property_t*)node)->type);
  }
}

void frg_ast_destructor_declaration_interface(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_INTERFACE);

  if (((frg_ast_node_declaration_interface_t*)node)->name != NULL) {
    g_string_free(((frg_ast_node_declaration_interface_t*)node)->name, TRUE);
  }

  _frg_ast_destroy_glist(((frg_ast_node_declaration_interface_t*)node)->extends);
  _frg_ast_destroy_glist(((frg_ast_node_declaration_interface_t*)node)->declarations);
}

void frg_ast_destructor_declaration_function_argument(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);

  if (((frg_ast_node_declaration_function_argument_t*)node)->property != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_declaration_function_argument_t*)node)->property);
  }

  if (((frg_ast_node_declaration_function_argument_t*)node)->default_value != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_declaration_function_argument_t*)node)
        ->default_value);
  }
}

void frg_ast_destructor_declaration_function(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);

  if (((frg_ast_node_declaration_function_t*)node)->name != NULL) {
    g_string_free(((frg_ast_node_declaration_function_t*)node)->name, TRUE);
  }

  if (((frg_ast_node_declaration_function_t*)node)->type != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_declaration_function_t*)node)->type);
  }

  if (((frg_ast_node_declaration_function_t*)node)->body != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_declaration_function_t*)node)->body);
  }
}

void frg_ast_destructor_declaration_variable(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_VARIABLE);

  if (((frg_ast_node_declaration_variable_t*)node)->property != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_declaration_variable_t*)node)->property);
  }

  if (((frg_ast_node_declaration_variable_t*)node)->initial_value != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_declaration_variable_t*)node)->initial_value);
  }
}

void frg_ast_destructor_declaration_block(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_BLOCK);

  _frg_ast_destroy_glist(((frg_ast_node_declaration_block_t*)node)->declarations);
}

void frg_ast_destructor_statement_return(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_RETURN);

  if (((frg_ast_node_statement_return_t*)node)->value != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_statement_return_t*)node)->value);
  }
}

void frg_ast_destructor_statement_if(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_IF);

  if (((frg_ast_node_statement_if_t*)node)->condition != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_statement_if_t*)node)->condition);
  }

  if (((frg_ast_node_statement_if_t*)node)->then_clause != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_statement_if_t*)node)->then_clause);
  }

  if (((frg_ast_node_statement_if_t*)node)->else_clause != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_statement_if_t*)node)->else_clause);
  }
}

void frg_ast_destructor_statement_while(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_WHILE);

  if (((frg_ast_node_statement_while_t*)node)->condition != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_statement_while_t*)node)->condition);
  }

  if (((frg_ast_node_statement_while_t*)node)->body != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_statement_while_t*)node)->body);
  }
}

void frg_ast_destructor_statement_block(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_BLOCK);

  _frg_ast_destroy_glist(((frg_ast_node_statement_block_t*)node)->statements);
}

void frg_ast_destructor_value_string(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_STRING);

  if (((frg_ast_node_value_string_t*)node)->value != NULL) {
    g_string_free(((frg_ast_node_value_string_t*)node)->value, TRUE);
  }
}

void frg_ast_destructor_value_symbol(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_SYMBOL);

  if (((frg_ast_node_value_symbol_t*)node)->name != NULL) {
    g_string_free(((frg_ast_node_value_symbol_t*)node)->name, TRUE);
  }
}

void frg_ast_destructor_value_call_keyword_argument(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT);

  if (((frg_ast_node_value_call_keyword_argument_t*)node)->name != NULL) {
    g_string_free(((frg_ast_node_value_call_keyword_argument_t*)node)->name, TRUE);
  }

  if (((frg_ast_node_value_call_keyword_argument_t*)node)->value != NULL) {
    frg_ast_node_destroy(
      (frg_ast_node_t*)((frg_ast_node_value_call_keyword_argument_t*)node)->value);
  }
}

void frg_ast_destructor_value_call(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL);

  if (((frg_ast_node_value_call_t*)node)->callee != NULL) {
    frg_ast_node_destroy((frg_ast_node_t*)((frg_ast_node_value_call_t*)node)->callee);
  }

  _frg_ast_destroy_glist(((frg_ast_node_value_call_t*)node)->arguments);
  _frg_ast_destroy_glist(((frg_ast_node_value_call_t*)node)->keyword_arguments);
}

void frg_ast_destructor_value_unary(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY);

  if (((frg_ast_node_value_unary_t*)node)->operand != NULL) {
    frg_ast_node_destroy((frg_ast_node_t*)((frg_ast_node_value_unary_t*)node)->operand);
  }
}

void frg_ast_destructor_value_binary(frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY);

  if (((frg_ast_node_value_binary_t*)node)->left != NULL) {
    frg_ast_node_destroy((frg_ast_node_t*)((frg_ast_node_value_binary_t*)node)->left);
  }

  if (((frg_ast_node_value_binary_t*)node)->right != NULL) {
    frg_ast_node_destroy((frg_ast_node_t*)((frg_ast_node_value_binary_t*)node)->right);
  }
}
