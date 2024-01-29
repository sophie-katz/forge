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

#include <forge/ast/debug_printers.h>
#include <forge/common/error.h>
#include <forge/common/debug.h>
#include <forge/ast/operations.h>
#include <forge/ast/kind_info.h>

typedef struct {
    frg_stream_output_t* stream;
    const frg_indent_t indent;
    const char* label;
    size_t index;
} _frg_ast_print_debug_list_ctx_t;

void _frg_ast_print_debug_list_gfunc(gpointer data, gpointer user_data) {
    _frg_ast_print_debug_list_ctx_t* ctx = (_frg_ast_print_debug_list_ctx_t*)user_data;

    frg_debug_print_newline(ctx->stream, ctx->indent);
    frg_debug_print_property_with_index(
        ctx->stream,
        ctx->label,
        ctx->index,
        NULL
    );

    frg_ast_print_debug(ctx->stream, (const frg_ast_t*)data, ctx->indent + FRG_DEBUG_INDENT_WIDTH);

    ctx->index++;
}

void _frg_ast_print_debug_list(frg_stream_output_t* stream, GList* list, const char* label, frg_indent_t indent) {
    _frg_ast_print_debug_list_ctx_t ctx = {
        .stream = stream,
        .indent = indent,
        .label = label,
        .index = 0
    };

    g_list_foreach(list, _frg_ast_print_debug_list_gfunc, &ctx);
}

void frg_ast_debug_printer_ty_symbol(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_SYMBOL);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_ty_symbol_t*)ast)->name->str);
    frg_debug_print_property(stream, "name", "%s", formatted->str);
    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_ty_pointer(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_POINTER);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_ast_print_debug(stream, ((frg_ast_ty_pointer_t*)ast)->value, indent + FRG_DEBUG_INDENT_WIDTH);
}

void frg_ast_debug_printer_ty_fn(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_FN);
    frg_assert_int_non_negative(indent);

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_ty_fn_t*)ast)->args,
        "args",
        indent + FRG_DEBUG_INDENT_WIDTH
    );

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "var-pos-args", NULL);
    frg_ast_print_debug(stream, ((frg_ast_ty_fn_t*)ast)->var_pos_args, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "var-kw-args", NULL);
    frg_ast_print_debug(stream, ((frg_ast_ty_fn_t*)ast)->var_kw_args, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "return-ty", NULL);
    frg_ast_print_debug(stream, ((frg_ast_ty_fn_t*)ast)->return_ty, indent + FRG_DEBUG_INDENT_WIDTH);
}

void frg_ast_debug_printer_decl_union(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_UNION);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_decl_union_t*)ast)->name->str);
    frg_debug_print_property(stream, "name", "%s", formatted->str);

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_decl_union_t*)ast)->props,
        "props",
        indent + FRG_DEBUG_INDENT_WIDTH
    );

    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_decl_struct(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_STRUCT);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_decl_struct_t*)ast)->name->str);
    frg_debug_print_property(stream, "name", "%s", formatted->str);

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_decl_struct_t*)ast)->decls,
        "decls",
        indent + FRG_DEBUG_INDENT_WIDTH
    );

    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_decl_prop(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_PROP);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* flags_string = frg_ast_decl_prop_flags_to_string(
        ((frg_ast_decl_prop_t*)ast)->flags
    );
    frg_debug_print_property(stream, "flags", flags_string->str);
    g_string_free(flags_string, TRUE);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_decl_prop_t*)ast)->name->str);
    frg_debug_print_property(stream, "name", "%s", formatted->str);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "type", NULL);
    frg_ast_print_debug(stream, ((frg_ast_decl_prop_t*)ast)->ty, indent + FRG_DEBUG_INDENT_WIDTH);

    g_string_free(flags_string, TRUE);
    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_decl_iface(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_IFACE);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* flags_string = frg_ast_decl_iface_flags_to_string(
        ((frg_ast_decl_iface_t*)ast)->flags
    );
    frg_debug_print_property(stream, "flags", flags_string->str);
    g_string_free(flags_string, TRUE);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_decl_iface_t*)ast)->name->str);
    frg_debug_print_property(stream, "name", "%s", formatted->str);

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_decl_iface_t*)ast)->extends,
        "extends",
        indent + FRG_DEBUG_INDENT_WIDTH
    );

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_decl_iface_t*)ast)->decls,
        "decls",
        indent + FRG_DEBUG_INDENT_WIDTH
    );

    g_string_free(flags_string, TRUE);
    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_decl_fn_arg(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN_ARG);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
            
    GString* flags_string = frg_ast_decl_fn_arg_flags_to_string(
        ((frg_ast_decl_fn_arg_t*)ast)->flags
    );
    frg_debug_print_property(stream, "flags", flags_string->str);
    g_string_free(flags_string, TRUE);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "prop", NULL);
    frg_ast_print_debug(stream, (const frg_ast_t*)((frg_ast_decl_fn_arg_t*)ast)->prop, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "default-value", NULL);
    frg_ast_print_debug(stream, ((frg_ast_decl_fn_arg_t*)ast)->default_value, indent + FRG_DEBUG_INDENT_WIDTH);

    g_string_free(flags_string, TRUE);
}

