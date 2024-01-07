// Copyright (c) 2024 Sophie Katz
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

#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/verification/verifier.h>
#include <forge/verification/supported_language_features.h>

frg_verification_verifier_t* frg_verification_verifier_new() {
    frg_verification_verifier_t* verifier = frg_safe_malloc(sizeof(frg_verification_verifier_t));
    verifier->scope = frg_ast_scope_new();
    return verifier;
}

void frg_verification_verifier_destroy(frg_verification_verifier_t** verifier) {
    frg_assert_pointer_non_null(verifier);
    frg_assert_pointer_non_null(*verifier);

    frg_ast_scope_destroy(&(*verifier)->scope);
    frg_safe_free((void**)verifier);
}

frg_ast_visitor_t* frg_verification_new_visitor(frg_verification_verifier_t* verifier) {
    frg_ast_visitor_t* visitor = frg_ast_visitor_new(verifier);

    // Supported language features
    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_TY_BOOL,
        frg_verification_supported_language_features_callback_pre_ty_bool,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_TY_F32,
        frg_verification_supported_language_features_callback_pre_ty_float,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_TY_F64,
        frg_verification_supported_language_features_callback_pre_ty_float,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_TY_SYMBOL,
        frg_verification_supported_language_features_callback_pre_ty_symbol,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_TY_FN,
        frg_verification_supported_language_features_callback_pre_ty_fn,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_UNION,
        frg_verification_supported_language_features_callback_pre_decl_union,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_STRUCT,
        frg_verification_supported_language_features_callback_pre_decl_struct,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_PROP,
        frg_verification_supported_language_features_callback_pre_decl_prop,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_IFACE,
        frg_verification_supported_language_features_callback_pre_decl_iface,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_FN_ARG,
        frg_verification_supported_language_features_callback_pre_decl_fn_arg,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_FN,
        frg_verification_supported_language_features_callback_pre_decl_fn,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_DECL_VAR,
        frg_verification_supported_language_features_callback_pre_decl_var,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_STMT_IF,
        frg_verification_supported_language_features_callback_pre_stmt_if,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_STMT_WHILE,
        frg_verification_supported_language_features_callback_pre_stmt_while,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_TRUE,
        frg_verification_supported_language_features_callback_pre_value_bool,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_FALSE,
        frg_verification_supported_language_features_callback_pre_value_bool,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_FLOAT,
        frg_verification_supported_language_features_callback_pre_value_float,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_CHAR,
        frg_verification_supported_language_features_callback_pre_value_char,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_STR,
        frg_verification_supported_language_features_callback_pre_value_str,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_CALL_KW_ARG,
        frg_verification_supported_language_features_callback_pre_value_call_kw_arg,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_CALL,
        frg_verification_supported_language_features_callback_pre_value_call,
        NULL
    );

    frg_ast_visitor_add_entry(
        visitor,
        FRG_AST_ID_VALUE_ACCESS,
        frg_verification_supported_language_features_callback_pre_value_access,
        NULL
    );

    return visitor;
}
