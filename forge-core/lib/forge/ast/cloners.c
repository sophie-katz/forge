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

#include <forge/ast/cloners.h>
#include <forge/ast/kind_info.h>
#include <forge/common/error.h>
#include <forge/common/memory.h>

GList* _frg_ast_clone_list(GList* list) {
    GList* clone = NULL;

    for (GList* it = list; it != NULL; it = it->next) {
        clone = g_list_append(clone, frg_ast_clone((frg_ast_t*)it->data));
    }

    return clone;
}

frg_ast_t* frg_ast_clone_base(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);

    frg_ast_t* result = frg_safe_malloc(sizeof(frg_ast_t));

    memcpy(result, ast, sizeof(frg_ast_t));

    return result;
}

frg_ast_t* frg_ast_clone_ty_symbol(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_SYMBOL);

    return (frg_ast_t*)frg_ast_new_ty_symbol(
        &ast->source_range,
        g_string_new(((frg_ast_ty_symbol_t*)ast)->name->str)
    );
}

frg_ast_t* frg_ast_clone_ty_pointer(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_POINTER);

    return (frg_ast_t*)frg_ast_new_ty_pointer(
        &ast->source_range,
        frg_ast_clone(((frg_ast_ty_pointer_t*)ast)->value)
    );
}

frg_ast_t* frg_ast_clone_ty_fn(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_FN);

    return (frg_ast_t*)frg_ast_new_ty_fn(
        &ast->source_range,
        _frg_ast_clone_list(((frg_ast_ty_fn_t*)ast)->args),
        frg_ast_clone(((frg_ast_ty_fn_t*)ast)->var_pos_args),
        frg_ast_clone(((frg_ast_ty_fn_t*)ast)->var_kw_args),
        frg_ast_clone(((frg_ast_ty_fn_t*)ast)->return_ty)
    );
}

frg_ast_t* frg_ast_clone_decl_union(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_UNION);

    return (frg_ast_t*)frg_ast_new_decl_union(
        &ast->source_range,
        g_string_new(((frg_ast_decl_union_t*)ast)->name->str),
        _frg_ast_clone_list(((frg_ast_decl_union_t*)ast)->props)
    );
}

frg_ast_t* frg_ast_clone_decl_struct(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_STRUCT);

    return (frg_ast_t*)frg_ast_new_decl_struct(
        &ast->source_range,
        g_string_new(((frg_ast_decl_struct_t*)ast)->name->str),
        _frg_ast_clone_list(((frg_ast_decl_struct_t*)ast)->decls)
    );
}

frg_ast_t* frg_ast_clone_decl_prop(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_PROP);

    return (frg_ast_t*)frg_ast_new_decl_prop(
        &ast->source_range,
        ((frg_ast_decl_prop_t*)ast)->flags,
        g_string_new(((frg_ast_decl_prop_t*)ast)->name->str),
        frg_ast_clone(((frg_ast_decl_prop_t*)ast)->ty)
    );
}

frg_ast_t* frg_ast_clone_decl_iface(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_IFACE);

    return (frg_ast_t*)frg_ast_new_decl_iface(
        &ast->source_range,
        ((frg_ast_decl_iface_t*)ast)->flags,
        g_string_new(((frg_ast_decl_iface_t*)ast)->name->str),
        _frg_ast_clone_list(((frg_ast_decl_iface_t*)ast)->extends),
        _frg_ast_clone_list(((frg_ast_decl_iface_t*)ast)->decls)
    );
}

frg_ast_t* frg_ast_clone_decl_fn_arg(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN_ARG);

    return (frg_ast_t*)frg_ast_new_decl_fn_arg(
        &ast->source_range,
        ((frg_ast_decl_fn_arg_t*)ast)->flags,
        (frg_ast_decl_prop_t*)frg_ast_clone((frg_ast_t*)((frg_ast_decl_fn_arg_t*)ast)->prop),
        frg_ast_clone(((frg_ast_decl_fn_arg_t*)ast)->default_value)
    );
}

frg_ast_t* frg_ast_clone_decl_fn(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN);

    return (frg_ast_t*)frg_ast_new_decl_fn(
        &ast->source_range,
        ((frg_ast_decl_fn_t*)ast)->flags,
        g_string_new(((frg_ast_decl_fn_t*)ast)->name->str),
        (frg_ast_ty_fn_t*)frg_ast_clone((frg_ast_t*)((frg_ast_decl_fn_t*)ast)->ty),
        frg_ast_clone(((frg_ast_decl_fn_t*)ast)->body)
    );
}

frg_ast_t* frg_ast_clone_decl_var(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_VAR);

    return (frg_ast_t*)frg_ast_new_decl_var(
        &ast->source_range,
        (frg_ast_decl_prop_t*)frg_ast_clone((frg_ast_t*)((frg_ast_decl_var_t*)ast)->prop),
        frg_ast_clone(((frg_ast_decl_var_t*)ast)->initial_value)
    );
}