void frg_ast_debug_printer_decl_fn(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* flags_string = frg_ast_decl_fn_flags_to_string(
        ((frg_ast_decl_fn_t*)ast)->flags
    );
    frg_debug_print_property(stream, "flags", flags_string->str);
    g_string_free(flags_string, TRUE);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_decl_fn_t*)ast)->name->str);
    frg_debug_print_property(stream, "name", "%s", formatted->str);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "ty", NULL);
    frg_ast_print_debug(stream, (const frg_ast_t*)((frg_ast_decl_fn_t*)ast)->ty, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "body", NULL);
    frg_ast_print_debug(stream, ((frg_ast_decl_fn_t*)ast)->body, indent + FRG_DEBUG_INDENT_WIDTH);

    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_decl_var(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_VAR);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "prop", NULL);
    frg_ast_print_debug(stream, (const frg_ast_t*)((frg_ast_decl_var_t*)ast)->prop, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "initial-value", NULL);
    frg_ast_print_debug(stream, ((frg_ast_decl_var_t*)ast)->initial_value, indent + FRG_DEBUG_INDENT_WIDTH);
}

void frg_ast_debug_printer_decl_block(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_BLOCK);
    frg_assert_int_non_negative(indent);

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_decl_block_t*)ast)->decls,
        "decls",
        indent + FRG_DEBUG_INDENT_WIDTH
    );
}

void frg_ast_debug_printer_stmt_return(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_RETURN);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "value", NULL);
    frg_ast_print_debug(stream, ((frg_ast_stmt_return_t*)ast)->value, indent + FRG_DEBUG_INDENT_WIDTH);
}

void frg_ast_debug_printer_stmt_if(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_IF);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "condition", NULL);
    frg_ast_print_debug(stream, ((frg_ast_stmt_if_t*)ast)->condition, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "then-clause", NULL);
    frg_ast_print_debug(stream, ((frg_ast_stmt_if_t*)ast)->then_clause, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "else-clause", NULL);
    frg_ast_print_debug(stream, ((frg_ast_stmt_if_t*)ast)->else_clause, indent + FRG_DEBUG_INDENT_WIDTH);
}

void frg_ast_debug_printer_stmt_while(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_WHILE);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "condition", NULL);
    frg_ast_print_debug(stream, ((frg_ast_stmt_while_t*)ast)->condition, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "body", NULL);
    frg_ast_print_debug(stream, ((frg_ast_stmt_while_t*)ast)->body, indent + FRG_DEBUG_INDENT_WIDTH);
}

void frg_ast_debug_printer_stmt_block(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_BLOCK);
    frg_assert_int_non_negative(indent);

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_stmt_block_t*)ast)->stmts,
        "stmts",
        indent
    );
}

