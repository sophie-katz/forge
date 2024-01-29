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

#include <forge/ast/visitor_acceptors.h>
#include <forge/ast/visitor.h>
#include <forge/ast/kind_info.h>
#include <forge/common/error.h>

frg_ast_visitor_status_t _frg_ast_accept_recursive(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
);

frg_ast_visitor_status_t _frg_ast_accept_recursive_list(
    frg_ast_visitor_t* visitor,
    GList* parents,
    GList** list
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(list);
    
    GList* iter = *list;
    while (iter != NULL) {
        frg_ast_t* ast = iter->data;
        frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
            visitor,
            parents,
            &ast
        );
        if (status != FRG_AST_VISITOR_STATUS_OK) {
            return status;
        } else if (ast == NULL) {
            GList* next = iter->next;
            *list = g_list_delete_link(*list, iter);
            iter = next;
        } else {
            iter->data = ast;
            iter = iter->next;
        }
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_ty_pointer(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_TY_POINTER);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_ty_pointer_t*)*ast)->value
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_ty_fn(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_TY_FN);

    frg_ast_visitor_status_t status = status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_ty_fn_t*)*ast)->args
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_ty_fn_t*)*ast)->var_pos_args
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_ty_fn_t*)*ast)->var_kw_args
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_ty_fn_t*)*ast)->return_ty
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_decl_union(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_UNION);

    frg_ast_visitor_status_t status = status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_decl_union_t*)*ast)->props
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_decl_struct(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_STRUCT);

    frg_ast_visitor_status_t status = status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_decl_struct_t*)*ast)->decls
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_decl_prop(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_PROP);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_decl_prop_t*)*ast)->ty
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_decl_iface(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_IFACE);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_decl_iface_t*)*ast)->extends
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_decl_iface_t*)*ast)->decls
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_decl_fn_arg(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_FN_ARG);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        (frg_ast_t**)&((frg_ast_decl_fn_arg_t*)*ast)->prop
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_decl_fn_arg_t*)*ast)->default_value
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_decl_fn(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_FN);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        (frg_ast_t**)&((frg_ast_decl_fn_t*)*ast)->ty
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_decl_fn_t*)*ast)->body
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_decl_var(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_VAR);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        (frg_ast_t**)&((frg_ast_decl_var_t*)*ast)->prop
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_decl_var_t*)*ast)->initial_value
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_decl_block(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_DECL_BLOCK);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_decl_block_t*)*ast)->decls
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_stmt_return(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_STMT_RETURN);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_stmt_return_t*)*ast)->value
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_stmt_if(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_STMT_IF);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_stmt_if_t*)*ast)->condition
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_stmt_if_t*)*ast)->then_clause
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_stmt_if_t*)*ast)->else_clause
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_stmt_while(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_STMT_WHILE);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_stmt_while_t*)*ast)->condition
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_stmt_while_t*)*ast)->body
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}


frg_ast_visitor_status_t frg_ast_visitor_acceptor_stmt_block(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_STMT_BLOCK);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_stmt_block_t*)*ast)->stmts
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_int(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_VALUE_INT);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_value_int_t*)*ast)->ty
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_float(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_VALUE_FLOAT);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_value_float_t*)*ast)->ty
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_call_kw_arg(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_VALUE_CALL_KW_ARG);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_value_call_kw_arg_t*)*ast)->value
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_call(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert_int_eq((*ast)->kind, FRG_AST_KIND_VALUE_CALL);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_value_call_t*)*ast)->callee
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_value_call_t*)*ast)->args
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive_list(
        visitor,
        parents,
        &((frg_ast_value_call_t*)*ast)->kw_args
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_unary(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert(frg_ast_kind_info_get((*ast)->kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_value_unary_t*)*ast)->operand
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_visitor_acceptor_value_binary(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);
    frg_assert(frg_ast_kind_info_get((*ast)->kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY);

    frg_ast_visitor_status_t status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_value_binary_t*)*ast)->left
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    status = _frg_ast_accept_recursive(
        visitor,
        parents,
        &((frg_ast_value_binary_t*)*ast)->right
    );
    if (status != FRG_AST_VISITOR_STATUS_OK) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}
