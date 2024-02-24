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
#include <forge/memory.h>
#include <forge/verification/verifier.h>
#include <forge/verification/visitor/supported_language_features.h>

frg_verification_verifier_t* frg_verification_verifier_new(
    frg_message_buffer_t* mut_message_buffer,
    frg_ast_scope_t* mut_scope
) {
    frg_verification_verifier_t* verifier = frg_malloc(sizeof(frg_verification_verifier_t));
    verifier->mut_message_buffer = mut_message_buffer;
    verifier->mut_scope = mut_scope;
    return verifier;
}

void frg_verification_verifier_destroy(frg_verification_verifier_t* verifier) {
    frg_assert_pointer_non_null(verifier);

    frg_free(verifier);
}

frg_ast_visitor_t* frg_verification_verifier_new_visitor(
    frg_verification_verifier_t* mut_verifier
) {
    frg_ast_visitor_t* visitor = frg_ast_visitor_new(mut_verifier);

    // Supported language features
    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_type_FUNCTION,
    //     frg_verification_supported_language_features_callback_type_function,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_DECLARATION_UNION,
    //     frg_verification_supported_language_features_callback_pre_declaration_union,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_DECLARATION_PROP,
    //     frg_verification_supported_language_features_callback_pre_declaration_prop,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_DECLARATION_INTERFACE,
    //     frg_verification_supported_language_features_callback_pre_declaration_interface,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_DECLARATION_FUNCTION_ARG,
    //     frg_verification_supported_language_features_callback_pre_declaration_function_arg,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_DECLARATION_FUNCTION,
    //     frg_verification_supported_language_features_callback_pre_declaration_function,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_CHAR,
    //     frg_verification_supported_language_features_callback_pre_value_char,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_STR,
    //     frg_verification_supported_language_features_callback_pre_value_str,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_CALL_KEYWORD_ARG,
    //     frg_verification_supported_language_features_callback_pre_value_call_keyword_arg,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_CALL,
    //     frg_verification_supported_language_features_callback_pre_value_call,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_BIT_AND_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_BIT_OR_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_ADD_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_INC,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_SUB_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_DEC,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_MUL_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_DIV_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_DIV_INT_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_MOD_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_EXP_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );
    
    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_LOG_AND_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    // frg_ast_visitor_add_entry(
    //     visitor,
    //     FRG_AST_KIND_VALUE_LOG_OR_ASSIGN,
    //     frg_verification_supported_language_features_callback_pre_operator,
    //     NULL
    // );

    return visitor;
}
