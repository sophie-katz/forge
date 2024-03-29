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
#include <forge/formatting/print.h>
#include <forge/messages/codes.h>
#include <forge/verification/type_operations.h>
#include <forge/verification/type_resolvers.h>

frg_ast_node_t* frg_verification_type_resolver_declaration_union(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_UNION);

  frg_message_emit_eft_1_unsupported_requirement(
    mut_message_buffer, &node->source_range, 9, "Union declarations");

  return NULL;
}

frg_ast_node_t* frg_verification_type_resolver_declaration_structure(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_STRUCTURE);

  frg_message_emit_eft_1_unsupported_requirement(
    mut_message_buffer, &node->source_range, 10, "Struct declarations");

  return NULL;
}

frg_ast_node_t* frg_verification_type_resolver_declaration_property(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);

  if (((frg_ast_node_declaration_property_t*)node)->type == NULL) {
    frg_message_emit_eft_1_unsupported_requirement(
      mut_message_buffer, &node->source_range, 11, "Dynamic objects");
  }

  return frg_ast_clone(((frg_ast_node_declaration_property_t*)node)->type);
}

frg_ast_node_t* frg_verification_type_resolver_declaration_interface(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_INTERFACE);

  frg_message_emit_eft_1_unsupported_requirement(
    mut_message_buffer, &node->source_range, 11, "Interface declarations");

  return NULL;
}

frg_ast_node_t* frg_verification_type_resolver_declaration_function_argument(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);

  frg_assert_pointer_non_null(
    ((frg_ast_node_declaration_function_argument_t*)node)->property);

  return frg_verification_resolve_type(
    mut_message_buffer,
    scope,
    (frg_ast_node_t*)((frg_ast_node_declaration_function_argument_t*)node)->property);
}

frg_ast_node_t* frg_verification_type_resolver_declaration_function(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);

  frg_assert_pointer_non_null(((frg_ast_node_declaration_function_t*)node)->type);

  return frg_ast_clone(
    (frg_ast_node_t*)((frg_ast_node_declaration_function_t*)node)->type);
}

frg_ast_node_t* frg_verification_type_resolver_declaration_assignment(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT);

  frg_assert_pointer_non_null(((frg_ast_node_declaration_assignment_t*)node)->property);

  return frg_verification_resolve_type(
    mut_message_buffer,
    scope,
    (frg_ast_node_t*)((frg_ast_node_declaration_assignment_t*)node)->property);
}

frg_ast_node_t* frg_verification_type_resolver_as_type_bool(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  return frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                       FRG_AST_NODE_KIND_TYPE_BOOL);
}

frg_ast_node_t* frg_verification_type_resolver_value_int(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_INT);

  return (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null,
    ((const frg_ast_node_value_int_t*)node)->type.flags,
    ((const frg_ast_node_value_int_t*)node)->type.bit_width);
}

frg_ast_node_t* frg_verification_type_resolver_value_float(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_FLOAT);

  return (frg_ast_node_t*)frg_ast_node_type_float_new(
    &frg_global_parsing_range_null,
    ((const frg_ast_node_value_float_t*)node)->type.bit_width);
}

frg_ast_node_t* frg_verification_type_resolver_value_character(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CHARACTER);

  frg_message_emit_eft_1_unsupported_requirement(
    mut_message_buffer, &node->source_range, 5, "Character literals");

  return NULL;
}

frg_ast_node_t* frg_verification_type_resolver_value_string(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_STRING);

  frg_message_emit_eft_1_unsupported_requirement(
    mut_message_buffer, &node->source_range, 4, "String literals");

  return NULL;
}

frg_ast_node_t* frg_verification_type_resolver_value_array(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_ARRAY);

  if (((frg_ast_node_value_array_t*)node)->elements == NULL) {
    frg_die_message_no_code_yet();
  }

  frg_ast_node_t* element_type = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_array_t*)node)->elements->data);

  if (element_type == NULL) {
    return NULL;
  }

  return (frg_ast_node_t*)frg_ast_node_type_array_new(
    &frg_global_parsing_range_null,
    g_list_length(((frg_ast_node_value_array_t*)node)->elements),
    frg_ast_clone(element_type));
}

frg_ast_node_t* frg_verification_type_resolver_value_array_repeated(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED);

  frg_ast_node_t* element_type = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_array_repeated_t*)node)->element);

  if (element_type == NULL) {
    return NULL;
  }

  return (frg_ast_node_t*)frg_ast_node_type_array_new(
    &frg_global_parsing_range_null,
    ((frg_ast_node_value_array_repeated_t*)node)->length,
    frg_ast_clone(element_type));
}

frg_ast_node_t* frg_verification_type_resolver_value_symbol(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_SYMBOL);

  frg_assert_gstring_non_empty(((frg_ast_node_value_symbol_t*)node)->name);

  frg_ast_node_t* declaration = frg_ast_scope_get_ast(
    (frg_ast_scope_t*)scope, ((frg_ast_node_value_symbol_t*)node)->name->str);

  if (declaration == NULL) {
    frg_emit_message_et_1_undeclared_symbol(mut_message_buffer,
                                            &node->source_range,
                                            ((frg_ast_node_value_symbol_t*)node)->name);

    return NULL;
  }

  return frg_verification_resolve_type(mut_message_buffer, scope, declaration);
}

