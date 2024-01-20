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

#include <forge/ast/visitor.h>
#include <forge/common/memory.h>
#include <forge/common/error.h>

frg_ast_visitor_entry_item_t* frg_ast_visitor_entry_item_new(
    frg_ast_visitor_callback_t callback_pre,
    frg_ast_visitor_callback_t callback_post
) {
    frg_ast_visitor_entry_item_t* item = frg_safe_malloc(sizeof(frg_ast_visitor_entry_item_t));
    item->callback_pre = callback_pre;
    item->callback_post = callback_post;
    return item;
}

void frg_ast_visitor_entry_item_destroy(frg_ast_visitor_entry_item_t** item) {
    frg_safe_free((void**)item);
}

frg_ast_visitor_t* frg_ast_visitor_new(void* user_data) {
    frg_ast_visitor_t* visitor = frg_safe_malloc(sizeof(frg_ast_visitor_t));
    memset(visitor->entries, 0, sizeof(visitor->entries));
    visitor->user_data = user_data;
    return visitor;
}

void frg_ast_visitor_destroy(frg_ast_visitor_t** visitor) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(*visitor);

    for (frg_ast_id_t id = FRG_AST_ID_FIRST; id <= FRG_AST_ID_LAST; id++) {
        if ((*visitor)->entries[id].items != NULL) {
            for (GList* iter = (*visitor)->entries[id].items; iter != NULL; iter = iter->next) {
                frg_ast_visitor_entry_item_destroy((frg_ast_visitor_entry_item_t**)&iter->data);
            }

            g_list_free((*visitor)->entries[id].items);
        }
    }

    frg_safe_free((void**)visitor);
}

