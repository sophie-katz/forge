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

#include <forge/ast/ast.h>
#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/common/debug.h>
#include <stddef.h>
#include <stdlib.h>
#include <forge/ast/kind_info.h>

frg_ast_t* frg_ast_new_ty_primary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind
) {
    frg_assert(frg_ast_kind_info_get(kind)->flags & FRG_AST_KIND_FLAG_TY_PRIMARY);

    frg_ast_t* ast = frg_safe_malloc(sizeof(frg_ast_t));
    ast->source_range = *source_range;
    ast->kind = kind;

    return ast;
}

frg_ast_ty_symbol_t* frg_ast_new_ty_symbol(
    const frg_parsing_range_t *source_range,
    GString* name
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_ty_symbol_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_symbol_t));

    ast->base.kind = FRG_AST_KIND_TY_SYMBOL;
    ast->base.source_range = *source_range;
    ast->name = name;

    return ast;
}

frg_ast_ty_pointer_t* frg_ast_new_ty_pointer(
    const frg_parsing_range_t *source_range,
    frg_ast_t* value
) {
    frg_assert_pointer_non_null(value);

    frg_ast_ty_pointer_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_pointer_t));

    ast->base.kind = FRG_AST_KIND_TY_POINTER;
    ast->base.source_range = *source_range;
    ast->value = value;

    return ast;
}

frg_ast_ty_fn_t* frg_ast_new_ty_fn(
    const frg_parsing_range_t *source_range,
    GList* args,
    frg_ast_t* var_pos_args,
    frg_ast_t* var_kw_args,
    frg_ast_t* return_ty
) {
    frg_ast_ty_fn_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_fn_t));

    ast->base.kind = FRG_AST_KIND_TY_FN;
    ast->base.source_range = *source_range;
    ast->args = args;
    ast->var_pos_args = var_pos_args;
    ast->var_kw_args = var_kw_args;
    ast->return_ty = return_ty;

    return ast;
}

frg_ast_decl_union_t* frg_ast_new_decl_union(
    const frg_parsing_range_t *source_range,
    GString* name,
    GList* props
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(props);

    frg_ast_decl_union_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_union_t));

    ast->base.kind = FRG_AST_KIND_DECL_UNION;
    ast->base.source_range = *source_range;
    ast->name = name;
    ast->props = props;

    return ast;
}

frg_ast_decl_struct_t* frg_ast_new_decl_struct(
    const frg_parsing_range_t *source_range,
    GString* name,
    GList* decls
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_struct_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_struct_t));

    ast->base.kind = FRG_AST_KIND_DECL_STRUCT;
    ast->base.source_range = *source_range;
    ast->name = name;
    ast->decls = decls;

    return ast;
}

frg_ast_decl_prop_t* frg_ast_new_decl_prop(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_prop_flags_t flags,
    GString* name,
    frg_ast_t* ty
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_prop_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_prop_t));

    ast->base.kind = FRG_AST_KIND_DECL_PROP;
    ast->base.source_range = *source_range;
    ast->flags = flags;
    ast->name = name;
    ast->ty = ty;

    return ast;
}

frg_ast_decl_iface_t* frg_ast_new_decl_iface(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_iface_flags_t flags,
    GString* name,
    GList* extends,
    GList* decls
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_iface_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_iface_t));

    ast->base.kind = FRG_AST_KIND_DECL_IFACE;
    ast->base.source_range = *source_range;
    ast->flags = flags;
    ast->name = name;
    ast->extends = extends;
    ast->decls = decls;

    return ast;
}

frg_ast_decl_fn_arg_t* frg_ast_new_decl_fn_arg(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_fn_arg_flags_t flags,
    frg_ast_decl_prop_t* prop,
    frg_ast_t* default_value
) {
    frg_assert_pointer_non_null(prop);

    frg_ast_decl_fn_arg_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_fn_arg_t));

    ast->base.kind = FRG_AST_KIND_DECL_FN_ARG;
    ast->base.source_range = *source_range;
    ast->flags = flags;
    ast->prop = prop;
    ast->default_value = default_value;

    return ast;
}

frg_ast_decl_fn_t* frg_ast_new_decl_fn(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_fn_flags_t flags,
    GString* name,
    frg_ast_ty_fn_t* ty,
    frg_ast_t* body
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(ty);

    frg_ast_decl_fn_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_fn_t));

    ast->base.kind = FRG_AST_KIND_DECL_FN;
    ast->base.source_range = *source_range;
    ast->flags = flags;
    ast->name = name;
    ast->ty = ty;
    ast->body = body;

    return ast;
}

