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

#include <forge/formatting/formatters.h>

void frg_format_ty_bool(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_BOOL);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "bool");
}

void frg_format_ty_u8(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_U8);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "u8");
}

void frg_format_ty_u16(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_U16);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "u16");
}

void frg_format_ty_u32(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_U32);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "u32");
}

void frg_format_ty_u64(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_U64);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "u64");
}

void frg_format_ty_i8(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_I8);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "i8");
}

void frg_format_ty_i16(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_I16);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "i16");
}

void frg_format_ty_i32(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_I32);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "i32");
}

void frg_format_ty_i64(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_I64);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "i64");
}

void frg_format_ty_f32(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_F32);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "f32");
}

void frg_format_ty_f64(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_F64);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "f64");
}

void frg_format_ty_symbol(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_SYMBOL);
    frg_assert_int_non_negative(indent);

    frg_assert_gstring_non_empty(((frg_ast_ty_symbol_t*)ast)->name);
    frg_stream_output_write_string(output, ((frg_ast_ty_symbol_t*)ast)->name->str);
}

void frg_format_ty_pointer(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_POINTER);
    frg_assert_int_non_negative(indent);

    frg_stream_output_write_string(output, "*");
    frg_format(output, ((frg_ast_ty_pointer_t*)ast)->value, indent);
}

void frg_format_ty_fn(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_FN);
    frg_assert_int_non_negative(indent);

    ty_fn = (frg_ast_ty_fn_t*)ast;

    frg_assert_pointer_null(ty_fn->var_pos_args);
    frg_assert_pointer_null(ty_fn->var_kw_args);
    frg_assert_pointer_non_null(ty_fn->return_ty);

    frg_stream_output_write_string(output, "(");

    for (GList* iter = ty_fn->args; iter != NULL; iter = iter->next) {
        frg_format(output, iter->data, indent);

        if (iter->next != NULL) {
            frg_stream_output_write_string(output, ", ");
        }
    }

    frg_stream_output_write_string(output, ") -> ");

    frg_format(output, ty_fn->return_ty, indent);
}

void frg_format_decl_prop(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_PROP);
    frg_assert_int_non_negative(indent);

}

void frg_format_decl_fn_arg(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN_ARG);
    frg_assert_int_non_negative(indent);

}
