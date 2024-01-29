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

#include <forge/ast/type_resolution.h>
#include <forge/ast/kind_info.h>
#include <forge/messages/codes.h>
#include <forge/common/error.h>
#include <forge/formatting/format.h>

frg_ast_t* frg_ast_get_numeric_containing_type(
    frg_ast_t* a,
    frg_ast_t* b
) {
    frg_assert_pointer_non_null(a);
    frg_assert_pointer_non_null(b);

    bool is_a_int_unsigned = frg_ast_kind_info_get(a->kind)->flags & FRG_AST_KIND_FLAG_TY_INT_UNSIGNED;
    bool is_b_int_unsigned = frg_ast_kind_info_get(b->kind)->flags & FRG_AST_KIND_FLAG_TY_INT_UNSIGNED;
    bool is_a_int_signed = frg_ast_kind_info_get(a->kind)->flags & FRG_AST_KIND_FLAG_TY_INT_SIGNED;
    bool is_b_int_signed = frg_ast_kind_info_get(b->kind)->flags & FRG_AST_KIND_FLAG_TY_INT_SIGNED;
    bool is_a_float = frg_ast_kind_info_get(a->kind)->flags & FRG_AST_KIND_FLAG_TY_FLOAT;
    bool is_b_float = frg_ast_kind_info_get(b->kind)->flags & FRG_AST_KIND_FLAG_TY_FLOAT;
    
    if (is_a_int_unsigned && is_b_int_unsigned) {
        if (frg_ast_kind_info_get(a->kind)->ty_bit_width == 64 || frg_ast_kind_info_get(b->kind)->ty_bit_width == 64) {
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_U64
            );
        } else {
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_U32
            );
        }
    } else if (is_a_int_signed && is_b_int_signed) {
        if (frg_ast_kind_info_get(a->kind)->ty_bit_width == 64 || frg_ast_kind_info_get(b->kind)->ty_bit_width == 64) {
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_I64
            );
        } else {
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_I32
            );
        }
    } else if (is_a_int_signed && is_b_int_unsigned) {
        if (frg_ast_kind_info_get(a->kind)->ty_bit_width <= 32 && frg_ast_kind_info_get(b->kind)->ty_bit_width <= 16) {
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_I32
            );
        } else {
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_I64
            );
        }
    } else if (is_a_int_unsigned && is_b_int_signed) {
        return frg_ast_get_numeric_containing_type(b, a);
    } else if (is_a_float && is_b_float) {
        if (frg_ast_kind_info_get(a->kind)->ty_bit_width == 64 || frg_ast_kind_info_get(b->kind)->ty_bit_width == 64) {
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_F64
            );
        } else {
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_F32
            );
        }
    } else {
        return frg_ast_new_ty_primary(
            &frg_parsing_range_null,
            FRG_AST_KIND_TY_F64
        );
    }
}

frg_ast_t* frg_ast_resolve_decl_type(
    frg_message_buffer_t* message_buffer,
    frg_ast_scope_t* scope,
    frg_ast_t* decl
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(decl);

    frg_ast_t* result = NULL;
    frg_ast_decl_prop_t* prop = NULL;
    frg_ast_ty_fn_t* ty_fn = NULL;

    switch (decl->kind) {
        case FRG_AST_KIND_DECL_UNION:
            frg_message_emit_eft_1_unsupported_requirement(
                message_buffer,
                &decl->source_range,
                9,
                "Union declarations"
            );

            return NULL;
        case FRG_AST_KIND_DECL_STRUCT:
            frg_message_emit_eft_1_unsupported_requirement(
                message_buffer,
                &decl->source_range,
                10,
                "Struct declarations"
            );

            return NULL;
        case FRG_AST_KIND_DECL_PROP:
            result = ((frg_ast_decl_prop_t*)decl)->ty;

            if (result == NULL) {
                frg_message_emit_eft_1_unsupported_requirement(
                    message_buffer,
                    &decl->source_range,
                    11,
                    "Dynamic objects"
                );
            }

            return frg_ast_clone(result);
        case FRG_AST_KIND_DECL_IFACE:
            frg_message_emit_eft_1_unsupported_requirement(
                message_buffer,
                &decl->source_range,
                11,
                "Interface declarations"
            );

            return NULL;
        case FRG_AST_KIND_DECL_FN_ARG:
            prop = ((frg_ast_decl_fn_arg_t*)decl)->prop;

            frg_assert_pointer_non_null(prop);

            return frg_ast_resolve_decl_type(
                message_buffer,
                scope,
                (frg_ast_t*)prop
            );
        case FRG_AST_KIND_DECL_FN:
            ty_fn = ((frg_ast_decl_fn_t*)decl)->ty;

            frg_assert_pointer_non_null(ty_fn);

            return frg_ast_clone((frg_ast_t*)ty_fn);
        case FRG_AST_KIND_DECL_VAR:
            prop = ((frg_ast_decl_var_t*)decl)->prop;

            frg_assert_pointer_non_null(prop);

            return frg_ast_resolve_decl_type(
                message_buffer,
                scope,
                (frg_ast_t*)prop
            );
        default:
            frg_die_unexpected_enum_value(decl->kind);
    }
}

