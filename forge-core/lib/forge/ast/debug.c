// Copyright (c) 2023 Sophie Katz
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

#include <forge/ast/debug.h>
#include <forge/common/stream.h>
#include <forge/common/lexical_casts.h>

typedef struct {
    const frg_indent_t indent;
    const char* label;
    size_t index;
} _frg_ast_print_debug_list_ctx_t;

void _frg_ast_print_debug_list_gfunc(gpointer data, gpointer user_data) {
    _frg_ast_print_debug_list_ctx_t* ctx = (_frg_ast_print_debug_list_ctx_t*)user_data;

    frg_debug_print_newline(ctx->indent);
    frg_debug_print_property_with_index(
        ctx->label,
        ctx->index,
        NULL
    );

    frg_ast_print_debug((const frg_ast_t*)data, ctx->indent + FRG_DEBUG_INDENT_WIDTH);

    ctx->index++;
}

void _frg_ast_print_debug_list(GList* list, const char* label, frg_indent_t indent) {
    _frg_ast_print_debug_list_ctx_t ctx = {
        .indent = indent,
        .label = label,
        .index = 0
    };

    g_list_foreach(list, _frg_ast_print_debug_list_gfunc, &ctx);
}

void frg_ast_print_debug(const frg_ast_t* ast, frg_indent_t indent) {
    if (ast == NULL) {
        frg_debug_print_node("null");
        return;
    }

    frg_debug_print_node(frg_ast_id_to_string(ast->id));

    frg_status_t result = FRG_STATUS_OK;
    GString* escaped = NULL;

    switch (ast->id) {
        case FRG_AST_ID_TY_SYMBOL:
            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_ty_symbol_t*)ast)->name->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("name", "%s", escaped->str);
            } else {
                frg_debug_print_property("name", "(error while escaping string)");
            }

            break;
        case FRG_AST_ID_DECL_UNION:
            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_decl_union_t*)ast)->name->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("name", "%s", escaped->str);
            } else {
                frg_debug_print_property("name", "(error while escaping string)");
            }

            _frg_ast_print_debug_list(
                ((frg_ast_decl_union_t*)ast)->props,
                "props",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            break;
        case FRG_AST_ID_DECL_STRUCT:
            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_decl_struct_t*)ast)->name->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("name", "%s", escaped->str);
            } else {
                frg_debug_print_property("name", "(error while escaping string)");
            }

            _frg_ast_print_debug_list(
                ((frg_ast_decl_struct_t*)ast)->decls,
                "decls",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            break;
        case FRG_AST_ID_DECL_PROP:
            frg_debug_print_newline(indent);
            frg_debug_print_property("flags", NULL);
            frg_ast_decl_prop_flags_print(
                FRG_STREAM_DEFAULT,
                ((frg_ast_decl_prop_t*)ast)->flags
            );

            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_decl_prop_t*)ast)->name->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("name", "%s", escaped->str);
            } else {
                frg_debug_print_property("name", "(error while escaping string)");
            }

            frg_debug_print_newline(indent);
            frg_debug_print_property("type", NULL);
            frg_ast_print_debug(((frg_ast_decl_prop_t*)ast)->type, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_DECL_IFACE:
            frg_debug_print_newline(indent);
            frg_debug_print_property("flags", NULL);
            frg_ast_decl_iface_flags_print(
                FRG_STREAM_DEFAULT,
                ((frg_ast_decl_iface_t*)ast)->flags
            );

            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_decl_iface_t*)ast)->name->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("name", "%s", escaped->str);
            } else {
                frg_debug_print_property("name", "(error while escaping string)");
            }

            _frg_ast_print_debug_list(
                ((frg_ast_decl_iface_t*)ast)->extends,
                "extends",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            _frg_ast_print_debug_list(
                ((frg_ast_decl_iface_t*)ast)->decls,
                "decls",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            break;
        case FRG_AST_ID_DECL_FN_ARG:
            frg_debug_print_newline(indent);
            frg_debug_print_property("flags", NULL);
            frg_ast_decl_fn_arg_flags_print(
                FRG_STREAM_DEFAULT,
                ((frg_ast_decl_fn_arg_t*)ast)->flags
            );

            frg_debug_print_newline(indent);
            frg_debug_print_property("prop", NULL);
            frg_ast_print_debug(((frg_ast_decl_fn_arg_t*)ast)->prop, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            frg_debug_print_property("default-value", NULL);
            frg_ast_print_debug(((frg_ast_decl_fn_arg_t*)ast)->default_value, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_DECL_FN:
            frg_debug_print_newline(indent);
            frg_debug_print_property("flags", NULL);
            frg_ast_decl_fn_flags_print(
                FRG_STREAM_DEFAULT,
                ((frg_ast_decl_fn_t*)ast)->flags
            );

            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_decl_fn_t*)ast)->name->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("name", "%s", escaped->str);
            } else {
                frg_debug_print_property("name", "(error while escaping string)");
            }

            _frg_ast_print_debug_list(
                ((frg_ast_decl_fn_t*)ast)->args,
                "args",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            _frg_ast_print_debug_list(
                ((frg_ast_decl_fn_t*)ast)->var_pos_args,
                "var-pos-args",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            _frg_ast_print_debug_list(
                ((frg_ast_decl_fn_t*)ast)->var_kw_args,
                "var-kw-args",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            frg_debug_print_newline(indent);
            frg_debug_print_property("return_ty", NULL);
            frg_ast_print_debug(((frg_ast_decl_fn_t*)ast)->return_ty, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            frg_debug_print_property("body", NULL);
            frg_ast_print_debug(((frg_ast_decl_fn_t*)ast)->body, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_DECL_VAR:
            frg_debug_print_newline(indent);
            frg_debug_print_property("prop", NULL);
            frg_ast_print_debug(((frg_ast_decl_var_t*)ast)->prop, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            frg_debug_print_property("initial-value", NULL);
            frg_ast_print_debug(((frg_ast_decl_var_t*)ast)->initial_value, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_DECL_BLOCK:
            _frg_ast_print_debug_list(
                ((frg_ast_decl_block_t*)ast)->decls,
                "decls",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            break;
        case FRG_AST_ID_STMT_RETURN:
            frg_debug_print_newline(indent);
            frg_debug_print_property("value", NULL);
            frg_ast_print_debug(((frg_ast_stmt_return_t*)ast)->value, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_STMT_IF:
            frg_debug_print_newline(indent);
            frg_debug_print_property("condition", NULL);
            frg_ast_print_debug(((frg_ast_stmt_if_t*)ast)->condition, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            frg_debug_print_property("then-clause", NULL);
            frg_ast_print_debug(((frg_ast_stmt_if_t*)ast)->then_clause, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            frg_debug_print_property("else-clause", NULL);
            frg_ast_print_debug(((frg_ast_stmt_if_t*)ast)->else_clause, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_STMT_WHILE:
            frg_debug_print_newline(indent);
            frg_debug_print_property("condition", NULL);
            frg_ast_print_debug(((frg_ast_stmt_while_t*)ast)->condition, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            frg_debug_print_property("body", NULL);
            frg_ast_print_debug(((frg_ast_stmt_while_t*)ast)->body, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_STMT_BLOCK:
            _frg_ast_print_debug_list(
                ((frg_ast_stmt_block_t*)ast)->stmts,
                "stmts",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            break;
        case FRG_AST_ID_VALUE_INT:
            frg_debug_print_newline(indent);
            frg_debug_print_property("ty", NULL);
            frg_ast_print_debug(((frg_ast_value_int_t*)ast)->ty, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            if (((frg_ast_value_int_t*)ast)->ty == NULL) {
                frg_debug_print_property("value", "%li (defaulting to i64 since 'ty' is null)", ((frg_ast_value_int_t*)ast)->value.i64);
            } else {
                switch (((frg_ast_value_int_t*)ast)->ty->id) {
                    case FRG_AST_ID_TY_U8:
                        frg_debug_print_property("value", "%u", ((frg_ast_value_int_t*)ast)->value.u8);
                        break;
                    case FRG_AST_ID_TY_U16:
                        frg_debug_print_property("value", "%u", ((frg_ast_value_int_t*)ast)->value.u16);
                        break;
                    case FRG_AST_ID_TY_U32:
                        frg_debug_print_property("value", "%u", ((frg_ast_value_int_t*)ast)->value.u32);
                        break;
                    case FRG_AST_ID_TY_U64:
                        frg_debug_print_property("value", "%lu", ((frg_ast_value_int_t*)ast)->value.u64);
                        break;
                    case FRG_AST_ID_TY_I8:
                        frg_debug_print_property("value", "%i", ((frg_ast_value_int_t*)ast)->value.i8);
                        break;
                    case FRG_AST_ID_TY_I16:
                        frg_debug_print_property("value", "%i", ((frg_ast_value_int_t*)ast)->value.i16);
                        break;
                    case FRG_AST_ID_TY_I32:
                        frg_debug_print_property("value", "%i", ((frg_ast_value_int_t*)ast)->value.i32);
                        break;
                    case FRG_AST_ID_TY_I64:
                        frg_debug_print_property("value", "%li", ((frg_ast_value_int_t*)ast)->value.i64);
                        break;
                    default:
                        frg_debug_print_property("value", "%li (defaulting to i64 since 'ty' is an unexpected type for an integer)", ((frg_ast_value_int_t*)ast)->value.i64);
                        break;
                }
            }

            break;
        case FRG_AST_ID_VALUE_FLOAT:
            frg_debug_print_newline(indent);
            frg_debug_print_property("ty", NULL);
            frg_ast_print_debug(((frg_ast_value_float_t*)ast)->ty, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            if (((frg_ast_value_float_t*)ast)->ty == NULL) {
                frg_debug_print_property("value", "%f (defaulting to f64 since 'ty' is null)", ((frg_ast_value_float_t*)ast)->value.f64);
            } else {
                switch (((frg_ast_value_int_t*)ast)->ty->id) {
                    case FRG_AST_ID_TY_F32:
                        frg_debug_print_property("value", "%f", ((frg_ast_value_float_t*)ast)->value.f32);
                        break;
                    case FRG_AST_ID_TY_F64:
                        frg_debug_print_property("value", "%f", ((frg_ast_value_float_t*)ast)->value.f64);
                        break;
                    default:
                        frg_debug_print_property("value", "%f (defaulting to f64 since 'ty' is an unexpected type for a float)", ((frg_ast_value_float_t*)ast)->value.f64);
                        break;
                }
            }

            break;
        case FRG_AST_ID_VALUE_CHAR:
            frg_debug_print_newline(indent);
            result = frg_escape_char(&escaped, ((frg_ast_value_char_t*)ast)->value);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("value", "%s", escaped->str);
            } else {
                frg_debug_print_property("value", "(error while escaping string)");
            }

            break;
        case FRG_AST_ID_VALUE_STR:
            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_value_str_t*)ast)->value->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("value", "%s", escaped->str);
            } else {
                frg_debug_print_property("value", "(error while escaping string)");
            }

            break;
        case FRG_AST_ID_VALUE_SYMBOL:
            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_value_symbol_t*)ast)->name->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("name", "%s", escaped->str);
            } else {
                frg_debug_print_property("name", "(error while escaping string)");
            }

            break;
        case FRG_AST_ID_VALUE_CALL_KW_ARG:
            frg_debug_print_newline(indent);
            result = frg_escape_str(&escaped, ((frg_ast_value_call_kw_arg_t*)ast)->name->str);
            if (result == FRG_STATUS_OK) {
                frg_debug_print_property("name", "%s", escaped->str);
            } else {
                frg_debug_print_property("name", "(error while escaping string)");
            }

            frg_debug_print_newline(indent);
            frg_debug_print_property("value", NULL);
            frg_ast_print_debug(((frg_ast_value_call_kw_arg_t*)ast)->value, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_VALUE_CALL:
            frg_debug_print_newline(indent);
            frg_debug_print_property("callee", NULL);
            frg_ast_print_debug(((frg_ast_value_call_t*)ast)->callee, indent + FRG_DEBUG_INDENT_WIDTH);

            _frg_ast_print_debug_list(
                ((frg_ast_value_call_t*)ast)->args,
                "args",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            _frg_ast_print_debug_list(
                ((frg_ast_value_call_t*)ast)->kw_args,
                "kw-args",
                indent + FRG_DEBUG_INDENT_WIDTH
            );

            break;
        case FRG_AST_ID_VALUE_BIT_NOT:
        case FRG_AST_ID_VALUE_NEG:
        case FRG_AST_ID_VALUE_LOG_NOT:
        case FRG_AST_ID_VALUE_INC:
        case FRG_AST_ID_VALUE_DEC:
            frg_debug_print_newline(indent);
            frg_debug_print_property("operand", NULL);
            frg_ast_print_debug(((frg_ast_value_unary_t*)ast)->operand, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        case FRG_AST_ID_VALUE_ACCESS:
        case FRG_AST_ID_VALUE_BIT_AND:
        case FRG_AST_ID_VALUE_BIT_OR:
        case FRG_AST_ID_VALUE_BIT_XOR:
        case FRG_AST_ID_VALUE_BIT_SHL:
        case FRG_AST_ID_VALUE_BIT_SHR:
        case FRG_AST_ID_VALUE_ADD:
        case FRG_AST_ID_VALUE_SUB:
        case FRG_AST_ID_VALUE_MUL:
        case FRG_AST_ID_VALUE_DIV:
        case FRG_AST_ID_VALUE_DIV_INT:
        case FRG_AST_ID_VALUE_MOD:
        case FRG_AST_ID_VALUE_EXP:
        case FRG_AST_ID_VALUE_EQ:
        case FRG_AST_ID_VALUE_NE:
        case FRG_AST_ID_VALUE_LT:
        case FRG_AST_ID_VALUE_LE:
        case FRG_AST_ID_VALUE_GT:
        case FRG_AST_ID_VALUE_GE:
        case FRG_AST_ID_VALUE_LOG_AND:
        case FRG_AST_ID_VALUE_LOG_OR:
        case FRG_AST_ID_VALUE_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_AND_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_OR_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_XOR_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_SHL_ASSIGN:
        case FRG_AST_ID_VALUE_BIT_SHR_ASSIGN:
        case FRG_AST_ID_VALUE_ADD_ASSIGN:
        case FRG_AST_ID_VALUE_SUB_ASSIGN:
        case FRG_AST_ID_VALUE_MUL_ASSIGN:
        case FRG_AST_ID_VALUE_DIV_ASSIGN:
        case FRG_AST_ID_VALUE_DIV_INT_ASSIGN:
        case FRG_AST_ID_VALUE_MOD_ASSIGN:
        case FRG_AST_ID_VALUE_EXP_ASSIGN:
        case FRG_AST_ID_VALUE_LOG_AND_ASSIGN:
        case FRG_AST_ID_VALUE_LOG_OR_ASSIGN:
            frg_debug_print_newline(indent);
            frg_debug_print_property("left", NULL);
            frg_ast_print_debug(((frg_ast_value_binary_t*)ast)->left, indent + FRG_DEBUG_INDENT_WIDTH);

            frg_debug_print_newline(indent);
            frg_debug_print_property("right", NULL);
            frg_ast_print_debug(((frg_ast_value_binary_t*)ast)->right, indent + FRG_DEBUG_INDENT_WIDTH);

            break;
        default:
            break;
    }
}
