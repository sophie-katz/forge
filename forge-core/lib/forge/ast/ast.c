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

#include <forge/ast/ast.h>
#include <forge/common/log.h>
#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <stddef.h>
#include <stdlib.h>

frg_ast_t* frg_ast_new_ty_primary(
    frg_ast_id_t id
) {
    frg_assert(frg_ast_id_is_ty_primary(id));

    frg_ast_t* ast = frg_safe_malloc(sizeof(frg_ast_t));
    ast->id = id;

    return ast;
}

frg_ast_t* frg_ast_new_ty_symbol(
    GString* name
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_ty_symbol_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_symbol_t));

    ast->base.id = FRG_AST_ID_TY_SYMBOL;
    ast->name = name;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_ty_pointer(
    frg_ast_t* value
) {
    frg_assert_pointer_non_null(value);

    frg_ast_ty_pointer_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_pointer_t));

    ast->base.id = FRG_AST_ID_TY_POINTER;
    ast->value = value;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_ty_fn(
    GList* args,
    GList* var_pos_args,
    GList* var_kw_args,
    frg_ast_t* return_ty
) {
    frg_assert_pointer_non_null(return_ty);

    frg_ast_ty_fn_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_fn_t));

    ast->base.id = FRG_AST_ID_TY_FN;
    ast->args = args;
    ast->var_pos_args = var_pos_args;
    ast->var_kw_args = var_kw_args;
    ast->return_ty = return_ty;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_decl_union(
    GString* name,
    GList* props
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(props);

    frg_ast_decl_union_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_union_t));

    ast->base.id = FRG_AST_ID_DECL_UNION;
    ast->name = name;
    ast->props = props;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_decl_struct(
    GString* name,
    GList* decls
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_struct_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_struct_t));

    ast->base.id = FRG_AST_ID_DECL_STRUCT;
    ast->name = name;
    ast->decls = decls;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_decl_prop(
    frg_ast_decl_prop_flags_t flags,
    GString* name,
    frg_ast_t* type
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_prop_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_prop_t));

    ast->base.id = FRG_AST_ID_DECL_PROP;
    ast->flags = flags;
    ast->name = name;
    ast->type = type;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_decl_iface(
    frg_ast_decl_iface_flags_t flags,
    GString* name,
    GList* extends,
    GList* decls
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_iface_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_iface_t));

    ast->base.id = FRG_AST_ID_DECL_IFACE;
    ast->flags = flags;
    ast->name = name;
    ast->extends = extends;
    ast->decls = decls;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_decl_fn_arg(
    frg_ast_decl_fn_arg_flags_t flags,
    frg_ast_decl_prop_t* prop,
    frg_ast_t* default_value
) {
    frg_assert_pointer_non_null(prop);

    frg_ast_decl_fn_arg_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_fn_arg_t));

    ast->base.id = FRG_AST_ID_DECL_FN_ARG;
    ast->flags = flags;
    ast->prop = prop;
    ast->default_value = default_value;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_decl_fn(
    frg_ast_decl_fn_flags_t flags,
    GString* name,
    frg_ast_ty_fn_t* ty,
    frg_ast_t* body
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(ty);

    frg_ast_decl_fn_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_fn_t));

    ast->base.id = FRG_AST_ID_DECL_FN;
    ast->flags = flags;
    ast->name = name;
    ast->ty = ty;
    ast->body = body;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_decl_var(
    frg_ast_decl_prop_t* prop,
    frg_ast_t* initial_value
) {
    frg_assert_pointer_non_null(prop);

    frg_ast_decl_var_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_var_t));

    ast->base.id = FRG_AST_ID_DECL_VAR;
    ast->prop = prop;
    ast->initial_value = initial_value;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_decl_block(
    GList* decls
) {
    frg_ast_decl_block_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_block_t));

    ast->base.id = FRG_AST_ID_DECL_BLOCK;
    ast->decls = decls;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_stmt_return(
    frg_ast_t* value
) {
    frg_ast_stmt_return_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_return_t));

    ast->base.id = FRG_AST_ID_STMT_RETURN;
    ast->value = value;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_stmt_if(
    frg_ast_t* condition,
    frg_ast_t* then_clause,
    frg_ast_t* else_clause
) {
    frg_assert_pointer_non_null(condition);
    frg_assert_pointer_non_null(then_clause);

    frg_ast_stmt_if_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_if_t));

    ast->base.id = FRG_AST_ID_STMT_IF;
    ast->condition = condition;
    ast->then_clause = then_clause;
    ast->else_clause = else_clause;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_stmt_while(
    frg_ast_t* condition,
    frg_ast_t* body
) {
    frg_assert_pointer_non_null(condition);

    frg_ast_stmt_while_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_while_t));

    ast->base.id = FRG_AST_ID_STMT_WHILE;
    ast->condition = condition;
    ast->body = body;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_stmt_block(
    GList* stmts
) {
    frg_ast_stmt_block_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_block_t));

    ast->base.id = FRG_AST_ID_STMT_BLOCK;
    ast->stmts = stmts;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_primary(
    frg_ast_id_t id
) {
    frg_assert(frg_ast_id_is_value_primary(id));

    frg_ast_t* ast = frg_safe_malloc(sizeof(frg_ast_t));

    ast->id = id;

    return ast;
}

