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

#include <forge/formatting/format.h>
#include <forge/common/error.h>

void frg_format(frg_stream_output_t *output, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(output);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_non_negative(indent);

    frg_ast_ty_fn_t* ty_fn = NULL;
    frg_ast_decl_prop_t* decl_prop = NULL;
    frg_ast_decl_fn_arg_t* decl_fn_arg = NULL;

    switch (ast->id) {
        case FRG_AST_ID_TY_BOOL:
            frg_stream_output_write_string(output, "bool");
            break;
        case FRG_AST_ID_TY_U8:
            frg_stream_output_write_string(output, "u8");
            break;
        case FRG_AST_ID_TY_U16:
            frg_stream_output_write_string(output, "u16");
            break;
        case FRG_AST_ID_TY_U32:
            frg_stream_output_write_string(output, "u32");
            break;
        case FRG_AST_ID_TY_U64:
            frg_stream_output_write_string(output, "u64");
            break;
        case FRG_AST_ID_TY_I8:
            frg_stream_output_write_string(output, "i8");
            break;
        case FRG_AST_ID_TY_I16:
            frg_stream_output_write_string(output, "i16");
            break;
        case FRG_AST_ID_TY_I32:
            frg_stream_output_write_string(output, "i32");
            break;
        case FRG_AST_ID_TY_I64:
            frg_stream_output_write_string(output, "i64");
            break;
        case FRG_AST_ID_TY_F32:
            frg_stream_output_write_string(output, "f32");
            break;
        case FRG_AST_ID_TY_F64:
            frg_stream_output_write_string(output, "f64");
            break;
        case FRG_AST_ID_TY_SYMBOL:
            frg_assert_gstring_non_empty(((frg_ast_ty_symbol_t*)ast)->name);
            frg_stream_output_write_string(output, ((frg_ast_ty_symbol_t*)ast)->name->str);
            break;
        case FRG_AST_ID_TY_POINTER:
            frg_stream_output_write_string(output, "*");
            frg_format(output, ((frg_ast_ty_pointer_t*)ast)->value, indent);
            break;
        case FRG_AST_ID_TY_FN:
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

            break;
        case FRG_AST_ID_DECL_PROP:
            decl_prop = (frg_ast_decl_prop_t*)ast;

            if ((decl_prop->flags & FRG_AST_DECL_PROP_FLAG_SPREAD) != 0) {
                frg_stream_output_write_string(output, "...");
            }

            frg_stream_output_write_string(output, decl_prop->name->str);

            if ((decl_prop->flags & FRG_AST_DECL_PROP_FLAG_OPTIONAL) != 0) {
                frg_stream_output_write_string(output, "?");
            } else if ((decl_prop->flags & FRG_AST_DECL_PROP_FLAG_NON_OPTIONAL) != 0) {
                frg_stream_output_write_string(output, "!");
            }

            if (decl_prop->ty != NULL) {
                frg_stream_output_write_string(output, ": ");
                frg_format(output, decl_prop->ty, indent);
            }

            break;
        case FRG_AST_ID_DECL_FN_ARG:
            decl_fn_arg = (frg_ast_decl_fn_arg_t*)ast;

            if ((decl_fn_arg->flags & FRG_AST_DECL_FN_ARG_FLAG_KW) != 0) {
                frg_stream_output_write_string(output, "kw ");
            }

            frg_format(output, (frg_ast_t*)decl_fn_arg->prop, indent);

            if (decl_fn_arg->default_value != NULL) {
                frg_stream_output_write_string(output, " = ");
                frg_format(output, decl_fn_arg->default_value, indent);
            }

            break;
        default:
            frg_die_unexpected_enum_value(ast->id);
    }
}
