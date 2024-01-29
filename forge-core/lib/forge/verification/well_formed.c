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

#include <forge/verification/well_formed.h>
#include <forge/verification/verifier.h>
#include <forge/messages/codes.h>
#include <forge/common/error.h>
#include <utf8proc.h>
#include <forge/ast/kind_info.h>

bool _frg_verification_well_formed_is_valid_symbol_codepoint_first(utf8proc_int32_t codepoint) {
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

bool _frg_verification_well_formed_is_valid_symbol_codepoint(utf8proc_int32_t codepoint) {
    if (codepoint >= '0' && codepoint <= '9') {
        return true;
    }

    return _frg_verification_well_formed_is_valid_symbol_codepoint_first(codepoint);
}

bool _frg_verification_well_formed_is_valid_symbol(const GString* name) {
    for (const gchar* iter = name->str; *iter != 0;) {
        utf8proc_int32_t codepoint = 0;

        utf8proc_ssize_t bytes_read = utf8proc_iterate(
            (const utf8proc_uint8_t*)iter,
            name->len - (iter - name->str),
            &codepoint
        );

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

frg_ast_visitor_status_t frg_verification_well_formed_callback_ty_symbol(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_TY_SYMBOL);
    frg_assert_pointer_non_null(user_data);

    frg_die_ast_kind_not_yet_supported((*ast)->kind);

    // frg_ast_ty_symbol_t* ty_symbol = (frg_ast_ty_symbol_t*)*ast;
    // frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    // if (ty_symbol->name == NULL) {
    //     frg_message_emit_is_5_ast_property_null(
    //         verifier->message_buffer,
    //         &(*ast)->source_range,
    //         (*ast)->kind,
    //         "name"
    //     );

    //     return FRG_AST_VISITOR_STATUS_SKIP;
    // }
    
    // if (ty_symbol->name->len == 0) {
    //     frg_message_emit_is_6_ast_property_empty_string(
    //         verifier->message_buffer,
    //         &(*ast)->source_range,
    //         (*ast)->kind,
    //         "name"
    //     );

    //     return FRG_AST_VISITOR_STATUS_SKIP;
    // }
    
    // if (!_frg_verification_well_formed_is_valid_symbol(ty_symbol->name)) {
    //     frg_message_emit_is_6_ast_property_invalid_symbol(
    //         verifier->message_buffer,
    //         &(*ast)->source_range,
    //         (*ast)->kind,
    //         "name"
    //     );

    //     return FRG_AST_VISITOR_STATUS_SKIP;
    // }

    // return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_callback_ty_pointer(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_TY_POINTER);
    frg_assert_pointer_non_null(user_data);

    frg_die_ast_kind_not_yet_supported((*ast)->kind);

    // frg_ast_ty_pointer_t* ty_pointer = (frg_ast_ty_pointer_t*)*ast;
    // frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    // if (ty_pointer->value == NULL) {
    //     frg_message_emit_is_5_ast_property_null(
    //         verifier->message_buffer,
    //         &(*ast)->source_range,
    //         (*ast)->kind,
    //         "value"
    //     );

    //     return FRG_AST_VISITOR_STATUS_SKIP;
    // }

    // return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_verification_well_formed_callback_ty_fn(
    GList* parents,
    frg_ast_t** ast,
    void* user_data
) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_TY_FN);
    frg_assert_pointer_non_null(user_data);

    frg_die_ast_kind_not_yet_supported((*ast)->kind);

    // frg_ast_ty_fn_t* ty_fn = (frg_ast_ty_fn_t*)*ast;
    // frg_verification_verifier_t* verifier = (frg_verification_verifier_t*)user_data;

    // size_t index = 0;
    // for (GList* iter = ty_fn->args; iter != NULL; iter = iter->next) {
    //     frg_ast_t* arg = (frg_ast_t*)iter->data;

    //     if (arg == NULL) {
    //         frg_message_emit_is_8_ast_property_null_element(
    //             verifier->message_buffer,
    //             &(*ast)->source_range,
    //             (*ast)->kind,
    //             "args",
    //             index
    //         );

    //         return FRG_AST_VISITOR_STATUS_SKIP;
    //     }

    //     if (arg->kind != FRG_AST_KIND_DECL_FN_ARG) {
    //         frg_message_emit_is_9_ast_property_unexpected_element(
    //             verifier->message_buffer,
    //             &(*ast)->source_range,
    //             (*ast)->kind,
    //             "args",
    //             index
    //         );
            
    //         return FRG_AST_VISITOR_STATUS_SKIP;
    //     }

    //     frg_ast_decl_fn_arg_t* decl_fn_arg = (frg_ast_decl_fn_arg_t*)arg;

    //     if (decl_fn_arg->flags & FRG_AST_DECL_FN_ARG_FLAG_KW) {
    //         frg_die_message_no_code_yet();
    //     }

    //     index++;
    // }

    // if (ty_fn->var_pos_args != NULL) {
    //     // TODO: Keep implementing this function
    // }

    // return FRG_AST_VISITOR_STATUS_OK;
}