frg_ast_t* frg_ast_new_value_i8(
    int8_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.id = FRG_AST_ID_VALUE_INT;
    ast->value.i8 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_I8);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_i16(
    int16_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.id = FRG_AST_ID_VALUE_INT;
    ast->value.i16 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_I16);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_i32(
    int32_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.id = FRG_AST_ID_VALUE_INT;
    ast->value.i32 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_I32);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_i64(
    int64_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.id = FRG_AST_ID_VALUE_INT;
    ast->value.i64 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_I64);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_u8(
    uint8_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.id = FRG_AST_ID_VALUE_INT;
    ast->value.u8 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_U8);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_u16(
    uint16_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.id = FRG_AST_ID_VALUE_INT;
    ast->value.u16 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_U16);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_u32(
    uint32_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.id = FRG_AST_ID_VALUE_INT;
    ast->value.u32 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_U32);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_u64(
    uint64_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.id = FRG_AST_ID_VALUE_INT;
    ast->value.u64 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_U64);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_f32(
    frg_f32_t value
) {
    frg_ast_value_float_t* ast = frg_safe_malloc(sizeof(frg_ast_value_float_t));

    ast->base.id = FRG_AST_ID_VALUE_FLOAT;
    ast->value.f32 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_F32);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_f64(
    frg_f64_t value
) {
    frg_ast_value_float_t* ast = frg_safe_malloc(sizeof(frg_ast_value_float_t));

    ast->base.id = FRG_AST_ID_VALUE_FLOAT;
    ast->value.f64 = value;
    ast->ty = frg_ast_new_ty_primary(FRG_AST_ID_TY_F64);

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_char(
    frg_char_t value
) {
    frg_ast_value_char_t* ast = frg_safe_malloc(sizeof(frg_ast_value_char_t));

    ast->base.id = FRG_AST_ID_VALUE_CHAR;
    ast->value = value;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_str(
    GString* value
) {
    frg_assert_pointer_non_null(value);

    frg_ast_value_str_t* ast = frg_safe_malloc(sizeof(frg_ast_value_str_t));

    ast->base.id = FRG_AST_ID_VALUE_STR;
    ast->value = value;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_symbol(
    GString* name
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_value_symbol_t* ast = frg_safe_malloc(sizeof(frg_ast_value_symbol_t));

    ast->base.id = FRG_AST_ID_VALUE_SYMBOL;
    ast->name = name;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_call_kw_arg(
    GString* name,
    frg_ast_t* value
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(value);

    frg_ast_value_call_kw_arg_t* ast = frg_safe_malloc(sizeof(frg_ast_value_call_kw_arg_t));

    ast->base.id = FRG_AST_ID_VALUE_CALL_KW_ARG;
    ast->name = name;
    ast->value = value;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_call(
    frg_ast_t* callee,
    GList* args,
    GList* kw_args
) {
    frg_assert_pointer_non_null(callee);

    frg_ast_value_call_t* ast = frg_safe_malloc(sizeof(frg_ast_value_call_t));

    ast->base.id = FRG_AST_ID_VALUE_CALL;
    ast->callee = callee;
    ast->args = args;
    ast->kw_args = kw_args;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_unary(
    frg_ast_id_t id,
    frg_ast_t* operand
) {
    frg_assert(frg_ast_id_is_value_unary(id));
    frg_assert_pointer_non_null(operand);

    frg_ast_value_unary_t* ast = frg_safe_malloc(sizeof(frg_ast_value_unary_t));

    ast->base.id = id;
    ast->operand = operand;

    return (frg_ast_t*)ast;
}

frg_ast_t* frg_ast_new_value_binary(
    frg_ast_id_t id,
    frg_ast_t* left,
    frg_ast_t* right
) {
    frg_assert(frg_ast_id_is_value_binary(id));
    frg_assert_pointer_non_null(left);
    frg_assert_pointer_non_null(right);

    frg_ast_value_binary_t* ast = frg_safe_malloc(sizeof(frg_ast_value_binary_t));

    ast->base.id = id;
    ast->left = left;
    ast->right = right;

    return (frg_ast_t*)ast;
}

void _frg_ast_destroy_gfunc(gpointer data, gpointer user_data) {
    frg_ast_destroy((frg_ast_t**)&data);
}

void _frg_asts_free_glist(GList* list) {
    g_list_foreach(list, _frg_ast_destroy_gfunc, NULL);
    g_list_free(list);
}

void frg_ast_destroy(frg_ast_t** ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);

    switch ((*ast)->id) {
        case FRG_AST_ID_TY_SYMBOL:
            g_string_free(((frg_ast_ty_symbol_t*)*ast)->name, TRUE);

            break;
        case FRG_AST_ID_TY_FN:
            _frg_asts_free_glist(((frg_ast_ty_fn_t*)*ast)->args);

            _frg_asts_free_glist(((frg_ast_ty_fn_t*)*ast)->var_pos_args);

            _frg_asts_free_glist(((frg_ast_ty_fn_t*)*ast)->var_kw_args);

            frg_ast_destroy(&((frg_ast_ty_fn_t*)*ast)->return_ty);

            break;
        case FRG_AST_ID_DECL_UNION:
            g_string_free(((frg_ast_decl_union_t*)*ast)->name, TRUE);

            _frg_asts_free_glist(((frg_ast_decl_union_t*)*ast)->props);

            break;
        case FRG_AST_ID_DECL_STRUCT:
            g_string_free(((frg_ast_decl_struct_t*)*ast)->name, TRUE);

            _frg_asts_free_glist(((frg_ast_decl_struct_t*)*ast)->decls);

            break;
        case FRG_AST_ID_DECL_PROP:
            g_string_free(((frg_ast_decl_prop_t*)*ast)->name, TRUE);

            frg_ast_destroy(&((frg_ast_decl_prop_t*)*ast)->type);

            break;
        case FRG_AST_ID_DECL_IFACE:
            g_string_free(((frg_ast_decl_iface_t*)*ast)->name, TRUE);

            _frg_asts_free_glist(((frg_ast_decl_iface_t*)*ast)->extends);

            _frg_asts_free_glist(((frg_ast_decl_iface_t*)*ast)->decls);

            break;
        case FRG_AST_ID_DECL_FN_ARG:
            frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_fn_arg_t*)*ast)->prop);

            frg_ast_destroy(&((frg_ast_decl_fn_arg_t*)*ast)->default_value);

            break;
        case FRG_AST_ID_DECL_FN:
            g_string_free(((frg_ast_decl_fn_t*)*ast)->name, TRUE);

            frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_fn_t*)*ast)->ty);

            frg_ast_destroy(&((frg_ast_decl_fn_t*)*ast)->body);

            break;
        case FRG_AST_ID_DECL_VAR:
            frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_var_t*)*ast)->prop);

            frg_ast_destroy(&((frg_ast_decl_var_t*)*ast)->initial_value);

            break;
        case FRG_AST_ID_DECL_BLOCK:
            _frg_asts_free_glist(((frg_ast_decl_block_t*)*ast)->decls);

            break;
        case FRG_AST_ID_STMT_RETURN:
            frg_ast_destroy(&((frg_ast_stmt_return_t*)*ast)->value);

            break;
        case FRG_AST_ID_STMT_IF:
            frg_ast_destroy(&((frg_ast_stmt_if_t*)*ast)->condition);

            frg_ast_destroy(&((frg_ast_stmt_if_t*)*ast)->then_clause);

            frg_ast_destroy(&((frg_ast_stmt_if_t*)*ast)->else_clause);

            break;
        case FRG_AST_ID_STMT_WHILE:
            frg_ast_destroy(&((frg_ast_stmt_while_t*)*ast)->condition);

            frg_ast_destroy(&((frg_ast_stmt_while_t*)*ast)->body);

            break;
        case FRG_AST_ID_STMT_BLOCK:
            _frg_asts_free_glist(((frg_ast_stmt_block_t*)*ast)->stmts);

            break;
        case FRG_AST_ID_VALUE_INT:
            frg_ast_destroy(&((frg_ast_value_int_t*)*ast)->ty);

            break;
        case FRG_AST_ID_VALUE_FLOAT:
            frg_ast_destroy(&((frg_ast_value_float_t*)*ast)->ty);

            break;
        case FRG_AST_ID_VALUE_STR:
            g_string_free(((frg_ast_value_str_t*)*ast)->value, TRUE);

            break;
        case FRG_AST_ID_VALUE_SYMBOL:
            g_string_free(((frg_ast_value_symbol_t*)*ast)->name, TRUE);

            break;
        case FRG_AST_ID_VALUE_CALL_KW_ARG:
            g_string_free(((frg_ast_value_call_kw_arg_t*)*ast)->name, TRUE);

            frg_ast_destroy(&((frg_ast_value_call_kw_arg_t*)*ast)->value);

            break;
        case FRG_AST_ID_VALUE_CALL:
            frg_ast_destroy(&((frg_ast_value_call_t*)*ast)->callee);

            _frg_asts_free_glist(((frg_ast_value_call_t*)*ast)->args);

            _frg_asts_free_glist(((frg_ast_value_call_t*)*ast)->kw_args);

            break;
        case FRG_AST_ID_VALUE_BIT_NOT:
        case FRG_AST_ID_VALUE_NEG:
        case FRG_AST_ID_VALUE_LOG_NOT:
        case FRG_AST_ID_VALUE_INC:
        case FRG_AST_ID_VALUE_DEC:
            frg_ast_destroy(&((frg_ast_value_unary_t*)*ast)->operand);

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
            frg_ast_destroy(&((frg_ast_value_binary_t*)*ast)->left);

            frg_ast_destroy(&((frg_ast_value_binary_t*)*ast)->right);

            break;
        default:
            break;
    }
}