frg_ast_node_t* frg_verification_type_resolver_value_dereference(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_DEREFERENCE);

  frg_ast_node_t* resolved_type = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_unary_t*)node)->operand);

  if (resolved_type == NULL) {
    return NULL;
  }

  if (resolved_type->kind != FRG_AST_NODE_KIND_TYPE_POINTER
      || ((const frg_ast_node_type_pointer_t*)resolved_type)->flags
           & FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE) {
    frg_emit_message_et_2_cannot_dereference_non_pointer(
      mut_message_buffer, &node->source_range, resolved_type);

    return NULL;
  }

  frg_assert_pointer_non_null(((frg_ast_node_type_pointer_t*)resolved_type)->value);

  return (frg_ast_node_t*)frg_ast_node_type_pointer_new(
    &frg_global_parsing_range_null,
    ((frg_ast_node_type_pointer_t*)resolved_type)->flags
      | FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE,
    frg_ast_clone(((frg_ast_node_type_pointer_t*)resolved_type)->value));
}

frg_ast_node_t* frg_verification_type_resolver_value_get_address(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_GET_ADDRESS);

  frg_ast_node_t* resolved_type = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_unary_t*)node)->operand);

  if (resolved_type == NULL) {
    return NULL;
  }

  if (resolved_type->kind != FRG_AST_NODE_KIND_TYPE_POINTER
      || !(((const frg_ast_node_type_pointer_t*)resolved_type)->flags
           & FRG_AST_NODE_TYPE_POINTER_FLAG_IMPLICIT_DEREFERENCE)) {
    frg_emit_message_et_4_cannot_get_address_non_reference(
      mut_message_buffer, &node->source_range, resolved_type);

    return NULL;
  }

  return (frg_ast_node_t*)frg_ast_node_type_pointer_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_POINTER_FLAG_NONE, resolved_type);
}

frg_ast_node_t* frg_verification_type_resolver_value_call(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CALL);

  frg_assert_pointer_non_null(((frg_ast_node_value_call_t*)node)->callee);

  frg_ast_node_t* resolved_type = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_call_t*)node)->callee);

  if (resolved_type == NULL) {
    return NULL;
  }

  if (resolved_type->kind != FRG_AST_NODE_KIND_TYPE_FUNCTION) {
    frg_emit_message_et_3_cannot_call_non_function(
      mut_message_buffer, &node->source_range, resolved_type);

    return NULL;
  }

  frg_assert_pointer_non_null(
    ((frg_ast_node_type_function_t*)resolved_type)->return_type);

  return frg_ast_clone(((frg_ast_node_type_function_t*)resolved_type)->return_type);
}

frg_ast_node_t* frg_verification_type_resolver_value_cast(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_CAST);

  frg_assert_pointer_non_null(((frg_ast_node_value_cast_t*)node)->type);

  return frg_ast_clone(((frg_ast_node_value_cast_t*)node)->type);
}

frg_ast_node_t* frg_verification_type_resolver_value_access(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_VALUE_ACCESS);

  frg_message_emit_eft_1_unsupported_requirement(
    mut_message_buffer, &node->source_range, 10, "Struct declarations");

  return NULL;
}

frg_ast_node_t* frg_verification_type_resolver_value_unary_ident(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY);

  frg_assert_pointer_non_null(((frg_ast_node_value_unary_t*)node)->operand);

  return frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_unary_t*)node)->operand);
}

frg_ast_node_t* frg_verification_type_resolver_value_unary_ident_int_only(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY);

  frg_assert_pointer_non_null(((frg_ast_node_value_unary_t*)node)->operand);

  frg_ast_node_t* result = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_unary_t*)node)->operand);

  if (result->kind == FRG_AST_NODE_KIND_TYPE_INT) {
    return result;
  } else {
    frg_ast_node_destroy(result);
    return NULL;
  }
}

frg_ast_node_t* frg_verification_type_resolver_value_unary_ident_numeric_only(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY);

  frg_assert_pointer_non_null(((frg_ast_node_value_unary_t*)node)->operand);

  frg_ast_node_t* result = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_unary_t*)node)->operand);

  if (result->kind == FRG_AST_NODE_KIND_TYPE_INT
      || result->kind == FRG_AST_NODE_KIND_TYPE_FLOAT) {
    return result;
  } else {
    frg_ast_node_destroy(result);
    return NULL;
  }
}

frg_ast_node_t* frg_verification_type_resolver_value_binary_containing(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY);

  frg_assert_pointer_non_null(((frg_ast_node_value_binary_t*)node)->left);
  frg_assert_pointer_non_null(((frg_ast_node_value_binary_t*)node)->right);

  frg_ast_node_t* type_left = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_binary_t*)node)->left);

  if (type_left == NULL) {
    return NULL;
  }

  frg_ast_node_t* type_right = frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_binary_t*)node)->right);

  if (type_right == NULL) {
    return NULL;
  }

  frg_ast_node_t* type_containing
    = frg_verification_get_numeric_containing_type(type_left, type_right);

  frg_ast_node_destroy(type_left);
  frg_ast_node_destroy(type_right);

  if (type_containing == NULL) {
    frg_emit_message_it_1_no_containing_type(
      mut_message_buffer, &node->source_range, type_left, type_right);
  }

  return type_containing;
}

frg_ast_node_t* frg_verification_type_resolver_value_binary_left(
  frg_message_buffer_t* mut_message_buffer,
  const frg_ast_scope_t* scope,
  const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY);

  frg_assert_pointer_non_null(((frg_ast_node_value_binary_t*)node)->left);

  return frg_verification_resolve_type(
    mut_message_buffer, scope, ((frg_ast_node_value_binary_t*)node)->left);
}
