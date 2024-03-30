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
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>
#include <forge/formatting/print.h>
#include <forge/messages/codes.h>
#include <forge/verification/type_operations.h>
#include <forge/verification/verifier.h>
#include <forge/verification/visitor/type_verification.h>

frg_ast_visitor_status_t
  frg_verification_type_verification_handle_enter_statement_return(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  const frg_ast_node_statement_return_t* statement_return
    = (const frg_ast_node_statement_return_t*)(*mut_node);

  const frg_ast_node_declaration_function_t* surrounding_declaration_function
    = (const frg_ast_node_declaration_function_t*)
      frg_ast_get_surrounding_node_with_kind(parents,
                                             FRG_AST_NODE_KIND_DECLARATION_FUNCTION);

  if (surrounding_declaration_function == NULL) {
    frg_die_message_no_code_yet();
  }

  const frg_ast_node_t* expected = surrounding_declaration_function->type->return_type;

  frg_ast_node_t* actual         = frg_verification_resolve_type(
    verifier->mut_message_buffer, verifier->mut_scope, statement_return->value);

  if (actual == NULL) {
    return FRG_AST_VISITOR_STATUS_OK;
  }

  if (!frg_ast_compare(expected, actual)) {
    frg_emit_message_et_5_mismatched_return_type(verifier->mut_message_buffer,
                                                 &statement_return->value->source_range,
                                                 expected,
                                                 actual);
  }

  frg_ast_node_destroy(actual);

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_type_verification_handle_enter_value_unary(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert((frg_ast_node_kind_info_get((*mut_node)->kind)->flags
              & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY)
             != 0);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  const frg_ast_node_value_unary_t* value_unary
    = (const frg_ast_node_value_unary_t*)(*mut_node);

  frg_ast_node_t* resolved = frg_verification_resolve_type(
    verifier->mut_message_buffer, verifier->mut_scope, value_unary->operand);

  if (resolved == NULL) {
    return FRG_AST_VISITOR_STATUS_OK;
  }

  switch (value_unary->base.kind) {
  case FRG_AST_NODE_KIND_VALUE_BIT_NOT:
    if (resolved->kind != FRG_AST_NODE_KIND_TYPE_INT) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_unary->operand->source_range,
        frg_ast_node_kind_info_get(value_unary->base.kind)->operator_symbol,
        "operand",
        "integer",
        resolved);
    }
    break;
  case FRG_AST_NODE_KIND_VALUE_NEGATE:
    if (resolved->kind != FRG_AST_NODE_KIND_TYPE_INT
        && resolved->kind != FRG_AST_NODE_KIND_TYPE_FLOAT) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_unary->operand->source_range,
        frg_ast_node_kind_info_get(value_unary->base.kind)->operator_symbol,
        "operand",
        "numeric",
        resolved);
    }
    break;
  case FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT:
    if (resolved->kind != FRG_AST_NODE_KIND_TYPE_BOOL) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_unary->operand->source_range,
        frg_ast_node_kind_info_get(value_unary->base.kind)->operator_symbol,
        "operand",
        "'bool'",
        resolved);
    }
    break;
  default:
    frg_die_unexpected_enum_value(value_unary->base.kind);
  }

  frg_ast_node_destroy(resolved);

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_type_verification_handle_enter_value_binary(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert((frg_ast_node_kind_info_get((*mut_node)->kind)->flags
              & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY)
             != 0);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  const frg_ast_node_value_binary_t* value_binary
    = (const frg_ast_node_value_binary_t*)(*mut_node);

  frg_ast_node_t* type_left = frg_verification_resolve_type(
    verifier->mut_message_buffer, verifier->mut_scope, value_binary->left);

  if (type_left == NULL) {
    return FRG_AST_VISITOR_STATUS_OK;
  }

  frg_ast_node_t* type_right = frg_verification_resolve_type(
    verifier->mut_message_buffer, verifier->mut_scope, value_binary->right);

  if (type_right == NULL) {
    frg_ast_node_destroy(type_left);
    return FRG_AST_VISITOR_STATUS_OK;
  }

  switch (value_binary->base.kind) {
  case FRG_AST_NODE_KIND_VALUE_BIT_AND:
  case FRG_AST_NODE_KIND_VALUE_BIT_OR:
  case FRG_AST_NODE_KIND_VALUE_BIT_XOR:
    if (type_left->kind != FRG_AST_NODE_KIND_TYPE_INT) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_binary->left->source_range,
        frg_ast_node_kind_info_get(value_binary->base.kind)->operator_symbol,
        "left-hand side",
        "integer",
        type_left);
    }

    if (type_right->kind != FRG_AST_NODE_KIND_TYPE_INT) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_binary->right->source_range,
        frg_ast_node_kind_info_get(value_binary->base.kind)->operator_symbol,
        "right-hand side",
        "integer",
        type_right);
    }

    break;
  case FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT:
  case FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT:
    if (type_left->kind != FRG_AST_NODE_KIND_TYPE_INT) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_binary->left->source_range,
        frg_ast_node_kind_info_get(value_binary->base.kind)->operator_symbol,
        "left-hand side",
        "integer",
        type_left);
    }

    if (type_right->kind != FRG_AST_NODE_KIND_TYPE_INT
        || ((const frg_ast_node_type_int_t*)type_right)->flags
             != FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED
        || ((const frg_ast_node_type_int_t*)type_right)->bit_width != 32) {
      // frg_message_t* message =
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_binary->right->source_range,
        frg_ast_node_kind_info_get(value_binary->base.kind)->operator_symbol,
        "right-hand side",
        "'u32'",
        type_right);

      // TODO: Make this note work
      // frg_message_emit_child(
      //   verifier->mut_message_buffer, message, FRG_MESSAGE_SEVERITY_NOTE, "NT-1");
    }

    break;
  case FRG_AST_NODE_KIND_VALUE_LOGICAL_AND:
  case FRG_AST_NODE_KIND_VALUE_LOGICAL_OR:
    if (type_left->kind != FRG_AST_NODE_KIND_TYPE_BOOL) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_binary->left->source_range,
        frg_ast_node_kind_info_get(value_binary->base.kind)->operator_symbol,
        "left-hand side",
        "'bool'",
        type_left);
    }

    if (type_right->kind != FRG_AST_NODE_KIND_TYPE_BOOL) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_binary->right->source_range,
        frg_ast_node_kind_info_get(value_binary->base.kind)->operator_symbol,
        "right-hand side",
        "'bool'",
        type_right);
    }

    break;
  case FRG_AST_NODE_KIND_VALUE_ADD:
  case FRG_AST_NODE_KIND_VALUE_SUBTRACT:
  case FRG_AST_NODE_KIND_VALUE_MULTIPLY:
  case FRG_AST_NODE_KIND_VALUE_DIVIDE:
  case FRG_AST_NODE_KIND_VALUE_DIVIDE_INT:
  case FRG_AST_NODE_KIND_VALUE_MODULO:
  case FRG_AST_NODE_KIND_VALUE_EXPONENTIATE:
  case FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN:
  case FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO:
  case FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN:
  case FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO:
    if (type_left->kind != FRG_AST_NODE_KIND_TYPE_INT
        && type_left->kind != FRG_AST_NODE_KIND_TYPE_FLOAT) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_binary->left->source_range,
        frg_ast_node_kind_info_get(value_binary->base.kind)->operator_symbol,
        "left-hand side",
        "numeric",
        type_left);
    }

    if (type_right->kind != FRG_AST_NODE_KIND_TYPE_INT
        && type_right->kind != FRG_AST_NODE_KIND_TYPE_FLOAT) {
      frg_emit_message_et_6_operator_unexpected_operand_type(
        verifier->mut_message_buffer,
        &value_binary->right->source_range,
        frg_ast_node_kind_info_get(value_binary->base.kind)->operator_symbol,
        "right-hand side",
        "numeric",
        type_right);
    }

    break;
  default:
    frg_die_unexpected_enum_value(value_binary->base.kind);
  }

  frg_ast_node_destroy(type_left);
  frg_ast_node_destroy(type_right);

  return FRG_AST_VISITOR_STATUS_OK;
}