frg_ast_ty_symbol_t* frg_ast_try_cast_ty_symbol(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_TY_SYMBOL) {
        return (frg_ast_ty_symbol_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_union_t* frg_ast_try_cast_decl_union(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_DECL_UNION) {
        return (frg_ast_decl_union_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_struct_t* frg_ast_try_cast_decl_struct(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_DECL_STRUCT) {
        return (frg_ast_decl_struct_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_prop_t* frg_ast_try_cast_decl_prop(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_DECL_PROP) {
        return (frg_ast_decl_prop_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_iface_t* frg_ast_try_cast_decl_iface(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_DECL_IFACE) {
        return (frg_ast_decl_iface_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_fn_arg_t* frg_ast_try_cast_decl_fn_arg(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_DECL_FN_ARG) {
        return (frg_ast_decl_fn_arg_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_fn_t* frg_ast_try_cast_decl_fn(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_DECL_FN) {
        return (frg_ast_decl_fn_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_var_t* frg_ast_try_cast_decl_var(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_DECL_VAR) {
        return (frg_ast_decl_var_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_block_t* frg_ast_try_cast_decl_block(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_DECL_BLOCK) {
        return (frg_ast_decl_block_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_return_t* frg_ast_try_cast_stmt_return(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_STMT_RETURN) {
        return (frg_ast_stmt_return_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_if_t* frg_ast_try_cast_stmt_if(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_STMT_IF) {
        return (frg_ast_stmt_if_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_while_t* frg_ast_try_cast_stmt_while(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_STMT_WHILE) {
        return (frg_ast_stmt_while_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_block_t* frg_ast_try_cast_stmt_block(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_STMT_BLOCK) {
        return (frg_ast_stmt_block_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_int_t* frg_ast_try_cast_value_int(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_VALUE_INT) {
        return (frg_ast_value_int_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_float_t* frg_ast_try_cast_value_float(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_VALUE_FLOAT) {
        return (frg_ast_value_float_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_char_t* frg_ast_try_cast_value_char(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_VALUE_CHAR) {
        return (frg_ast_value_char_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_str_t* frg_ast_try_cast_value_str(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_VALUE_STR) {
        return (frg_ast_value_str_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_symbol_t* frg_ast_try_cast_value_symbol(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_VALUE_SYMBOL) {
        return (frg_ast_value_symbol_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_call_kw_arg_t* frg_ast_try_cast_value_call_kw_arg(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_VALUE_CALL_KW_ARG) {
        return (frg_ast_value_call_kw_arg_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_call_t* frg_ast_try_cast_value_call(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->id == FRG_AST_ID_VALUE_CALL) {
        return (frg_ast_value_call_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_unary_t* frg_ast_try_cast_value_unary(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (frg_ast_id_is_value_unary(ast->id)) {
        return (frg_ast_value_unary_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_binary_t* frg_ast_try_cast_value_binary(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (frg_ast_id_is_value_binary(ast->id)) {
        return (frg_ast_value_binary_t*)ast;
    } else {
        return NULL;
    }
}

const char* frg_ast_get_name(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);

    switch (ast->id) {
        case FRG_AST_ID_DECL_UNION:
            return ((frg_ast_decl_union_t*)ast)->name->str;
        case FRG_AST_ID_DECL_STRUCT:
            return ((frg_ast_decl_struct_t*)ast)->name->str;
        case FRG_AST_ID_DECL_PROP:
            return ((frg_ast_decl_prop_t*)ast)->name->str;
        case FRG_AST_ID_DECL_IFACE:
            return ((frg_ast_decl_iface_t*)ast)->name->str;
        case FRG_AST_ID_DECL_FN_ARG:
            return ((frg_ast_decl_prop_t*)(((frg_ast_decl_fn_arg_t*)ast)->prop))->name->str;
        case FRG_AST_ID_DECL_FN:
            return ((frg_ast_decl_fn_t*)ast)->name->str;
        case FRG_AST_ID_DECL_VAR:
            return ((frg_ast_decl_prop_t*)(((frg_ast_decl_var_t*)ast)->prop))->name->str;
        default:
            return NULL;
    }
}
