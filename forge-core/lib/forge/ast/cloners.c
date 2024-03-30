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

#include <forge/assert.h>
#include <forge/ast/cloners.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>
#include <forge/memory.h>

GList* _frg_ast_cloner_clone_list(const GList* list) {
  GList* clone = NULL;

  for (const GList* it = list; it != NULL; it = it->next) {
    clone = g_list_append(clone, frg_ast_clone((frg_ast_node_t*)it->data));
  }

  return clone;
}

frg_ast_node_t* frg_ast_cloner_type_primary(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert((frg_ast_node_kind_info_get(node->kind)->flags
              & FRG_AST_NODE_KIND_FLAG_TYPE_PRIMARY)
             != 0);

  frg_ast_node_t* result = frg_malloc(sizeof(frg_ast_node_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  return result;
}

frg_ast_node_t* frg_ast_cloner_type_int(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_INT);

  frg_ast_node_type_int_t* result = frg_malloc(sizeof(frg_ast_node_type_int_t));

  memcpy(result, node, sizeof(frg_ast_node_type_int_t));

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_type_float(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FLOAT);

  frg_ast_node_type_float_t* result = frg_malloc(sizeof(frg_ast_node_type_float_t));

  memcpy(result, node, sizeof(frg_ast_node_type_float_t));

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_type_symbol(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);

  frg_ast_node_type_symbol_t* result = frg_malloc(sizeof(frg_ast_node_type_symbol_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->name = g_string_new_len(((const frg_ast_node_type_symbol_t*)node)->name->str,
                                  ((const frg_ast_node_type_symbol_t*)node)->name->len);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_type_pointer(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_POINTER);

  frg_ast_node_type_pointer_t* result = frg_malloc(sizeof(frg_ast_node_type_pointer_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->flags = ((const frg_ast_node_type_pointer_t*)node)->flags;
  result->value = frg_ast_clone(((const frg_ast_node_type_pointer_t*)node)->value);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_type_array(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_ARRAY);

  frg_ast_node_type_array_t* result = frg_malloc(sizeof(frg_ast_node_type_array_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->length = ((const frg_ast_node_type_array_t*)node)->length;
  result->value  = frg_ast_clone(((const frg_ast_node_type_array_t*)node)->value);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_type_function(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);

  frg_ast_node_type_function_t* result
    = frg_malloc(sizeof(frg_ast_node_type_function_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->arguments = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_type_function_t*)node)->arguments);
  result->variadic_positional_arguments = frg_ast_clone(
    ((const frg_ast_node_type_function_t*)node)->variadic_positional_arguments);
  result->variadic_keyword_arguments = frg_ast_clone(
    ((const frg_ast_node_type_function_t*)node)->variadic_keyword_arguments);
  result->return_type
    = frg_ast_clone(((const frg_ast_node_type_function_t*)node)->return_type);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_declaration_union(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_UNION);

  frg_ast_node_declaration_union_t* result
    = frg_malloc(sizeof(frg_ast_node_declaration_union_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->name
    = g_string_new_len(((const frg_ast_node_declaration_union_t*)node)->name->str,
                       ((const frg_ast_node_declaration_union_t*)node)->name->len);
  result->properties = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_declaration_union_t*)node)->properties);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_declaration_structure(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_STRUCTURE);

  frg_ast_node_declaration_structure_t* result
    = frg_malloc(sizeof(frg_ast_node_declaration_structure_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->name
    = g_string_new_len(((const frg_ast_node_declaration_structure_t*)node)->name->str,
                       ((const frg_ast_node_declaration_structure_t*)node)->name->len);
  result->declarations = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_declaration_structure_t*)node)->declarations);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_declaration_property(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);

  frg_ast_node_declaration_property_t* result
    = frg_malloc(sizeof(frg_ast_node_declaration_property_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->flags = ((const frg_ast_node_declaration_property_t*)node)->flags;
  result->name
    = g_string_new_len(((const frg_ast_node_declaration_property_t*)node)->name->str,
                       ((const frg_ast_node_declaration_property_t*)node)->name->len);
  result->type
    = frg_ast_clone(((const frg_ast_node_declaration_property_t*)node)->type);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_declaration_interface(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_INTERFACE);

  frg_ast_node_declaration_interface_t* result
    = frg_malloc(sizeof(frg_ast_node_declaration_interface_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->flags = ((const frg_ast_node_declaration_interface_t*)node)->flags;
  result->name
    = g_string_new_len(((const frg_ast_node_declaration_interface_t*)node)->name->str,
                       ((const frg_ast_node_declaration_interface_t*)node)->name->len);
  result->extends = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_declaration_interface_t*)node)->extends);
  result->declarations = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_declaration_interface_t*)node)->declarations);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_declaration_function_argument(
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);

  frg_ast_node_declaration_function_argument_t* result
    = frg_malloc(sizeof(frg_ast_node_declaration_function_argument_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->flags    = ((const frg_ast_node_declaration_function_argument_t*)node)->flags;
  result->property = (frg_ast_node_declaration_property_t*)frg_ast_clone(
    (const frg_ast_node_t*)((const frg_ast_node_declaration_function_argument_t*)node)
      ->property);
  result->default_value = frg_ast_clone(
    ((const frg_ast_node_declaration_function_argument_t*)node)->default_value);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_declaration_function(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);

  frg_ast_node_declaration_function_t* result
    = frg_malloc(sizeof(frg_ast_node_declaration_function_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->flags = ((const frg_ast_node_declaration_function_t*)node)->flags;
  result->name
    = g_string_new_len(((const frg_ast_node_declaration_function_t*)node)->name->str,
                       ((const frg_ast_node_declaration_function_t*)node)->name->len);
  result->type = (frg_ast_node_type_function_t*)frg_ast_clone(
    (const frg_ast_node_t*)((const frg_ast_node_declaration_function_t*)node)->type);
  result->body
    = frg_ast_clone(((const frg_ast_node_declaration_function_t*)node)->body);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_declaration_assignment(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT);

  frg_ast_node_declaration_assignment_t* result
    = frg_malloc(sizeof(frg_ast_node_declaration_assignment_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->property = (frg_ast_node_declaration_property_t*)frg_ast_clone(
    (const frg_ast_node_t*)((const frg_ast_node_declaration_assignment_t*)node)
      ->property);
  result->value
    = frg_ast_clone(((const frg_ast_node_declaration_assignment_t*)node)->value);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_declaration_block(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_BLOCK);

  frg_ast_node_declaration_block_t* result
    = frg_malloc(sizeof(frg_ast_node_declaration_block_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->declarations = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_declaration_block_t*)node)->declarations);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_statement_return(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_RETURN);

  frg_ast_node_statement_return_t* result
    = frg_malloc(sizeof(frg_ast_node_statement_return_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->value = frg_ast_clone(((const frg_ast_node_statement_return_t*)node)->value);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_statement_if_conditional_clause(
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind,
                          FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE);

  frg_ast_node_statement_if_conditional_clause_t* result
    = frg_malloc(sizeof(frg_ast_node_statement_if_conditional_clause_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->condition = frg_ast_clone(
    ((const frg_ast_node_statement_if_conditional_clause_t*)node)->condition);
  result->body = frg_ast_clone(
    ((const frg_ast_node_statement_if_conditional_clause_t*)node)->body);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_statement_if(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_IF);

  frg_ast_node_statement_if_t* result = frg_malloc(sizeof(frg_ast_node_statement_if_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->conditional_clauses = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_statement_if_t*)node)->conditional_clauses);

  result->else_clause
    = frg_ast_clone(((const frg_ast_node_statement_if_t*)node)->else_clause);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_statement_while(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_WHILE);

  frg_ast_node_statement_while_t* result
    = frg_malloc(sizeof(frg_ast_node_statement_while_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->condition
    = frg_ast_clone(((const frg_ast_node_statement_while_t*)node)->condition);
  result->body = frg_ast_clone(((const frg_ast_node_statement_while_t*)node)->body);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_statement_block(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_STATEMENT_BLOCK);

  frg_ast_node_statement_block_t* result
    = frg_malloc(sizeof(frg_ast_node_statement_block_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->statements = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_statement_block_t*)node)->statements);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_bool(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_BOOL);

  frg_ast_node_value_bool_t* result = frg_malloc(sizeof(frg_ast_node_value_bool_t));

  memcpy(result, node, sizeof(frg_ast_node_value_bool_t));

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_int(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_INT);

  frg_ast_node_value_int_t* result = frg_malloc(sizeof(frg_ast_node_value_int_t));

  memcpy(result, node, sizeof(frg_ast_node_value_int_t));

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_float(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_FLOAT);

  frg_ast_node_value_float_t* result = frg_malloc(sizeof(frg_ast_node_value_float_t));

  memcpy(result, node, sizeof(frg_ast_node_value_float_t));

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_character(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CHARACTER);

  frg_ast_node_value_character_t* result
    = frg_malloc(sizeof(frg_ast_node_value_character_t));

  memcpy(result, node, sizeof(frg_ast_node_value_character_t));

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_string(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_STRING);

  frg_ast_node_value_string_t* result = frg_malloc(sizeof(frg_ast_node_value_string_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->value
    = g_string_new_len(((const frg_ast_node_value_string_t*)node)->value->str,
                       ((const frg_ast_node_value_string_t*)node)->value->len);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_array(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_ARRAY);

  frg_ast_node_value_array_t* result = frg_malloc(sizeof(frg_ast_node_value_array_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->elements
    = _frg_ast_cloner_clone_list(((const frg_ast_node_value_array_t*)node)->elements);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_array_repeated(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED);

  frg_ast_node_value_array_repeated_t* result
    = frg_malloc(sizeof(frg_ast_node_value_array_repeated_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->length = ((const frg_ast_node_value_array_repeated_t*)node)->length;
  result->element
    = frg_ast_clone(((const frg_ast_node_value_array_repeated_t*)node)->element);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_structure(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_STRUCTURE);

  frg_ast_node_value_structure_t* result
    = frg_malloc(sizeof(frg_ast_node_value_structure_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->assignments = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_value_structure_t*)node)->assignments);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_symbol(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_SYMBOL);

  frg_ast_node_value_symbol_t* result = frg_malloc(sizeof(frg_ast_node_value_symbol_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->name
    = g_string_new_len(((const frg_ast_node_value_symbol_t*)node)->name->str,
                       ((const frg_ast_node_value_symbol_t*)node)->name->len);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_call_keyword_argument(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT);

  frg_ast_node_value_call_keyword_argument_t* result
    = frg_malloc(sizeof(frg_ast_node_value_call_keyword_argument_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->name = g_string_new_len(
    ((const frg_ast_node_value_call_keyword_argument_t*)node)->name->str,
    ((const frg_ast_node_value_call_keyword_argument_t*)node)->name->len);
  result->value
    = frg_ast_clone(((const frg_ast_node_value_call_keyword_argument_t*)node)->value);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_call(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL);

  frg_ast_node_value_call_t* result = frg_malloc(sizeof(frg_ast_node_value_call_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->callee = frg_ast_clone(((const frg_ast_node_value_call_t*)node)->callee);
  result->arguments
    = _frg_ast_cloner_clone_list(((const frg_ast_node_value_call_t*)node)->arguments);
  result->keyword_arguments = _frg_ast_cloner_clone_list(
    ((const frg_ast_node_value_call_t*)node)->keyword_arguments);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_cast(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CAST);

  frg_ast_node_value_cast_t* result = frg_malloc(sizeof(frg_ast_node_value_cast_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->value = frg_ast_clone(((const frg_ast_node_value_cast_t*)node)->value);
  result->type  = frg_ast_clone(((const frg_ast_node_value_cast_t*)node)->type);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_unary(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert(
    (frg_ast_node_kind_info_get(node->kind)->flags & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY)
    != 0);

  frg_ast_node_value_unary_t* result = frg_malloc(sizeof(frg_ast_node_value_unary_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->operand = frg_ast_clone(((const frg_ast_node_value_unary_t*)node)->operand);

  return (frg_ast_node_t*)result;
}

frg_ast_node_t* frg_ast_cloner_value_binary(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert((frg_ast_node_kind_info_get(node->kind)->flags
              & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY)
             != 0);

  frg_ast_node_value_binary_t* result = frg_malloc(sizeof(frg_ast_node_value_binary_t));

  memcpy(result, node, sizeof(frg_ast_node_t));

  result->left  = frg_ast_clone(((const frg_ast_node_value_binary_t*)node)->left);
  result->right = frg_ast_clone(((const frg_ast_node_value_binary_t*)node)->right);

  return (frg_ast_node_t*)result;
}