void frg_verification_type_verification_add_handlers(frg_ast_visitor_t* mut_visitor) {
  frg_ast_visitor_handler_t* handler_statement_return
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  handler_statement_return->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_statement_return;

  frg_ast_visitor_handler_t* handler_value_bit_not
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_BIT_NOT);
  handler_value_bit_not->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_unary;

  frg_ast_visitor_handler_t* handler_value_bit_and
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_BIT_AND);
  handler_value_bit_and->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_binary;

  frg_ast_visitor_handler_t* handler_value_bit_or
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_BIT_OR);
  handler_value_bit_or->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_binary;

  frg_ast_visitor_handler_t* handler_value_bit_xor
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_BIT_XOR);
  handler_value_bit_xor->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_binary;

  frg_ast_visitor_handler_t* handler_value_bit_shift_left
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT);
  handler_value_bit_shift_left->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_binary;

  frg_ast_visitor_handler_t* handler_value_bit_shift_right
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT);
  handler_value_bit_shift_right->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_binary;

  frg_ast_visitor_handler_t* handler_value_logical_not
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT);
  handler_value_logical_not->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_unary;

  frg_ast_visitor_handler_t* handler_value_logical_and
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_LOGICAL_AND);
  handler_value_logical_and->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_binary;

  frg_ast_visitor_handler_t* handler_value_logical_or
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_LOGICAL_OR);
  handler_value_logical_or->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_binary;

  frg_ast_visitor_handler_t* handler_value_negate
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_VALUE_NEGATE);
  handler_value_negate->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_value_unary;
}
