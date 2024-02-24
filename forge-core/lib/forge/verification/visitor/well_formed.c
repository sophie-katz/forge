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
#include <forge/messages/codes.h>
#include <forge/verification/verifier.h>
#include <forge/verification/visitor/well_formed.h>
#include <utf8proc.h>

bool _frg_verification_well_formed_is_valid_symbol_codepoint_first(
  utf8proc_int32_t codepoint) {
  if (codepoint == '_') {
    return true;
  } else if (codepoint >= 'a' && codepoint <= 'z') {
    return true;
  } else if (codepoint >= 'A' && codepoint <= 'Z') {
    return true;
  }

  switch (utf8proc_category(codepoint)) {
  case UTF8PROC_CATEGORY_CN:
  case UTF8PROC_CATEGORY_CC:
  case UTF8PROC_CATEGORY_CF:
  case UTF8PROC_CATEGORY_CS:
  case UTF8PROC_CATEGORY_CO:
    return false;
  default:
    return true;
  }
}

bool _frg_verification_well_formed_is_valid_symbol_codepoint(
  utf8proc_int32_t codepoint) {
  if (codepoint >= '0' && codepoint <= '9') {
    return true;
  }

  return _frg_verification_well_formed_is_valid_symbol_codepoint_first(codepoint);
}

bool _frg_verification_well_formed_is_valid_symbol(const GString* name) {
  for (const gchar* iter = name->str; *iter != 0;) {
    utf8proc_int32_t codepoint  = 0;

    utf8proc_ssize_t bytes_read = utf8proc_iterate(
      (const utf8proc_uint8_t*)iter, name->len - (iter - name->str), &codepoint);

    if (bytes_read < 0) {
      return false;
    } else {
      iter += bytes_read;
    }

    if (iter == name->str) {
      if (!_frg_verification_well_formed_is_valid_symbol_codepoint_first(codepoint)) {
        return false;
      }
    } else {
      if (!_frg_verification_well_formed_is_valid_symbol_codepoint(codepoint)) {
        return false;
      }
    }
  }

  return true;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_symbol(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);
  frg_assert_pointer_non_null(mut_user_data);

  frg_die_ast_kind_not_yet_supported((*mut_node)->kind);

  // frg_ast_type_symbol_t* ty_symbol = (frg_ast_type_symbol_t*)*mut_node;
  // frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

  // if (ty_symbol->name == NULL) {
  //     frg_message_emit_is_5_ast_property_null(
  //         verifier->message_buffer,
  //         &(*mut_node)->source_range,
  //         (*mut_node)->kind,
  //         "name"
  //     );

  // return FRG_AST_VISITOR_STATUS_SKIP;
  // }

  // if (ty_symbol->name->len == 0) {
  //     frg_message_emit_is_6_ast_property_empty_string(
  //         verifier->message_buffer,
  //         &(*mut_node)->source_range,
  //         (*mut_node)->kind,
  //         "name"
  //     );

  // return FRG_AST_VISITOR_STATUS_SKIP;
  // }

  // if (!_frg_verification_well_formed_is_valid_symbol(ty_symbol->name)) {
  //     frg_message_emit_is_6_ast_property_invalid_symbol(
  //         verifier->message_buffer,
  //         &(*mut_node)->source_range,
  //         (*mut_node)->kind,
  //         "name"
  //     );

  // return FRG_AST_VISITOR_STATUS_SKIP;
  // }

  // return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_pointer(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_POINTER);
  frg_assert_pointer_non_null(mut_user_data);

  frg_die_ast_kind_not_yet_supported((*mut_node)->kind);

  // frg_ast_type_pointer_t* ty_pointer = (frg_ast_type_pointer_t*)*mut_node;
  // frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

  // if (ty_pointer->value == NULL) {
  //     frg_message_emit_is_5_ast_property_null(
  //         verifier->message_buffer,
  //         &(*mut_node)->source_range,
  //         (*mut_node)->kind,
  //         "value"
  //     );

  // return FRG_AST_VISITOR_STATUS_SKIP;
  // }

  // return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_function(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);
  frg_assert_pointer_non_null(mut_user_data);

  frg_die_ast_kind_not_yet_supported((*mut_node)->kind);

  // frg_ast_type_function_t* ty_function = (frg_ast_type_function_t*)*mut_node;
  // frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

  // size_t index = 0;
  // for (GList* iter = ty_function->args; iter != NULL; iter = iter->next) {
  //     frg_ast_node_t* arg = (frg_ast_node_t*)iter->data;

  // if (arg == NULL) {
  //     frg_message_emit_is_8_ast_property_null_element(
  //         verifier->message_buffer,
  //         &(*mut_node)->source_range,
  //         (*mut_node)->kind,
  //         "args",
  //         index
  //     );

  // return FRG_AST_VISITOR_STATUS_SKIP;
  // }

  // if (arg->kind != FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARG) {
  //     frg_message_emit_is_9_ast_property_unexpected_element(
  //         verifier->message_buffer,
  //         &(*mut_node)->source_range,
  //         (*mut_node)->kind,
  //         "args",
  //         index
  //     );

  // return FRG_AST_VISITOR_STATUS_SKIP;
  // }

  // frg_ast_declaration_function_argument_t* decl_function_arg =
  // (frg_ast_declaration_function_argument_t*)arg;

  // if (decl_function_arg->flags & FRG_AST_DECLARATION_FUNCTION_ARG_FLAG_KW) {
  //     frg_die_message_no_code_yet();
  // }

  // index++;
  // }

  // if (ty_function->var_positional_arguments != NULL) {
  //     // TODO: Keep implementing this function
  // }

  // return FRG_AST_VISITOR_STATUS_OK;
}
