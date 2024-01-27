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

#include <forge/ast/destructors.h>
#include <forge/common/error.h>
#include <forge/common/memory.h>

void _frg_ast_destroy_glist(GList* list) {
    if (list == NULL) {
        return;
    }

    for (GList* iter = list; iter != NULL; iter = iter->next) {
        frg_ast_destroy((frg_ast_t**)&iter->data);
    }

    g_list_free(list);
}

void frg_ast_destroy_ty_symbol(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_SYMBOL);

    if (((frg_ast_ty_symbol_t*)ast)->name != NULL) {
        g_string_free(((frg_ast_ty_symbol_t*)ast)->name, TRUE);
    }
}

void frg_ast_destroy_ty_pointer(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_POINTER);

    if (((frg_ast_ty_pointer_t*)ast)->value != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_ty_pointer_t*)ast)->value);
    }
}

void frg_ast_destroy_ty_fn(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_TY_FN);

    _frg_ast_destroy_glist(((frg_ast_ty_fn_t*)ast)->args);

    if (((frg_ast_ty_fn_t*)ast)->var_pos_args != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_ty_fn_t*)ast)->var_pos_args);
    }

    if (((frg_ast_ty_fn_t*)ast)->var_kw_args != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_ty_fn_t*)ast)->var_kw_args);
    }

    if (((frg_ast_ty_fn_t*)ast)->return_ty != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_ty_fn_t*)ast)->return_ty);
    }
}

void frg_ast_destroy_decl_union(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_UNION);

    if (((frg_ast_decl_union_t*)ast)->name != NULL) {
        g_string_free(((frg_ast_decl_union_t*)ast)->name, TRUE);
    }

    _frg_ast_destroy_glist(((frg_ast_decl_union_t*)ast)->props);
}

void frg_ast_destroy_decl_struct(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_STRUCT);

    if (((frg_ast_decl_struct_t*)ast)->name != NULL) {
        g_string_free(((frg_ast_decl_struct_t*)ast)->name, TRUE);
    }

    _frg_ast_destroy_glist(((frg_ast_decl_struct_t*)ast)->decls);
}

void frg_ast_destroy_decl_prop(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_PROP);

    if (((frg_ast_decl_prop_t*)ast)->name != NULL) {
        g_string_free(((frg_ast_decl_prop_t*)ast)->name, TRUE);
    }

    if (((frg_ast_decl_prop_t*)ast)->ty != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_prop_t*)ast)->ty);
    }
}

void frg_ast_destroy_decl_iface(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_IFACE);

    if (((frg_ast_decl_iface_t*)ast)->name != NULL) {
        g_string_free(((frg_ast_decl_iface_t*)ast)->name, TRUE);
    }

    _frg_ast_destroy_glist(((frg_ast_decl_iface_t*)ast)->extends);
    _frg_ast_destroy_glist(((frg_ast_decl_iface_t*)ast)->decls);
}

void frg_ast_destroy_decl_fn_arg(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN_ARG);

    if (((frg_ast_decl_fn_arg_t*)ast)->prop != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_fn_arg_t*)ast)->prop);
    }

    if (((frg_ast_decl_fn_arg_t*)ast)->default_value != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_fn_arg_t*)ast)->default_value);
    }
}

void frg_ast_destroy_decl_fn(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN);

    if (((frg_ast_decl_fn_t*)ast)->name != NULL) {
        g_string_free(((frg_ast_decl_fn_t*)ast)->name, TRUE);
    }

    if (((frg_ast_decl_fn_t*)ast)->ty != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_fn_t*)ast)->ty);
    }

    if (((frg_ast_decl_fn_t*)ast)->body != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_fn_t*)ast)->body);
    }
}

void frg_ast_destroy_decl_var(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_VAR);

    if (((frg_ast_decl_var_t*)ast)->prop != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_var_t*)ast)->prop);
    }

    if (((frg_ast_decl_var_t*)ast)->initial_value != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_decl_var_t*)ast)->initial_value);
    }
}

void frg_ast_destroy_decl_block(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_BLOCK);

    _frg_ast_destroy_glist(((frg_ast_decl_block_t*)ast)->decls);
}

void frg_ast_destroy_stmt_return(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_RETURN);

    if (((frg_ast_stmt_return_t*)ast)->value != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_stmt_return_t*)ast)->value);
    }
}

void frg_ast_destroy_stmt_if(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_IF);

    if (((frg_ast_stmt_if_t*)ast)->condition != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_stmt_if_t*)ast)->condition);
    }

    if (((frg_ast_stmt_if_t*)ast)->then_clause != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_stmt_if_t*)ast)->then_clause);
    }

    if (((frg_ast_stmt_if_t*)ast)->else_clause != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_stmt_if_t*)ast)->else_clause);
    }
}

void frg_ast_destroy_stmt_while(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_WHILE);

    if (((frg_ast_stmt_while_t*)ast)->condition != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_stmt_while_t*)ast)->condition);
    }

    if (((frg_ast_stmt_while_t*)ast)->body != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_stmt_while_t*)ast)->body);
    }
}

void frg_ast_destroy_stmt_block(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_STMT_BLOCK);

    _frg_ast_destroy_glist(((frg_ast_stmt_block_t*)ast)->stmts);
}

void frg_ast_destroy_value_int(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_INT);

    if (((frg_ast_value_int_t*)ast)->ty != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_value_int_t*)ast)->ty);
    }
}

void frg_ast_destroy_value_float(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_INT);

    if (((frg_ast_value_float_t*)ast)->ty != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_value_float_t*)ast)->ty);
    }
}

void frg_ast_destroy_value_str(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_STR);

    if (((frg_ast_value_str_t*)ast)->value != NULL) {
        g_string_free(((frg_ast_value_str_t*)ast)->value, TRUE);
    }
}

void frg_ast_destroy_value_symbol(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_SYMBOL);

    if (((frg_ast_value_symbol_t*)ast)->name != NULL) {
        g_string_free(((frg_ast_value_symbol_t*)ast)->name, TRUE);
    }
}

void frg_ast_destroy_value_call_kw_arg(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CALL_KW_ARG);

    if (((frg_ast_value_call_kw_arg_t*)ast)->name != NULL) {
        g_string_free(((frg_ast_value_call_kw_arg_t*)ast)->name, TRUE);
    }

    if (((frg_ast_value_call_kw_arg_t*)ast)->value != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_value_call_kw_arg_t*)ast)->value);
    }
}

void frg_ast_destroy_value_call(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_VALUE_CALL);

    if (((frg_ast_value_call_t*)ast)->callee != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_value_call_t*)ast)->callee);
    }

    _frg_ast_destroy_glist(((frg_ast_value_call_t*)ast)->args);
    _frg_ast_destroy_glist(((frg_ast_value_call_t*)ast)->kw_args);
}

void frg_ast_destroy_value_unary(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY);

    if (((frg_ast_value_unary_t*)ast)->operand != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_value_unary_t*)ast)->operand);
    }
}

void frg_ast_destroy_value_binary(frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY);

    if (((frg_ast_value_binary_t*)ast)->left != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_value_binary_t*)ast)->left);
    }

    if (((frg_ast_value_binary_t*)ast)->right != NULL) {
        frg_ast_destroy((frg_ast_t**)&((frg_ast_value_binary_t*)ast)->right);
    }
}