frg_ast_t* frg_ast_resolve_value_type(
    frg_message_buffer_t* message_buffer,
    frg_ast_scope_t* scope,
    frg_ast_t* value
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(scope);
    frg_assert_pointer_non_null(value);

    frg_ast_t* ast = NULL;
    frg_ast_value_int_t* value_int = NULL;
    frg_ast_value_float_t* value_float = NULL;
    frg_ast_value_symbol_t* value_symbol = NULL;
    frg_ast_value_unary_t* value_unary = NULL;
    frg_ast_value_call_t* value_call = NULL;
    frg_ast_value_binary_t* value_binary = NULL;
    frg_ast_t* type_left = NULL;
    frg_ast_t* type_right = NULL;

    switch (value->kind) {
        case FRG_AST_KIND_VALUE_TRUE:
        case FRG_AST_KIND_VALUE_FALSE:
        case FRG_AST_KIND_VALUE_EQ:
        case FRG_AST_KIND_VALUE_NE:
        case FRG_AST_KIND_VALUE_LT:
        case FRG_AST_KIND_VALUE_LE:
        case FRG_AST_KIND_VALUE_GT:
        case FRG_AST_KIND_VALUE_GE:
        case FRG_AST_KIND_VALUE_LOG_NOT:
        case FRG_AST_KIND_VALUE_LOG_AND:
        case FRG_AST_KIND_VALUE_LOG_OR:
            return frg_ast_new_ty_primary(
                &frg_parsing_range_null,
                FRG_AST_KIND_TY_BOOL
            );
        case FRG_AST_KIND_VALUE_INT:
            value_int = (frg_ast_value_int_t*)value;

            frg_assert_pointer_non_null(value_int->ty);

            return frg_ast_clone(value_int->ty);
        case FRG_AST_KIND_VALUE_FLOAT:
            value_float = (frg_ast_value_float_t*)value;

            frg_assert_pointer_non_null(value_float->ty);

            return frg_ast_clone(value_float->ty);
        case FRG_AST_KIND_VALUE_CHAR:
            frg_message_emit_eft_1_unsupported_requirement(
                message_buffer,
                &value->source_range,
                5,
                "Character literals"
            );

            return NULL;
        case FRG_AST_KIND_VALUE_STR:
            frg_message_emit_eft_1_unsupported_requirement(
                message_buffer,
                &value->source_range,
                4,
                "String literals"
            );

            return NULL;
        case FRG_AST_KIND_VALUE_SYMBOL:
            value_symbol = (frg_ast_value_symbol_t*)value;

            frg_assert_gstring_non_empty(value_symbol->name);

            ast = frg_ast_scope_get_ast(
                scope,
                ((frg_ast_value_symbol_t*)value)->name->str
            );

            if (ast == NULL) {
                frg_emit_message_et_1_undeclared_symbol(
                    message_buffer,
                    &value->source_range,
                    value_symbol->name
                );

                return NULL;
            }

            return frg_ast_resolve_decl_type(
                message_buffer,
                scope,
                ast
            );
        case FRG_AST_KIND_VALUE_DEREF:
            value_unary = (frg_ast_value_unary_t*)value;

            ast = frg_ast_resolve_value_type(
                message_buffer,
                scope,
                value_unary->operand
            );

            if (ast == NULL) {
                return NULL;
            }

            if (ast->kind != FRG_AST_KIND_TY_POINTER) {
                frg_emit_message_et_2_cannot_deref_non_pointer(
                    message_buffer,
                    &value->source_range,
                    ast
                );

                return NULL;
            }

            frg_assert_pointer_non_null(((frg_ast_ty_pointer_t*)ast)->value);

            return frg_ast_clone(((frg_ast_ty_pointer_t*)ast)->value);
        case FRG_AST_KIND_VALUE_GETADDR:
            value_unary = (frg_ast_value_unary_t*)value;

            ast = frg_ast_resolve_value_type(
                message_buffer,
                scope,
                value_unary->operand
            );

            if (ast == NULL) {
                return NULL;
            }

            return (frg_ast_t*)frg_ast_new_ty_pointer(
                &frg_parsing_range_null,
                ast
            );
        case FRG_AST_KIND_VALUE_CALL:
            value_call = (frg_ast_value_call_t*)value;

            frg_assert_pointer_non_null(value_call->callee);

            ast = frg_ast_resolve_value_type(
                message_buffer,
                scope,
                value_call->callee
            );

            if (ast == NULL) {
                return NULL;
            }

            if (ast->kind != FRG_AST_KIND_TY_FN) {
                frg_emit_message_et_3_cannot_call_non_fn(
                    message_buffer,
                    &value->source_range,
                    ast
                );

                return NULL;
            }

            frg_assert_pointer_non_null(((frg_ast_ty_fn_t*)ast)->return_ty);

            return frg_ast_clone(((frg_ast_ty_fn_t*)ast)->return_ty);
        case FRG_AST_KIND_VALUE_ACCESS:
            frg_message_emit_eft_1_unsupported_requirement(
                message_buffer,
                &value->source_range,
                10,
                "Struct declarations"
            );

            return NULL;
        case FRG_AST_KIND_VALUE_BIT_NOT:
        case FRG_AST_KIND_VALUE_NEG:
            value_unary = (frg_ast_value_unary_t*)value;

            frg_assert_pointer_non_null(value_unary->operand);

            return frg_ast_resolve_value_type(
                message_buffer,
                scope,
                value_unary->operand
            );
        case FRG_AST_KIND_VALUE_BIT_AND:
        case FRG_AST_KIND_VALUE_BIT_OR:
        case FRG_AST_KIND_VALUE_BIT_XOR:
        case FRG_AST_KIND_VALUE_BIT_SHL:
        case FRG_AST_KIND_VALUE_BIT_SHR:
        case FRG_AST_KIND_VALUE_ADD:
        case FRG_AST_KIND_VALUE_SUB:
        case FRG_AST_KIND_VALUE_MUL:
        case FRG_AST_KIND_VALUE_MOD:
        case FRG_AST_KIND_VALUE_DIV:
        case FRG_AST_KIND_VALUE_DIV_INT:
        case FRG_AST_KIND_VALUE_EXP:
            value_binary = (frg_ast_value_binary_t*)value;

            frg_assert_pointer_non_null(value_binary->left);
            frg_assert_pointer_non_null(value_binary->right);

            type_left = frg_ast_resolve_value_type(
                message_buffer,
                scope,
                value_binary->left
            );

            if (type_left == NULL) {
                return NULL;
            }

            type_right = frg_ast_resolve_value_type(
                message_buffer,
                scope,
                value_binary->right
            );

            if (type_right == NULL) {
                return NULL;
            }

            ast = frg_ast_get_numeric_containing_type(
                type_left,
                type_right
            );

            frg_ast_destroy(&type_left);
            frg_ast_destroy(&type_right);

            if (ast == NULL) {
                frg_emit_message_it_1_no_containing_type(
                    message_buffer,
                    &value->source_range,
                    type_left,
                    type_right
                );
            }

            return ast;
        case FRG_AST_KIND_VALUE_ASSIGN:
        case FRG_AST_KIND_VALUE_BIT_AND_ASSIGN:
        case FRG_AST_KIND_VALUE_BIT_OR_ASSIGN:
        case FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN:
        case FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN:
        case FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN:
        case FRG_AST_KIND_VALUE_ADD_ASSIGN:
        case FRG_AST_KIND_VALUE_INC:
        case FRG_AST_KIND_VALUE_SUB_ASSIGN:
        case FRG_AST_KIND_VALUE_DEC:
        case FRG_AST_KIND_VALUE_MUL_ASSIGN:
        case FRG_AST_KIND_VALUE_DIV_ASSIGN:
        case FRG_AST_KIND_VALUE_DIV_INT_ASSIGN:
        case FRG_AST_KIND_VALUE_MOD_ASSIGN:
        case FRG_AST_KIND_VALUE_EXP_ASSIGN:
        case FRG_AST_KIND_VALUE_LOG_AND_ASSIGN:
        case FRG_AST_KIND_VALUE_LOG_OR_ASSIGN:
            value_binary = (frg_ast_value_binary_t*)value;

            frg_assert_pointer_non_null(value_binary->left);

            return frg_ast_resolve_value_type(
                message_buffer,
                scope,
                value_binary->left
            );
        default:
            frg_die_unexpected_enum_value(value->kind);
    }
}
