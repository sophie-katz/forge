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

#include <forge/verification/type_operations.h>
#include <forge/common/error.h>
#include <forge/ast/kind_info.h>

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