frg_ast_decl_var_t* frg_ast_new_decl_var(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_prop_t* prop,
    frg_ast_t* initial_value
) {
    frg_assert_pointer_non_null(prop);

    frg_ast_decl_var_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_var_t));

    ast->base.kind = FRG_AST_KIND_DECL_VAR;
    ast->base.source_range = *source_range;
    ast->prop = prop;
    ast->initial_value = initial_value;

    return ast;
}

frg_ast_decl_block_t* frg_ast_new_decl_block(
    const frg_parsing_range_t *source_range,
    GList* decls
) {
    frg_ast_decl_block_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_block_t));

    ast->base.kind = FRG_AST_KIND_DECL_BLOCK;
    ast->base.source_range = *source_range;
    ast->decls = decls;

    return ast;
}

frg_ast_stmt_return_t* frg_ast_new_stmt_return(
    const frg_parsing_range_t *source_range,
    frg_ast_t* value
) {
    frg_ast_stmt_return_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_return_t));

    ast->base.kind = FRG_AST_KIND_STMT_RETURN;
    ast->base.source_range = *source_range;
    ast->value = value;

    return ast;
}

frg_ast_stmt_if_t* frg_ast_new_stmt_if(
    const frg_parsing_range_t *source_range,
    frg_ast_t* condition,
    frg_ast_t* then_clause,
    frg_ast_t* else_clause
) {
    frg_assert_pointer_non_null(condition);
    frg_assert_pointer_non_null(then_clause);

    frg_ast_stmt_if_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_if_t));

    ast->base.kind = FRG_AST_KIND_STMT_IF;
    ast->base.source_range = *source_range;
    ast->condition = condition;
    ast->then_clause = then_clause;
    ast->else_clause = else_clause;

    return ast;
}

frg_ast_stmt_while_t* frg_ast_new_stmt_while(
    const frg_parsing_range_t *source_range,
    frg_ast_t* condition,
    frg_ast_t* body
) {
    frg_assert_pointer_non_null(condition);

    frg_ast_stmt_while_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_while_t));

    ast->base.kind = FRG_AST_KIND_STMT_WHILE;
    ast->base.source_range = *source_range;
    ast->condition = condition;
    ast->body = body;

    return ast;
}

frg_ast_stmt_block_t* frg_ast_new_stmt_block(
    const frg_parsing_range_t *source_range,
    GList* stmts
) {
    frg_ast_stmt_block_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_block_t));

    ast->base.kind = FRG_AST_KIND_STMT_BLOCK;
    ast->base.source_range = *source_range;
    ast->stmts = stmts;

    return ast;
}

frg_ast_t* frg_ast_new_value_primary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind
) {
    frg_assert(frg_ast_kind_info_get(kind)->flags & FRG_AST_KIND_FLAG_VALUE_PRIMARY);

    frg_ast_t* ast = frg_safe_malloc(sizeof(frg_ast_t));

    ast->kind = kind;
    ast->source_range = *source_range;

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_i8(
    const frg_parsing_range_t *source_range,
    int8_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.i8 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_I8);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_i16(
    const frg_parsing_range_t *source_range,
    int16_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.i16 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_I16);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_i32(
    const frg_parsing_range_t *source_range,
    int32_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.i32 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_I32);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_i64(
    const frg_parsing_range_t *source_range,
    int64_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.i64 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_I64);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_u8(
    const frg_parsing_range_t *source_range,
    uint8_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.u8 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_U8);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_u16(
    const frg_parsing_range_t *source_range,
    uint16_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.u16 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_U16);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_u32(
    const frg_parsing_range_t *source_range,
    uint32_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.u32 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_U32);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_u64(
    const frg_parsing_range_t *source_range,
    uint64_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.u64 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_U64);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_int_from_lexical_cast_result(
    const frg_parsing_range_t *source_range,
    const frg_parse_uint_result_t* result
) {
    frg_assert_pointer_non_null(result);

    if (result->bit_width == 8) {
        if (result->is_signed) {
            return frg_ast_new_value_i8(source_range, (int8_t)result->value);
        } else {
            return frg_ast_new_value_u8(source_range, (uint8_t)result->value);
        }
    } else if (result->bit_width == 16) {
        if (result->is_signed) {
            return frg_ast_new_value_i16(source_range, (int16_t)result->value);
        } else {
            return frg_ast_new_value_u16(source_range, (uint16_t)result->value);
        }
    } else if (result->bit_width == 32) {
        if (result->is_signed) {
            return frg_ast_new_value_i32(source_range, (int32_t)result->value);
        } else {
            return frg_ast_new_value_u32(source_range, (uint32_t)result->value);
        }
    } else if (result->bit_width == 64) {
        if (result->is_signed) {
            return frg_ast_new_value_i64(source_range, (int64_t)result->value);
        } else {
            return frg_ast_new_value_u64(source_range, (uint64_t)result->value);
        }
    } else {
        frg_die("unexpected bit width %i", result->bit_width);
    }
}

frg_ast_value_float_t* frg_ast_new_value_f32(
    const frg_parsing_range_t *source_range,
    frg_f32_t value
) {
    frg_ast_value_float_t* ast = frg_safe_malloc(sizeof(frg_ast_value_float_t));

    ast->base.kind = FRG_AST_KIND_VALUE_FLOAT;
    ast->base.source_range = *source_range;
    ast->value.f32 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_F32);

    return ast;
}