frg_ast_t* frg_ast_clone_decl_block(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_BLOCK);

    return (frg_ast_t*)frg_ast_new_decl_block(
        &ast->source_range,
        _frg_ast_clone_list(((frg_ast_decl_block_t*)ast)->decls)
    );
}

frg_ast_t* frg_ast_clone_stmt_return(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_RETURN);

    return (frg_ast_t*)frg_ast_new_stmt_return(
        &ast->source_range,
        frg_ast_clone(((frg_ast_stmt_return_t*)ast)->value)
    );
}

frg_ast_t* frg_ast_clone_stmt_if(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_IF);

    return (frg_ast_t*)frg_ast_new_stmt_if(
        &ast->source_range,
        frg_ast_clone(((frg_ast_stmt_if_t*)ast)->condition),
        frg_ast_clone(((frg_ast_stmt_if_t*)ast)->then_clause),
        frg_ast_clone(((frg_ast_stmt_if_t*)ast)->else_clause)
    );
}

frg_ast_t* frg_ast_clone_stmt_while(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_WHILE);

    return (frg_ast_t*)frg_ast_new_stmt_while(
        &ast->source_range,
        frg_ast_clone(((frg_ast_stmt_while_t*)ast)->condition),
        frg_ast_clone(((frg_ast_stmt_while_t*)ast)->body)
    );
}

frg_ast_t* frg_ast_clone_stmt_block(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_BLOCK);

    return (frg_ast_t*)frg_ast_new_stmt_block(
        &ast->source_range,
        _frg_ast_clone_list(((frg_ast_stmt_block_t*)ast)->stmts)
    );
}

frg_ast_t* frg_ast_clone_value_int(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_INT);

    frg_ast_t* result = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    result->kind = ast->kind;
    result->source_range = ast->source_range;

    ((frg_ast_value_int_t*)result)->ty = frg_ast_clone(((frg_ast_value_int_t*)ast)->ty);
    ((frg_ast_value_int_t*)result)->value = ((frg_ast_value_int_t*)ast)->value;

    return result;
}

frg_ast_t* frg_ast_clone_value_float(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_FLOAT);

    frg_ast_t* result = frg_safe_malloc(sizeof(frg_ast_value_float_t));

    result->kind = ast->kind;
    result->source_range = ast->source_range;

    ((frg_ast_value_float_t*)result)->ty = frg_ast_clone(((frg_ast_value_float_t*)ast)->ty);
    ((frg_ast_value_float_t*)result)->value = ((frg_ast_value_float_t*)ast)->value;

    return result;
}

frg_ast_t* frg_ast_clone_value_char(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CHAR);

    return (frg_ast_t*)frg_ast_new_value_char(
        &ast->source_range,
        ((frg_ast_value_char_t*)ast)->value
    );
}

frg_ast_t* frg_ast_clone_value_str(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_STR);

    return (frg_ast_t*)frg_ast_new_value_str(
        &ast->source_range,
        g_string_new(((frg_ast_value_str_t*)ast)->value->str)
    );
}

frg_ast_t* frg_ast_clone_value_symbol(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_SYMBOL);

    return (frg_ast_t*)frg_ast_new_value_symbol(
        &ast->source_range,
        g_string_new(((frg_ast_value_symbol_t*)ast)->name->str)
    );
}

frg_ast_t* frg_ast_clone_value_call_kw_arg(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CALL_KW_ARG);

    return (frg_ast_t*)frg_ast_new_value_call_kw_arg(
        &ast->source_range,
        g_string_new(((frg_ast_value_call_kw_arg_t*)ast)->name->str),
        frg_ast_clone(((frg_ast_value_call_kw_arg_t*)ast)->value)
    );
}

frg_ast_t* frg_ast_clone_value_call(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CALL);

    return (frg_ast_t*)frg_ast_new_value_call(
        &ast->source_range,
        frg_ast_clone(((frg_ast_value_call_t*)ast)->callee),
        _frg_ast_clone_list(((frg_ast_value_call_t*)ast)->args),
        _frg_ast_clone_list(((frg_ast_value_call_t*)ast)->kw_args)
    );
}

frg_ast_t* frg_ast_clone_value_unary(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY);

    return (frg_ast_t*)frg_ast_new_value_unary(
        &ast->source_range,
        ast->kind,
        frg_ast_clone(((frg_ast_value_unary_t*)ast)->operand)
    );
}

frg_ast_t* frg_ast_clone_value_binary(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY);

    return (frg_ast_t*)frg_ast_new_value_binary(
        &ast->source_range,
        ast->kind,
        frg_ast_clone(((frg_ast_value_binary_t*)ast)->left),
        frg_ast_clone(((frg_ast_value_binary_t*)ast)->right)
    );
}
