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

#include <forge/ast/ast.h>
#include <forge/common/memory.h>
#include <stddef.h>
#include <stdlib.h>

frg_status_t frg_ast_new_ty_primary(
    frg_ast_t** ast,
    frg_ast_id_t id
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (!frg_ast_id_is_ty_primary(id)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->id = id;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_ty_symbol(
    frg_ast_ty_symbol_t** ast,
    GString* name
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name->len == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_ty_symbol_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_TY_SYMBOL;
    (*ast)->name = name;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_decl_union(
    frg_ast_decl_union_t** ast,
    GString* name,
    GList* props
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name->len == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (props == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_decl_union_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_DECL_UNION;
    (*ast)->name = name;
    (*ast)->props = props;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_decl_struct(
    frg_ast_decl_struct_t** ast,
    GString* name,
    GList* decls
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name->len == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_decl_struct_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_DECL_STRUCT;
    (*ast)->name = name;
    (*ast)->decls = decls;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_decl_prop(
    frg_ast_decl_prop_t** ast,
    frg_ast_decl_prop_flags_t flags,
    GString* name,
    frg_ast_t* type
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name->len == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (type == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_decl_prop_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_DECL_PROP;
    (*ast)->flags = flags;
    (*ast)->name = name;
    (*ast)->type = type;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_decl_iface(
    frg_ast_decl_iface_t** ast,
    frg_ast_decl_iface_flags_t flags,
    GString* name,
    GList* extends,
    GList* decls
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name->len == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_decl_iface_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_DECL_IFACE;
    (*ast)->flags = flags;
    (*ast)->name = name;
    (*ast)->extends = extends;
    (*ast)->decls = decls;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_decl_fn_arg(
    frg_ast_decl_fn_arg_t** ast,
    frg_ast_decl_fn_arg_flags_t flags,
    frg_ast_t* prop,
    frg_ast_t* default_value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (prop == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_decl_fn_arg_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_DECL_FN_ARG;
    (*ast)->flags = flags;
    (*ast)->prop = prop;
    (*ast)->default_value = default_value;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_decl_fn(
    frg_ast_decl_fn_t** ast,
    frg_ast_decl_fn_flags_t flags,
    GString* name,
    GList* args,
    GList* var_pos_args,
    GList* var_kw_args,
    frg_ast_t* return_ty,
    frg_ast_t* body
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name->len == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_decl_fn_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_DECL_FN;
    (*ast)->flags = flags;
    (*ast)->name = name;
    (*ast)->args = args;
    (*ast)->var_pos_args = var_pos_args;
    (*ast)->var_kw_args = var_kw_args;
    (*ast)->return_ty = return_ty;
    (*ast)->body = body;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_decl_var(
    frg_ast_decl_var_t** ast,
    frg_ast_t* prop,
    frg_ast_t* initial_value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (prop == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (initial_value == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_decl_var_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_DECL_VAR;
    (*ast)->prop = prop;
    (*ast)->initial_value = initial_value;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_decl_block(
    frg_ast_decl_block_t** ast,
    GList* decls
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_decl_block_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_DECL_BLOCK;
    (*ast)->decls = decls;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_stmt_return(
    frg_ast_stmt_return_t** ast,
    frg_ast_t* value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_stmt_return_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_STMT_RETURN;
    (*ast)->value = value;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_stmt_if(
    frg_ast_stmt_if_t** ast,
    frg_ast_t* condition,
    frg_ast_t* then_clause,
    frg_ast_t* else_clause
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (condition == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (then_clause == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_stmt_if_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_STMT_IF;
    (*ast)->condition = condition;
    (*ast)->then_clause = then_clause;
    (*ast)->else_clause = else_clause;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_stmt_while(
    frg_ast_stmt_while_t** ast,
    frg_ast_t* condition,
    frg_ast_t* body
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (condition == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_stmt_while_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_STMT_WHILE;
    (*ast)->condition = condition;
    (*ast)->body = body;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_stmt_block(
    frg_ast_stmt_block_t** ast,
    GList* stmts
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_stmt_block_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_STMT_WHILE;
    (*ast)->stmts = stmts;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_primary(
    frg_ast_t** ast,
    frg_ast_id_t id
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (!frg_ast_id_is_value_primary(id)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->id = id;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_i8(
    frg_ast_value_int_t** ast,
    int8_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_int_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_INT;
    (*ast)->value.i8 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_I8);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_i16(
    frg_ast_value_int_t** ast,
    int16_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_int_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_INT;
    (*ast)->value.i16 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_I16);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_i32(
    frg_ast_value_int_t** ast,
    int32_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_int_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_INT;
    (*ast)->value.i32 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_I32);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_i64(
    frg_ast_value_int_t** ast,
    int64_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_int_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_INT;
    (*ast)->value.i64 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_I64);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_u8(
    frg_ast_value_int_t** ast,
    uint8_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_int_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_INT;
    (*ast)->value.u8 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_U8);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_u16(
    frg_ast_value_int_t** ast,
    uint16_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_int_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_INT;
    (*ast)->value.u16 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_U16);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_u32(
    frg_ast_value_int_t** ast,
    uint32_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_int_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_INT;
    (*ast)->value.u32 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_U32);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_u64(
    frg_ast_value_int_t** ast,
    uint64_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_int_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_INT;
    (*ast)->value.u64 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_U64);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_f32(
    frg_ast_value_float_t** ast,
    frg_f32_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_float_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_FLOAT;
    (*ast)->value.f32 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_F32);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_f64(
    frg_ast_value_float_t** ast,
    frg_f64_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_float_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_FLOAT;
    (*ast)->value.f64 = value;

    result = frg_ast_new_ty_primary(&(*ast)->ty, FRG_AST_ID_TY_F64);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_char(
    frg_ast_value_char_t** ast,
    frg_char_t value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_char_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_CHAR;
    (*ast)->value = value;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_str(
    frg_ast_value_str_t** ast,
    GString* value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (value == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_str_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_STR;
    (*ast)->value = value;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_symbol(
    frg_ast_value_symbol_t** ast,
    GString* name
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name->len == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_symbol_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_SYMBOL;
    (*ast)->name = name;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_call_kw_arg(
    frg_ast_value_call_kw_arg_t** ast,
    GString* name,
    frg_ast_t* value
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (name->len == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (value == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_call_kw_arg_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_CALL_KW_ARG;
    (*ast)->name = name;
    (*ast)->value = value;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_call(
    frg_ast_value_call_t** ast,
    frg_ast_t* callee,
    GList* args,
    GList* kw_args
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (callee == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_call_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = FRG_AST_ID_VALUE_CALL;
    (*ast)->callee = callee;
    (*ast)->args = args;
    (*ast)->kw_args = kw_args;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_unary(
    frg_ast_value_unary_t** ast,
    frg_ast_id_t id,
    frg_ast_t* operand
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (!frg_ast_id_is_value_unary(id)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE;
    } else if (operand == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_unary_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = id;
    (*ast)->operand = operand;

    return FRG_STATUS_OK;
}

frg_status_t frg_ast_new_value_binary(
    frg_ast_value_binary_t** ast,
    frg_ast_id_t id,
    frg_ast_t* left,
    frg_ast_t* right
) {
    if (ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (!frg_ast_id_is_value_binary(id)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE;
    } else if (left == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (right == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result = frg_safe_malloc((void**)ast, sizeof(frg_ast_value_binary_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*ast)->base.id = id;
    (*ast)->left = left;
    (*ast)->right = right;

    return FRG_STATUS_OK;
}

void _frg_ast_destroy_gfunc(gpointer data, gpointer user_data) {
    frg_status_t* result = (frg_status_t*)user_data;

    if (*result == FRG_STATUS_OK) {
        *result = frg_ast_destroy((frg_ast_t**)&data);
    }
}

frg_status_t _frg_asts_free_glist(GList* list) {
    frg_status_t result = FRG_STATUS_OK;

    g_list_foreach(list, _frg_ast_destroy_gfunc, &result);
    g_list_free(list);

    return result;
}

frg_status_t frg_ast_destroy(frg_ast_t** ast) {
    if (ast == NULL || *ast == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_status_t result;

    switch ((*ast)->id) {
        case FRG_AST_ID_TY_SYMBOL:
            g_string_free(((frg_ast_ty_symbol_t*)*ast)->name, TRUE);

            break;
        case FRG_AST_ID_DECL_UNION:
            g_string_free(((frg_ast_decl_union_t*)*ast)->name, TRUE);

            result = _frg_asts_free_glist(((frg_ast_decl_union_t*)*ast)->props);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_DECL_STRUCT:
            g_string_free(((frg_ast_decl_struct_t*)*ast)->name, TRUE);

            result = _frg_asts_free_glist(((frg_ast_decl_struct_t*)*ast)->decls);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_DECL_PROP:
            g_string_free(((frg_ast_decl_prop_t*)*ast)->name, TRUE);

            result = frg_ast_destroy(&((frg_ast_decl_prop_t*)*ast)->type);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_DECL_IFACE:
            g_string_free(((frg_ast_decl_iface_t*)*ast)->name, TRUE);

            result = _frg_asts_free_glist(((frg_ast_decl_iface_t*)*ast)->extends);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = _frg_asts_free_glist(((frg_ast_decl_iface_t*)*ast)->decls);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_DECL_FN_ARG:
            result = frg_ast_destroy(&((frg_ast_decl_fn_arg_t*)*ast)->prop);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = frg_ast_destroy(&((frg_ast_decl_fn_arg_t*)*ast)->default_value);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_DECL_FN:
            g_string_free(((frg_ast_decl_fn_t*)*ast)->name, TRUE);

            result = _frg_asts_free_glist(((frg_ast_decl_fn_t*)*ast)->args);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = _frg_asts_free_glist(((frg_ast_decl_fn_t*)*ast)->var_pos_args);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = _frg_asts_free_glist(((frg_ast_decl_fn_t*)*ast)->var_kw_args);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = frg_ast_destroy(&((frg_ast_decl_fn_t*)*ast)->body);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_DECL_VAR:
            result = frg_ast_destroy(&((frg_ast_decl_var_t*)*ast)->prop);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = frg_ast_destroy(&((frg_ast_decl_var_t*)*ast)->initial_value);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_DECL_BLOCK:
            result = _frg_asts_free_glist(((frg_ast_decl_block_t*)*ast)->decls);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_STMT_RETURN:
            result = frg_ast_destroy(&((frg_ast_stmt_return_t*)*ast)->value);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_STMT_IF:
            result = frg_ast_destroy(&((frg_ast_stmt_if_t*)*ast)->condition);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = frg_ast_destroy(&((frg_ast_stmt_if_t*)*ast)->then_clause);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = frg_ast_destroy(&((frg_ast_stmt_if_t*)*ast)->else_clause);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_STMT_WHILE:
            result = frg_ast_destroy(&((frg_ast_stmt_while_t*)*ast)->condition);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = frg_ast_destroy(&((frg_ast_stmt_while_t*)*ast)->body);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_STMT_BLOCK:
            result = _frg_asts_free_glist(((frg_ast_stmt_block_t*)*ast)->stmts);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_VALUE_INT:
            result = frg_ast_destroy(&((frg_ast_value_int_t*)*ast)->ty);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_VALUE_FLOAT:
            result = frg_ast_destroy(&((frg_ast_value_float_t*)*ast)->ty);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_VALUE_STR:
            g_string_free(((frg_ast_value_str_t*)*ast)->value, TRUE);

            break;
        case FRG_AST_ID_VALUE_SYMBOL:
            g_string_free(((frg_ast_value_symbol_t*)*ast)->name, TRUE);

            break;
        case FRG_AST_ID_VALUE_CALL_KW_ARG:
            g_string_free(((frg_ast_value_call_kw_arg_t*)*ast)->name, TRUE);

            result = frg_ast_destroy(&((frg_ast_value_call_kw_arg_t*)*ast)->value);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_VALUE_CALL:
            result = frg_ast_destroy(&((frg_ast_value_call_t*)*ast)->callee);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = _frg_asts_free_glist(((frg_ast_value_call_t*)*ast)->args);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = _frg_asts_free_glist(((frg_ast_value_call_t*)*ast)->kw_args);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        case FRG_AST_ID_VALUE_BIT_NOT:
        case FRG_AST_ID_VALUE_NEG:
        case FRG_AST_ID_VALUE_LOG_NOT:
        case FRG_AST_ID_VALUE_INC:
        case FRG_AST_ID_VALUE_DEC:
            result = frg_ast_destroy(&((frg_ast_value_unary_t*)*ast)->operand);
            if (result != FRG_STATUS_OK) {
                return result;
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
            result = frg_ast_destroy(&((frg_ast_value_binary_t*)*ast)->left);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            result = frg_ast_destroy(&((frg_ast_value_binary_t*)*ast)->right);
            if (result != FRG_STATUS_OK) {
                return result;
            }

            break;
        default:
            break;
    }

    return FRG_STATUS_OK;
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