frg_ast_value_float_t* frg_ast_new_value_f64(
    const frg_parsing_range_t *source_range,
    frg_f64_t value
) {
    frg_ast_value_float_t* ast = frg_safe_malloc(sizeof(frg_ast_value_float_t));

    ast->base.kind = FRG_AST_KIND_VALUE_FLOAT;
    ast->base.source_range = *source_range;
    ast->value.f64 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_F64);

    return ast;
}

frg_ast_value_float_t* frg_ast_new_value_float_from_lexical_cast_result(
    const frg_parsing_range_t *source_range,
    const frg_parse_float_result_t* result
) {
    frg_assert_pointer_non_null(result);

    if (result->bit_width == 32) {
        return frg_ast_new_value_f32(source_range, (frg_f32_t)result->value);
    } else if (result->bit_width == 64) {
        return frg_ast_new_value_f64(source_range, (frg_f64_t)result->value);
    } else {
        frg_die("unexpected bit width %i", result->bit_width);
    }
}

frg_ast_value_char_t* frg_ast_new_value_char(
    const frg_parsing_range_t *source_range,
    frg_char_t value
) {
    frg_ast_value_char_t* ast = frg_safe_malloc(sizeof(frg_ast_value_char_t));

    ast->base.kind = FRG_AST_KIND_VALUE_CHAR;
    ast->base.source_range = *source_range;
    ast->value = value;

    return ast;
}

frg_ast_value_str_t* frg_ast_new_value_str(
    const frg_parsing_range_t *source_range,
    GString* value
) {
    frg_assert_pointer_non_null(value);

    frg_ast_value_str_t* ast = frg_safe_malloc(sizeof(frg_ast_value_str_t));

    ast->base.kind = FRG_AST_KIND_VALUE_STR;
    ast->base.source_range = *source_range;
    ast->value = value;

    return ast;
}

frg_ast_value_symbol_t* frg_ast_new_value_symbol(
    const frg_parsing_range_t *source_range,
    GString* name
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_value_symbol_t* ast = frg_safe_malloc(sizeof(frg_ast_value_symbol_t));

    ast->base.kind = FRG_AST_KIND_VALUE_SYMBOL;
    ast->base.source_range = *source_range;
    ast->name = name;

    return ast;
}

frg_ast_value_call_kw_arg_t* frg_ast_new_value_call_kw_arg(
    const frg_parsing_range_t *source_range,
    GString* name,
    frg_ast_t* value
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(value);

    frg_ast_value_call_kw_arg_t* ast = frg_safe_malloc(sizeof(frg_ast_value_call_kw_arg_t));

    ast->base.kind = FRG_AST_KIND_VALUE_CALL_KW_ARG;
    ast->base.source_range = *source_range;
    ast->name = name;
    ast->value = value;

    return ast;
}

frg_ast_value_call_t* frg_ast_new_value_call(
    const frg_parsing_range_t *source_range,
    frg_ast_t* callee,
    GList* args,
    GList* kw_args
) {
    frg_assert_pointer_non_null(callee);

    frg_ast_value_call_t* ast = frg_safe_malloc(sizeof(frg_ast_value_call_t));

    ast->base.kind = FRG_AST_KIND_VALUE_CALL;
    ast->base.source_range = *source_range;
    ast->callee = callee;
    ast->args = args;
    ast->kw_args = kw_args;

    return ast;
}

