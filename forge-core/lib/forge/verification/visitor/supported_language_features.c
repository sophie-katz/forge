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
#include <forge/messages/codes.h>
#include <forge/verification/verifier.h>
#include <forge/verification/visitor/supported_language_features.h>

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_type_function(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_type_function_t* type_function
    = (frg_ast_node_type_function_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if (type_function->variadic_positional_arguments != NULL) {
    frg_message_emit_eft_2_unsupported_requirement_subitem(
      verifier->mut_message_buffer,
      &(*mut_node)->source_range,
      14,
      "Function declarations",
      1,
      "Variable positional arguments");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (type_function->variadic_keyword_arguments != NULL) {
    frg_message_emit_eft_2_unsupported_requirement_subitem(
      verifier->mut_message_buffer,
      &(*mut_node)->source_range,
      14,
      "Function declarations",
      2,
      "Variable keyword arguments");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (type_function->return_type == NULL) {
    frg_message_emit_eft_1_unsupported_requirement(
      verifier->mut_message_buffer, &(*mut_node)->source_range, 11, "Dynamic objects");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_union(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_UNION);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  frg_message_emit_eft_1_unsupported_requirement(
    verifier->mut_message_buffer, &(*mut_node)->source_range, 9, "Union declarations");

  return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_property(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_PROPERTY);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_declaration_property_t* declaration_property
    = (frg_ast_node_declaration_property_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if ((declaration_property->flags & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_OPTIONAL)
        != 0
      || (declaration_property->flags
          & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NON_OPTIONAL)
           != 0) {
    frg_message_emit_eft_1_unsupported_requirement(
      verifier->mut_message_buffer, &(*mut_node)->source_range, 15, "Optionals");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if ((declaration_property->flags & FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_SPREAD)
      != 0) {
    frg_message_emit_eft_1_unsupported_requirement(
      verifier->mut_message_buffer, &(*mut_node)->source_range, 16, "Spreads");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (declaration_property->type == NULL) {
    frg_message_emit_eft_1_unsupported_requirement(
      verifier->mut_message_buffer, &(*mut_node)->source_range, 11, "Dynamic objects");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_interface(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_INTERFACE);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  frg_message_emit_eft_1_unsupported_requirement(verifier->mut_message_buffer,
                                                 &(*mut_node)->source_range,
                                                 2,
                                                 "Interface declarations");

  return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_function_argument(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind,
                          FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_declaration_function_argument_t* declaration_function_argument
    = (frg_ast_node_declaration_function_argument_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if ((declaration_function_argument->flags
       & FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_KEYWORD)
      != 0) {
    frg_message_emit_eft_2_unsupported_requirement_subitem(
      verifier->mut_message_buffer,
      &(*mut_node)->source_range,
      14,
      "Function declarations",
      2,
      "Variadic keyword arguments");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  if (declaration_function_argument->default_value != NULL) {
    frg_message_emit_eft_2_unsupported_requirement_subitem(verifier->mut_message_buffer,
                                                           &(*mut_node)->source_range,
                                                           14,
                                                           "Function declarations",
                                                           3,
                                                           "Default argument values");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_declaration_function(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_declaration_function_t* declaration_function
    = (frg_ast_node_declaration_function_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if ((declaration_function->flags & FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_MUTABLE)
        != 0
      || (declaration_function->flags & FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_OVERRIDE)
           != 0) {
    frg_message_emit_eft_2_unsupported_requirement_subitem(verifier->mut_message_buffer,
                                                           &(*mut_node)->source_range,
                                                           14,
                                                           "Function declarations",
                                                           4,
                                                           "Overriding");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_value_character(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_VALUE_CHARACTER);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  frg_message_emit_eft_1_unsupported_requirement(
    verifier->mut_message_buffer, &(*mut_node)->source_range, 5, "Character literals");

  return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_value_string(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_VALUE_STRING);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  frg_message_emit_eft_1_unsupported_requirement(
    verifier->mut_message_buffer, &(*mut_node)->source_range, 4, "String literals");

  return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_value_call_keyword_argument(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind,
                          FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  frg_message_emit_eft_2_unsupported_requirement_subitem(verifier->mut_message_buffer,
                                                         &(*mut_node)->source_range,
                                                         14,
                                                         "Function declarations",
                                                         2,
                                                         "Variadic keyword arguments");

  return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_value_call(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_int_equal_to((*mut_node)->kind, FRG_AST_NODE_KIND_VALUE_CALL);
  frg_assert_pointer_non_null(mut_user_data);

  frg_ast_node_value_call_t* value_call = (frg_ast_node_value_call_t*)*mut_node;
  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  if (value_call->keyword_arguments != NULL) {
    frg_message_emit_eft_2_unsupported_requirement_subitem(
      verifier->mut_message_buffer,
      &(*mut_node)->source_range,
      14,
      "Function declarations",
      2,
      "Variadic keyword arguments");

    return FRG_AST_VISITOR_STATUS_SKIP;
  }

  return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t
  frg_verification_supported_language_features_handle_enter_operator(
    frg_ast_node_t** mut_node, void* mut_user_data, const GList* parents) {
  frg_assert_pointer_non_null(mut_node);
  frg_assert_pointer_non_null(*mut_node);
  frg_assert_pointer_non_null(mut_user_data);

  frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)mut_user_data;

  frg_message_emit_eft_1_unsupported_requirement(
    verifier->mut_message_buffer, &(*mut_node)->source_range, 7, "Operators");

  return FRG_AST_VISITOR_STATUS_SKIP;
}

void frg_verification_supported_language_features_add_handlers(
  frg_ast_visitor_t* mut_visitor) {}