void frg_ast_debug_printer_value_int(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_INT);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "ty", NULL);
    frg_ast_print_debug(stream, ((frg_ast_value_int_t*)ast)->ty, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    if (((frg_ast_value_int_t*)ast)->ty == NULL) {
        frg_debug_print_property(stream, "value", "%li (defaulting to i64 since 'ty' is null)", ((frg_ast_value_int_t*)ast)->value.i64);
    } else {
        switch (((frg_ast_value_int_t*)ast)->ty->kind) {
            case FRG_AST_KIND_TY_U8:
                frg_debug_print_property(stream, "value", "%u", ((frg_ast_value_int_t*)ast)->value.u8);
                break;
            case FRG_AST_KIND_TY_U16:
                frg_debug_print_property(stream, "value", "%u", ((frg_ast_value_int_t*)ast)->value.u16);
                break;
            case FRG_AST_KIND_TY_U32:
                frg_debug_print_property(stream, "value", "%u", ((frg_ast_value_int_t*)ast)->value.u32);
                break;
            case FRG_AST_KIND_TY_U64:
                frg_debug_print_property(stream, "value", "%lu", ((frg_ast_value_int_t*)ast)->value.u64);
                break;
            case FRG_AST_KIND_TY_I8:
                frg_debug_print_property(stream, "value", "%i", ((frg_ast_value_int_t*)ast)->value.i8);
                break;
            case FRG_AST_KIND_TY_I16:
                frg_debug_print_property(stream, "value", "%i", ((frg_ast_value_int_t*)ast)->value.i16);
                break;
            case FRG_AST_KIND_TY_I32:
                frg_debug_print_property(stream, "value", "%i", ((frg_ast_value_int_t*)ast)->value.i32);
                break;
            case FRG_AST_KIND_TY_I64:
                frg_debug_print_property(stream, "value", "%li", ((frg_ast_value_int_t*)ast)->value.i64);
                break;
            default:
                frg_debug_print_property(stream, "value", "%li (defaulting to i64 since 'ty' is an unexpected type for an integer)", ((frg_ast_value_int_t*)ast)->value.i64);
                break;
        }
    }
}

void frg_ast_debug_printer_value_float(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_FLOAT);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "ty", NULL);
    frg_ast_print_debug(stream, ((frg_ast_value_float_t*)ast)->ty, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    if (((frg_ast_value_float_t*)ast)->ty == NULL) {
        frg_debug_print_property(stream, "value", "%f (defaulting to f64 since 'ty' is null)", ((frg_ast_value_float_t*)ast)->value.f64);
    } else {
        switch (((frg_ast_value_int_t*)ast)->ty->kind) {
            case FRG_AST_KIND_TY_F32:
                frg_debug_print_property(stream, "value", "%f", ((frg_ast_value_float_t*)ast)->value.f32);
                break;
            case FRG_AST_KIND_TY_F64:
                frg_debug_print_property(stream, "value", "%f", ((frg_ast_value_float_t*)ast)->value.f64);
                break;
            default:
                frg_debug_print_property(stream, "value", "%f (defaulting to f64 since 'ty' is an unexpected type for a float)", ((frg_ast_value_float_t*)ast)->value.f64);
                break;
        }
    }
}

void frg_ast_debug_printer_value_char(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CHAR);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_char(((frg_ast_value_char_t*)ast)->value);
    frg_debug_print_property(stream, "value", "%s", formatted->str);
    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_value_str(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_STR);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_value_str_t*)ast)->value->str);
    frg_debug_print_property(stream, "value", "%s", formatted->str);
    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_value_symbol(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_SYMBOL);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_value_symbol_t*)ast)->name->str);
    frg_debug_print_property(stream, "name", "%s", formatted->str);
    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_value_call_kw_arg(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CALL_KW_ARG);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    GString* formatted = frg_format_str(((frg_ast_value_call_kw_arg_t*)ast)->name->str);
    frg_debug_print_property(stream, "name", "%s", formatted->str);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "value", NULL);
    frg_ast_print_debug(stream, ((frg_ast_value_call_kw_arg_t*)ast)->value, indent + FRG_DEBUG_INDENT_WIDTH);

    g_string_free(formatted, TRUE);
}

void frg_ast_debug_printer_value_call(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CALL);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "callee", NULL);
    frg_ast_print_debug(stream, ((frg_ast_value_call_t*)ast)->callee, indent + FRG_DEBUG_INDENT_WIDTH);

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_value_call_t*)ast)->args,
        "args",
        indent + FRG_DEBUG_INDENT_WIDTH
    );

    _frg_ast_print_debug_list(
        stream,
        ((frg_ast_value_call_t*)ast)->kw_args,
        "kw-args",
        indent + FRG_DEBUG_INDENT_WIDTH
    );
}

void frg_ast_debug_printer_value_unary(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "operand", NULL);
    frg_ast_print_debug(stream, ((frg_ast_value_unary_t*)ast)->operand, indent + FRG_DEBUG_INDENT_WIDTH);
}

void frg_ast_debug_printer_value_binary(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY);
    frg_assert_int_non_negative(indent);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "left", NULL);
    frg_ast_print_debug(stream, ((frg_ast_value_binary_t*)ast)->left, indent + FRG_DEBUG_INDENT_WIDTH);

    frg_debug_print_newline(stream, indent);
    frg_debug_print_property(stream, "right", NULL);
    frg_ast_print_debug(stream, ((frg_ast_value_binary_t*)ast)->right, indent + FRG_DEBUG_INDENT_WIDTH);
}