frg_ast_value_unary_t* frg_ast_new_value_unary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind,
    frg_ast_t* operand
) {
    frg_assert(frg_ast_kind_info_get(kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY);
    frg_assert_pointer_non_null(operand);

    frg_ast_value_unary_t* ast = frg_safe_malloc(sizeof(frg_ast_value_unary_t));

    ast->base.kind = kind;
    ast->base.source_range = *source_range;
    ast->operand = operand;

    return ast;
}

frg_ast_value_binary_t* frg_ast_new_value_binary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind,
    frg_ast_t* left,
    frg_ast_t* right
) {
    frg_assert(frg_ast_kind_info_get(kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY);
    frg_assert_pointer_non_null(left);
    frg_assert_pointer_non_null(right);

    frg_ast_value_binary_t* ast = frg_safe_malloc(sizeof(frg_ast_value_binary_t));

    ast->base.kind = kind;
    ast->base.source_range = *source_range;
    ast->left = left;
    ast->right = right;

    return ast;
}

void frg_ast_destroy(frg_ast_t** ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);

    frg_ast_destructor_t destructor = frg_ast_kind_info_get((*ast)->kind)->destructor;
    
    if (destructor != NULL) {
        destructor(*ast);
    }

    frg_safe_free((void**)ast);
}

frg_ast_t* frg_ast_clone(const frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    }

    return frg_ast_kind_info_get(ast->kind)->cloner(ast);
}

frg_ast_ty_symbol_t* frg_ast_try_cast_ty_symbol(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_TY_SYMBOL) {
        return (frg_ast_ty_symbol_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_union_t* frg_ast_try_cast_decl_union(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_UNION) {
        return (frg_ast_decl_union_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_struct_t* frg_ast_try_cast_decl_struct(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_STRUCT) {
        return (frg_ast_decl_struct_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_prop_t* frg_ast_try_cast_decl_prop(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_PROP) {
        return (frg_ast_decl_prop_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_iface_t* frg_ast_try_cast_decl_iface(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_IFACE) {
        return (frg_ast_decl_iface_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_fn_arg_t* frg_ast_try_cast_decl_fn_arg(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_FN_ARG) {
        return (frg_ast_decl_fn_arg_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_fn_t* frg_ast_try_cast_decl_fn(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_FN) {
        return (frg_ast_decl_fn_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_var_t* frg_ast_try_cast_decl_var(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_VAR) {
        return (frg_ast_decl_var_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_block_t* frg_ast_try_cast_decl_block(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_BLOCK) {
        return (frg_ast_decl_block_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_return_t* frg_ast_try_cast_stmt_return(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_STMT_RETURN) {
        return (frg_ast_stmt_return_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_if_t* frg_ast_try_cast_stmt_if(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_STMT_IF) {
        return (frg_ast_stmt_if_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_while_t* frg_ast_try_cast_stmt_while(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_STMT_WHILE) {
        return (frg_ast_stmt_while_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_block_t* frg_ast_try_cast_stmt_block(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_STMT_BLOCK) {
        return (frg_ast_stmt_block_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_int_t* frg_ast_try_cast_value_int(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_INT) {
        return (frg_ast_value_int_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_float_t* frg_ast_try_cast_value_float(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_FLOAT) {
        return (frg_ast_value_float_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_char_t* frg_ast_try_cast_value_char(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_CHAR) {
        return (frg_ast_value_char_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_str_t* frg_ast_try_cast_value_str(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_STR) {
        return (frg_ast_value_str_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_symbol_t* frg_ast_try_cast_value_symbol(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_SYMBOL) {
        return (frg_ast_value_symbol_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_call_kw_arg_t* frg_ast_try_cast_value_call_kw_arg(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_CALL_KW_ARG) {
        return (frg_ast_value_call_kw_arg_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_call_t* frg_ast_try_cast_value_call(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_CALL) {
        return (frg_ast_value_call_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_unary_t* frg_ast_try_cast_value_unary(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY) {
        return (frg_ast_value_unary_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_binary_t* frg_ast_try_cast_value_binary(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY) {
        return (frg_ast_value_binary_t*)ast;
    } else {
        return NULL;
    }
}
