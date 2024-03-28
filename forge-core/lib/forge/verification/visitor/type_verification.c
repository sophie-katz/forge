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
#include <forge/verification/verifier.h>
#include <forge/verification/visitor/type_verification.h>

frg_ast_visitor_status_t frg_verification_type_verification_handle_enter_type_symbol(
  frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_die_ast_kind_not_yet_supported((*mut_node)->kind);
}

const frg_ast_node_declaration_function_t* _frg_get_surrounding_declaration_function(
  const GList* parents) {
  const frg_ast_node_t* parent = NULL;
  for (const GList* it = parents; it != NULL; it = it->next) {
    parent = it->data;
    if (parent->kind == FRG_AST_NODE_KIND_DECLARATION_FUNCTION) {
      return (const frg_ast_node_declaration_function_t*)parent;
    }
  }
  return NULL;
}

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
    = _frg_get_surrounding_declaration_function(parents);

  if (surrounding_declaration_function == NULL) {
    frg_die_message_no_code_yet();
  }

  const frg_ast_node_t* expected = surrounding_declaration_function->type->return_type;

  const frg_ast_node_t* actual   = frg_verification_resolve_type(
    verifier->mut_message_buffer, verifier->mut_scope, statement_return->value);

  if (!frg_ast_compare(expected, actual)) {
    frg_emit_message_et_5_mismatched_return_type(verifier->mut_message_buffer,
                                                 &statement_return->value->source_range,
                                                 expected,
                                                 actual);
    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

void frg_verification_type_verification_add_handlers(frg_ast_visitor_t* mut_visitor) {
  frg_ast_visitor_handler_t* handler_statement_return
    = frg_ast_visitor_add_handler(mut_visitor, FRG_AST_NODE_KIND_STATEMENT_RETURN);
  handler_statement_return->event_callbacks[FRG_AST_VISITOR_EVENT_ENTER]
    = frg_verification_type_verification_handle_enter_statement_return;
}
