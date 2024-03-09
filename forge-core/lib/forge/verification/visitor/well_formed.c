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

bool _frg_verification_well_formed_verify_source_range(
  frg_message_buffer_t* mut_message_buffer, const frg_parsing_range_t* source_range) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(source_range);

  if (source_range->start.path == NULL) {
    frg_die_message_no_code_yet();
  }

  if (*source_range->start.path == 0) {
    frg_die_message_no_code_yet();
  }

  if (source_range->start.line_number <= 0) {
    frg_die_message_no_code_yet();
  }

  if (source_range->start.column_number <= 0) {
    frg_die_message_no_code_yet();
  }

  if (source_range->length <= 0) {
    frg_die_message_no_code_yet();
  }

  return true;
}

bool _frg_verification_well_formed_verify_node_base(
  frg_message_buffer_t* mut_message_buffer, const frg_ast_node_t* node) {
  if (!_frg_verification_well_formed_verify_source_range(mut_message_buffer,
                                                         &node->source_range)) {
    return false;
  }

  return true;
}

bool _frg_verification_well_formed_verify_valid_symbol_codepoint_first(
  frg_message_buffer_t* mut_message_buffer, utf8proc_int32_t codepoint) {
  frg_assert_pointer_non_null(mut_message_buffer);

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
    frg_die_message_no_code_yet();
  default:
    return true;
  }
}

bool _frg_verification_well_formed_verify_valid_symbol_codepoint(
  frg_message_buffer_t* mut_message_buffer, utf8proc_int32_t codepoint) {
  frg_assert_pointer_non_null(mut_message_buffer);

  if (codepoint >= '0' && codepoint <= '9') {
    return true;
  }

  return _frg_verification_well_formed_verify_valid_symbol_codepoint_first(
    mut_message_buffer, codepoint);
}

