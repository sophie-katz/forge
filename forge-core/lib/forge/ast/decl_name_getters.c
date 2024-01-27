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

#include <forge/ast/decl_name_getters.h>
#include <forge/common/error.h>
#include <forge/common/memory.h>

const char* frg_ast_decl_name_getter_decl_union(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_UNION);

    return ((frg_ast_decl_union_t*)ast)->name->str;
}

const char* frg_ast_decl_name_getter_decl_struct(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_STRUCT);

    return ((frg_ast_decl_struct_t*)ast)->name->str;
}

const char* frg_ast_decl_name_getter_decl_prop(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_PROP);

    return ((frg_ast_decl_prop_t*)ast)->name->str;
}

const char* frg_ast_decl_name_getter_decl_iface(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_IFACE);

    return ((frg_ast_decl_iface_t*)ast)->name->str;
}

const char* frg_ast_decl_name_getter_decl_fn_arg(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN_ARG);

    return ((frg_ast_decl_prop_t*)(((frg_ast_decl_fn_arg_t*)ast)->prop))->name->str;
}

const char* frg_ast_decl_name_getter_decl_fn(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN);

    return ((frg_ast_decl_fn_t*)ast)->name->str;
}

const char* frg_ast_decl_name_getter_decl_var(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_VAR);

    return ((frg_ast_decl_prop_t*)(((frg_ast_decl_var_t*)ast)->prop))->name->str;
}