void frg_ast_visitor_add_entry(
    frg_ast_visitor_t* visitor,
    frg_ast_id_t id,
    frg_ast_visitor_callback_t callback_pre,
    frg_ast_visitor_callback_t callback_post
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_int_ge(id, FRG_AST_ID_FIRST);
    frg_assert_int_le(id, FRG_AST_ID_LAST);

    frg_ast_visitor_entry_item_t* item = frg_ast_visitor_entry_item_new(
        callback_pre,
        callback_post
    );

    visitor->entries[id].items = g_list_append(
        visitor->entries[id].items,
        item
    );
}

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
            *list = g_list_delete_link(*list, iter);
        } else {
            iter->data = ast;
        }

        iter = iter->next;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t _frg_ast_visitor_call_entry(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast,
    bool pre
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);

    for (GList* iter = visitor->entries[(*ast)->id].items; iter != NULL; iter = iter->next) {
        frg_ast_visitor_entry_item_t* item = iter->data;
        frg_ast_visitor_callback_t callback = (pre ? item->callback_pre : item->callback_post);
        if (callback != NULL) {
            frg_ast_visitor_status_t status = callback(
                parents,
                ast,
                visitor->user_data
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }
        }
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t _frg_ast_accept_recursive(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
) {
    frg_assert_pointer_non_null(visitor);
    frg_assert_pointer_non_null(ast);
    
    if (*ast == NULL) {
        return FRG_AST_VISITOR_STATUS_OK;
    }

    frg_ast_visitor_status_t status = _frg_ast_visitor_call_entry(
        visitor,
        parents,
        ast,
        true
    );
    if (status == FRG_AST_VISITOR_STATUS_SKIP) {
        return FRG_AST_VISITOR_STATUS_OK;
    } else if (status == FRG_AST_VISITOR_STATUS_STOP) {
        return status;
    }

    if (*ast == NULL) {
        return FRG_AST_VISITOR_STATUS_OK;
    }

    parents = g_list_prepend(parents, *ast);
    status = FRG_AST_VISITOR_STATUS_OK;

    switch ((*ast)->id) {
        case FRG_AST_ID_TY_POINTER:
            status = _frg_ast_accept_recursive(
                visitor,
                parents,
                &((frg_ast_ty_pointer_t*)*ast)->value
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_TY_FN:
            status = _frg_ast_accept_recursive_list(
                visitor,
                parents,
                &((frg_ast_ty_fn_t*)*ast)->args
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            status = _frg_ast_accept_recursive_list(
                visitor,
                parents,
                &((frg_ast_ty_fn_t*)*ast)->var_pos_args
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            status = _frg_ast_accept_recursive_list(
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

            break;
        case FRG_AST_ID_DECL_UNION:
            status = _frg_ast_accept_recursive_list(
                visitor,
                parents,
                &((frg_ast_decl_union_t*)*ast)->props
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_DECL_STRUCT:
            status = _frg_ast_accept_recursive_list(
                visitor,
                parents,
                &((frg_ast_decl_struct_t*)*ast)->decls
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_DECL_PROP:
            status = _frg_ast_accept_recursive(
                visitor,
                parents,
                &((frg_ast_decl_prop_t*)*ast)->ty
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_DECL_IFACE:
            status = _frg_ast_accept_recursive_list(
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

            break;
        case FRG_AST_ID_DECL_FN_ARG:
            status = _frg_ast_accept_recursive(
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

            break;
        case FRG_AST_ID_DECL_FN:
            status = _frg_ast_accept_recursive(
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

            break;
        case FRG_AST_ID_DECL_VAR:
            status = _frg_ast_accept_recursive(
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

            break;
        case FRG_AST_ID_DECL_BLOCK:
            status = _frg_ast_accept_recursive_list(
                visitor,
                parents,
                &((frg_ast_decl_block_t*)*ast)->decls
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_STMT_RETURN:
            status = _frg_ast_accept_recursive(
                visitor,
                parents,
                &((frg_ast_stmt_return_t*)*ast)->value
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_STMT_IF:
            status = _frg_ast_accept_recursive(
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

            break;
        case FRG_AST_ID_STMT_WHILE:
            status = _frg_ast_accept_recursive(
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

            break;
        case FRG_AST_ID_STMT_BLOCK:
            status = _frg_ast_accept_recursive_list(
                visitor,
                parents,
                &((frg_ast_stmt_block_t*)*ast)->stmts
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_VALUE_INT:
            status = _frg_ast_accept_recursive(
                visitor,
                parents,
                &((frg_ast_value_int_t*)*ast)->ty
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_VALUE_FLOAT:
            status = _frg_ast_accept_recursive(
                visitor,
                parents,
                &((frg_ast_value_float_t*)*ast)->ty
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_VALUE_CALL_KW_ARG:
            status = _frg_ast_accept_recursive(
                visitor,
                parents,
                &((frg_ast_value_call_kw_arg_t*)*ast)->value
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

            break;
        case FRG_AST_ID_VALUE_CALL:
            status = _frg_ast_accept_recursive(
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

            break;
        case FRG_AST_ID_VALUE_DEREF:
        case FRG_AST_ID_VALUE_GETADDR:
        case FRG_AST_ID_VALUE_BIT_NOT:
        case FRG_AST_ID_VALUE_NEG:
        case FRG_AST_ID_VALUE_LOG_NOT:
        case FRG_AST_ID_VALUE_INC:
        case FRG_AST_ID_VALUE_DEC:
            status = _frg_ast_accept_recursive(
                visitor,
                parents,
                &((frg_ast_value_unary_t*)*ast)->operand
            );
            if (status != FRG_AST_VISITOR_STATUS_OK) {
                return status;
            }

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
            status = _frg_ast_accept_recursive(
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

            break;
        default:
            break;
    }

    parents = g_list_delete_link(parents, parents);

    status = _frg_ast_visitor_call_entry(
        visitor,
        parents,
        ast,
        false
    );
    if (status == FRG_AST_VISITOR_STATUS_STOP) {
        return status;
    }

    return FRG_AST_VISITOR_STATUS_OK;
}

frg_ast_visitor_status_t frg_ast_accept(
    frg_ast_visitor_t* visitor,
    frg_ast_t** ast
) {
    return _frg_ast_accept_recursive(
        visitor,
        NULL,
        ast
    );
}