bool _frg_verification_well_formed_verify_valid_symbol(
  frg_message_buffer_t* mut_message_buffer, const GString* name) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(name);

  size_t codepoint_count = 0;
  for (const gchar* iter = name->str; *iter != 0;) {
    utf8proc_int32_t codepoint  = 0;

    utf8proc_ssize_t bytes_read = utf8proc_iterate(
      (const utf8proc_uint8_t*)iter, name->len - (iter - name->str), &codepoint);

    if (bytes_read < 0) {
      frg_die_message_no_code_yet();
    } else {
      iter += bytes_read;
    }

    if (iter == name->str) {
      if (!_frg_verification_well_formed_verify_valid_symbol_codepoint_first(
            mut_message_buffer, codepoint)) {
        return false;
      }
    } else {
      if (!_frg_verification_well_formed_verify_valid_symbol_codepoint(
            mut_message_buffer, codepoint)) {
        return false;
      }
    }

    codepoint_count++;
  }

  if (codepoint_count == 0) {
    frg_die_message_no_code_yet();
  }

  return true;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_primary(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert(frg_ast_node_kind_info_get((*mut_node)->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_TYPE_PRIMARY);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_t* type_primary          = (frg_ast_node_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if (!_frg_verification_well_formed_verify_node_base(verifier->mut_message_buffer,
                                                      type_primary)) {
    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_int(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_INT);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_type_int_t* type_int     = (frg_ast_node_type_int_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if (!_frg_verification_well_formed_verify_node_base(verifier->mut_message_buffer,
                                                      &type_int->base)) {
    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if ((type_int->flags & frg_get_enum_mask_from_max(FRG_AST_NODE_TYPE_INT_FLAG_MAX))
      != type_int->flags) {
    frg_die_message_no_code_yet();
  }

  if (!frg_is_valid_bit_width_int(type_int->bit_width)) {
    frg_die_message_no_code_yet();
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_float(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_FLOAT);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_type_float_t* type_float = (frg_ast_node_type_float_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if (!_frg_verification_well_formed_verify_node_base(verifier->mut_message_buffer,
                                                      &type_float->base)) {
    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (!frg_is_valid_bit_width_float(type_float->bit_width)) {
    frg_die_message_no_code_yet();
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_symbol(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_type_symbol_t* type_symbol = (frg_ast_node_type_symbol_t*)*mut_node;
  frg_verification_verifier_t* verifier   = (frg_verification_verifier_t*)mut_user_data;

  if (!_frg_verification_well_formed_verify_node_base(verifier->mut_message_buffer,
                                                      &type_symbol->base)) {
    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (type_symbol->name == NULL) {
    frg_message_emit_is_5_ast_node_property_null(verifier->mut_message_buffer,
                                                 &(*mut_node)->source_range,
                                                 (*mut_node)->kind,
                                                 "name");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (!_frg_verification_well_formed_verify_valid_symbol(verifier->mut_message_buffer,
                                                         type_symbol->name)) {
    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_pointer(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_POINTER);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_type_pointer_t* type_pointer = (frg_ast_node_type_pointer_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if (!_frg_verification_well_formed_verify_node_base(verifier->mut_message_buffer,
                                                      &type_pointer->base)) {
    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if ((type_pointer->flags
       & frg_get_enum_mask_from_max(FRG_AST_NODE_TYPE_POINTER_FLAG_MAX))
      != type_pointer->flags) {
    frg_die_message_no_code_yet();
  }

  if (type_pointer->value == NULL) {
    frg_message_emit_is_5_ast_node_property_null(verifier->mut_message_buffer,
                                                 &(*mut_node)->source_range,
                                                 (*mut_node)->kind,
                                                 "value");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (!(frg_ast_node_kind_info_get(type_pointer->value->kind)->flags
        & FRG_AST_NODE_KIND_FLAG_TYPE)) {
    frg_die_message_no_code_yet();
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_handle_enter_type_function(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_type_function_t* type_function
    = (frg_ast_node_type_function_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if (!_frg_verification_well_formed_verify_node_base(verifier->mut_message_buffer,
                                                      &type_function->base)) {
    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  size_t index = 0;
  for (GList* iter = type_function->arguments; iter != NULL; iter = iter->next) {
    frg_ast_node_t* argument = (frg_ast_node_t*)iter->data;

    if (argument == NULL) {
      frg_message_emit_is_8_ast_node_property_null_element(verifier->mut_message_buffer,
                                                           &(*mut_node)->source_range,
                                                           (*mut_node)->kind,
                                                           "arguments",
                                                           index);

      return FRG_AST_VISITOR_STATUS_SKIP;
    }

    if (argument->kind != FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT) {
      frg_message_emit_is_9_ast_node_property_unexpected_element(
        verifier->mut_message_buffer,
        &(*mut_node)->source_range,
        (*mut_node)->kind,
        "arguments",
        index);

      return FRG_AST_VISITOR_STATUS_SKIP;
    }

    frg_ast_node_declaration_function_argument_t* declaration_function_argument
      = (frg_ast_node_declaration_function_argument_t*)argument;

    if (declaration_function_argument->flags
        & FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD) {
      frg_die_message_no_code_yet();
    }

    index++;
  }

  if (type_function->variadic_positional_arguments != NULL) {
    if (type_function->variadic_positional_arguments->kind
        != FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT) {
      frg_message_emit_is_9_ast_node_property_unexpected_element(
        verifier->mut_message_buffer,
        &(*mut_node)->source_range,
        (*mut_node)->kind,
        "variadic_positional_arguments",
        index);

      return FRG_AST_VISITOR_STATUS_SKIP;
    }

    frg_ast_node_declaration_function_argument_t* declaration_function_argument
      = (frg_ast_node_declaration_function_argument_t*)
          type_function->variadic_positional_arguments;

    if (declaration_function_argument->flags
        & FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD) {
      frg_die_message_no_code_yet();
    }
  }

  if (type_function->variadic_keyword_arguments != NULL) {
    if (type_function->variadic_keyword_arguments->kind
        != FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT) {
      frg_message_emit_is_9_ast_node_property_unexpected_element(
        verifier->mut_message_buffer,
        &(*mut_node)->source_range,
        (*mut_node)->kind,
        "variadic_keyword_arguments",
        index);

      return FRG_AST_VISITOR_STATUS_SKIP;
    }

    frg_ast_node_declaration_function_argument_t* declaration_function_argument
      = (frg_ast_node_declaration_function_argument_t*)
          type_function->variadic_keyword_arguments;

    if (!(declaration_function_argument->flags
          & FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD)) {
      frg_die_message_no_code_yet();
    }
  }

  if (type_function->return_type == NULL) {
    frg_message_emit_is_5_ast_node_property_null(verifier->mut_message_buffer,
                                                 &(*mut_node)->source_range,
                                                 (*mut_node)->kind,
                                                 "return_type");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (!(frg_ast_node_kind_info_get(type_function->return_type->kind)->flags
        & FRG_AST_NODE_KIND_FLAG_TYPE)) {
    frg_die_message_no_code_yet();
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

void frg_verification_well_formed_add_handlers(frg_ast_visitor_t* mut_visitor) {
  frg_ast_visitor_handler_t* handler_type_void
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_TYPE_VOID);
  handler_type_void->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_well_formed_handle_enter_type_primary;

  frg_ast_visitor_handler_t* handler_type_bool
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_TYPE_BOOL);
  handler_type_bool->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_well_formed_handle_enter_type_primary;

  frg_ast_visitor_handler_t* handler_type_int
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_TYPE_INT);
  handler_type_int->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_well_formed_handle_enter_type_int;

  frg_ast_visitor_handler_t* handler_type_float
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_TYPE_FLOAT);
  handler_type_float->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_well_formed_handle_enter_type_float;

  frg_ast_visitor_handler_t* handler_type_symbol
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_TYPE_SYMBOL);
  handler_type_symbol->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_well_formed_handle_enter_type_symbol;

  frg_ast_visitor_handler_t* handler_type_pointer
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_TYPE_POINTER);
  handler_type_pointer->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_well_formed_handle_enter_type_pointer;

  frg_ast_visitor_handler_t* handler_type_function
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_TYPE_FUNCTION);
  handler_type_function->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_well_formed_handle_enter_type_function;
}
