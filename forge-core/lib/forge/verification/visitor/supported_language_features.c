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

#include <forge/verification/visitor/supported_language_features.h>
#include <forge/verification/verifier.h>
#include <forge/messages/codes.h>
#include <forge/common/error.h>

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_ty_fn(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_TY_FN);
    frg_assert_pointer_non_null(user_data);

    frg_ast_ty_fn_t* ty_fn = (frg_ast_ty_fn_t*)*ast;
    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    if (ty_fn->var_pos_args != NULL) {
        frg_message_emit_eft_2_unsupported_requirement_subitem(
            verifier->message_buffer,
            &(*ast)->source_range,
            14,
            "Function declarations",
            1,
            "Variable positional arguments"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    if (ty_fn->var_kw_args != NULL) {
        frg_message_emit_eft_2_unsupported_requirement_subitem(
            verifier->message_buffer,
            &(*ast)->source_range,
            14,
            "Function declarations",
            2,
            "Variable keyword arguments"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    if (ty_fn->return_ty == NULL) {
        frg_message_emit_eft_1_unsupported_requirement(
            verifier->message_buffer,
            &(*ast)->source_range,
            11,
            "Dynamic objects"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_union(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_UNION);
    frg_assert_pointer_non_null(user_data);

    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    frg_message_emit_eft_1_unsupported_requirement(
        verifier->message_buffer,
        &(*ast)->source_range,
        9,
        "Union declarations"
    );

    return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_prop(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_PROP);
    frg_assert_pointer_non_null(user_data);

    frg_ast_decl_prop_t* decl_prop = (frg_ast_decl_prop_t*)*ast;
    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    if ((decl_prop->flags & FRG_AST_DECL_PROP_FLAG_OPTIONAL) != 0 || (decl_prop->flags & FRG_AST_DECL_PROP_FLAG_NON_OPTIONAL) != 0) {
        frg_message_emit_eft_1_unsupported_requirement(
            verifier->message_buffer,
            &(*ast)->source_range,
            15,
            "Optionals"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    if ((decl_prop->flags & FRG_AST_DECL_PROP_FLAG_SPREAD) != 0) {
        frg_message_emit_eft_1_unsupported_requirement(
            verifier->message_buffer,
            &(*ast)->source_range,
            16,
            "Spreads"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    if (decl_prop->ty == NULL) {
        frg_message_emit_eft_1_unsupported_requirement(
            verifier->message_buffer,
            &(*ast)->source_range,
            11,
            "Dynamic objects"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_iface(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_IFACE);
    frg_assert_pointer_non_null(user_data);

    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    frg_message_emit_eft_1_unsupported_requirement(
        verifier->message_buffer,
        &(*ast)->source_range,
        2,
        "Interface declarations"
    );

    return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_fn_arg(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_FN_ARG);
    frg_assert_pointer_non_null(user_data);

    frg_ast_decl_fn_arg_t* decl_fn_arg = (frg_ast_decl_fn_arg_t*)*ast;
    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    if ((decl_fn_arg->flags & FRG_AST_DECL_FN_ARG_FLAG_KW) != 0) {
        frg_message_emit_eft_2_unsupported_requirement_subitem(
            verifier->message_buffer,
            &(*ast)->source_range,
            14,
            "Function declarations",
            2,
            "Variadic keyword arguments"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    if (decl_fn_arg->default_value != NULL) {
        frg_message_emit_eft_2_unsupported_requirement_subitem(
            verifier->message_buffer,
            &(*ast)->source_range,
            14,
            "Function declarations",
            3,
            "Default argument values"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_decl_fn(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_FN);
    frg_assert_pointer_non_null(user_data);

    frg_ast_decl_fn_t* decl_fn = (frg_ast_decl_fn_t*)*ast;
    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    if ((decl_fn->flags & FRG_AST_DECL_FN_FLAG_MUT) != 0 || (decl_fn->flags & FRG_AST_DECL_FN_FLAG_OVERRIDE) != 0) {
        frg_message_emit_eft_2_unsupported_requirement_subitem(
            verifier->message_buffer,
            &(*ast)->source_range,
            14,
            "Function declarations",
            4,
            "Overriding"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_value_char(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_VALUE_CHAR);
    frg_assert_pointer_non_null(user_data);

    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    frg_message_emit_eft_1_unsupported_requirement(
        verifier->message_buffer,
        &(*ast)->source_range,
        5,
        "Character literals"
    );

    return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_value_str(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_VALUE_STR);
    frg_assert_pointer_non_null(user_data);

    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    frg_message_emit_eft_1_unsupported_requirement(
        verifier->message_buffer,
        &(*ast)->source_range,
        4,
        "String literals"
    );

    return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_value_call_kw_arg(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_VALUE_CALL_KW_ARG);
    frg_assert_pointer_non_null(user_data);

    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    frg_message_emit_eft_2_unsupported_requirement_subitem(
        verifier->message_buffer,
        &(*ast)->source_range,
        14,
        "Function declarations",
        2,
        "Variadic keyword arguments"
    );

    return FRG_AST_VISITOR_STATUS_SKIP;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_value_call(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_VALUE_CALL);
    frg_assert_pointer_non_null(user_data);

    frg_ast_value_call_t* value_call = (frg_ast_value_call_t*)*ast;
    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    if (value_call->kw_args != NULL) {
        frg_message_emit_eft_2_unsupported_requirement_subitem(
            verifier->message_buffer,
            &(*ast)->source_range,
            14,
            "Function declarations",
            2,
            "Variadic keyword arguments"
        );

        return FRG_AST_VISITOR_STATUS_SKIP;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_supported_language_features_callback_pre_operator(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_pointer_non_null(user_data);

    frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    frg_message_emit_eft_1_unsupported_requirement(
        verifier->message_buffer,
        &(*ast)->source_range,
        7,
        "Operators"
    );

    return FRG_AST_VISITOR_STATUS_SKIP;
}
