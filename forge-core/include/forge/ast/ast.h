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

#pragma once

#include <forge/common/enums.h>
#include <forge/common/types.h>
#include <forge/common/lexical_casts.h>
#include <forge/parsing/domain.h>
#include <glib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    frg_ast_kind_t kind;
    frg_parsing_range_t source_range;
} frg_ast_t;

typedef void (*frg_ast_destructor)(frg_ast_t* ast);
typedef frg_ast_t* (*frg_ast_cloner)(const frg_ast_t* ast);
typedef const char* (*frg_ast_decl_name_getter)(const frg_ast_t* ast);
typedef void (*frg_ast_debug_printer)(
    frg_stream_output_t* stream,
    const frg_ast_t* ast,
    frg_indent_t indent
);

typedef struct {
    const char* name;
    frg_ast_kind_flags_t flags;
    frg_bit_width_t ty_bit_width;
    frg_ast_destructor destructor;
    frg_ast_cloner cloner;
    frg_ast_decl_name_getter decl_name_getter;
    frg_ast_debug_printer debug_printer;
} frg_ast_kind_info_t;

const frg_ast_kind_info_t* frg_ast_kind_info_get(frg_ast_kind_t kind);

typedef struct {
    frg_ast_t base;
    GString* name;
} frg_ast_ty_symbol_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* value;
} frg_ast_ty_pointer_t;

typedef struct {
    frg_ast_t base;
    GList* args;
    frg_ast_t* var_pos_args;
    frg_ast_t* var_kw_args;
    frg_ast_t* return_ty;
} frg_ast_ty_fn_t;

typedef struct {
    frg_ast_t base;
    GString* name;
    GList* props;
} frg_ast_decl_union_t;

typedef struct {
    frg_ast_t base;
    GString* name;
    GList* decls;
} frg_ast_decl_struct_t;

typedef struct {
    frg_ast_t base;
    frg_ast_decl_prop_flags_t flags;
    GString* name;
    frg_ast_t* ty;
} frg_ast_decl_prop_t;

typedef struct {
    frg_ast_t base;
    frg_ast_decl_iface_flags_t flags;
    GString* name;
    GList* extends;
    GList* decls;
} frg_ast_decl_iface_t;

typedef struct {
    frg_ast_t base;
    frg_ast_decl_fn_arg_flags_t flags;
    frg_ast_decl_prop_t* prop;
    frg_ast_t* default_value;
} frg_ast_decl_fn_arg_t;

typedef struct {
    frg_ast_t base;
    frg_ast_decl_fn_flags_t flags;
    GString* name;
    frg_ast_ty_fn_t* ty;
    frg_ast_t* body;
} frg_ast_decl_fn_t;

typedef struct {
    frg_ast_t base;
    frg_ast_decl_prop_t* prop;
    frg_ast_t* initial_value;
} frg_ast_decl_var_t;

typedef struct {
    frg_ast_t base;
    GList* decls;
} frg_ast_decl_block_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* value;
} frg_ast_stmt_return_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* condition;
    frg_ast_t* then_clause;
    frg_ast_t* else_clause;
} frg_ast_stmt_if_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* condition;
    frg_ast_t* body;
} frg_ast_stmt_while_t;

typedef struct {
    frg_ast_t base;
    GList* stmts;
} frg_ast_stmt_block_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* ty;
    union {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
    } value;
} frg_ast_value_int_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* ty;
    union {
        frg_f32_t f32;
        frg_f64_t f64;
    } value;
} frg_ast_value_float_t;

typedef struct {
    frg_ast_t base;
    frg_char_t value;
} frg_ast_value_char_t;

typedef struct {
    frg_ast_t base;
    GString* value;
} frg_ast_value_str_t;

typedef struct {
    frg_ast_t base;
    GString* name;
} frg_ast_value_symbol_t;

typedef struct {
    frg_ast_t base;
    GString* name;
    frg_ast_t* value;
} frg_ast_value_call_kw_arg_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* callee;
    GList* args;
    GList* kw_args;
} frg_ast_value_call_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* operand;
} frg_ast_value_unary_t;

typedef struct {
    frg_ast_t base;
    frg_ast_t* left;
    frg_ast_t* right;
} frg_ast_value_binary_t;

frg_ast_t* frg_ast_new_ty_primary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind
);

frg_ast_ty_symbol_t* frg_ast_new_ty_symbol(
    const frg_parsing_range_t *source_range,
    GString* name
);

frg_ast_ty_pointer_t* frg_ast_new_ty_pointer(
    const frg_parsing_range_t *source_range,
    frg_ast_t* value
);

frg_ast_ty_fn_t* frg_ast_new_ty_fn(
    const frg_parsing_range_t *source_range,
    GList* args,
    frg_ast_t* var_pos_args,
    frg_ast_t* var_kw_args,
    frg_ast_t* return_ty
);

frg_ast_decl_union_t* frg_ast_new_decl_union(
    const frg_parsing_range_t *source_range,
    GString* name,
    GList* props
);

frg_ast_decl_struct_t* frg_ast_new_decl_struct(
    const frg_parsing_range_t *source_range,
    GString* name,
    GList* decls
);

frg_ast_decl_prop_t* frg_ast_new_decl_prop(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_prop_flags_t flags,
    GString* name,
    frg_ast_t* ty
);

frg_ast_decl_iface_t* frg_ast_new_decl_iface(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_iface_flags_t flags,
    GString* name,
    GList* extends,
    GList* decls
);

frg_ast_decl_fn_arg_t* frg_ast_new_decl_fn_arg(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_fn_arg_flags_t flags,
    frg_ast_decl_prop_t* prop,
    frg_ast_t* default_value
);

frg_ast_decl_fn_t* frg_ast_new_decl_fn(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_fn_flags_t flags,
    GString* name,
    frg_ast_ty_fn_t* ty,
    frg_ast_t* body
);

frg_ast_decl_var_t* frg_ast_new_decl_var(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_prop_t* prop,
    frg_ast_t* initial_value
);

frg_ast_decl_block_t* frg_ast_new_decl_block(
    const frg_parsing_range_t *source_range,
    GList* decls
);

frg_ast_stmt_return_t* frg_ast_new_stmt_return(
    const frg_parsing_range_t *source_range,
    frg_ast_t* value
);

frg_ast_stmt_if_t* frg_ast_new_stmt_if(
    const frg_parsing_range_t *source_range,
    frg_ast_t* condition,
    frg_ast_t* then_clause,
    frg_ast_t* else_clause
);

frg_ast_stmt_while_t* frg_ast_new_stmt_while(
    const frg_parsing_range_t *source_range,
    frg_ast_t* condition,
    frg_ast_t* body
);

frg_ast_stmt_block_t* frg_ast_new_stmt_block(
    const frg_parsing_range_t *source_range,
    GList* stmts
);

frg_ast_t* frg_ast_new_value_primary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind
);

frg_ast_value_int_t* frg_ast_new_value_i8(
    const frg_parsing_range_t *source_range,
    int8_t value
);

frg_ast_value_int_t* frg_ast_new_value_i16(
    const frg_parsing_range_t *source_range,
    int16_t value
);

frg_ast_value_int_t* frg_ast_new_value_i32(
    const frg_parsing_range_t *source_range,
    int32_t value
);

frg_ast_value_int_t* frg_ast_new_value_i64(
    const frg_parsing_range_t *source_range,
    int64_t value
);

frg_ast_value_int_t* frg_ast_new_value_u8(
    const frg_parsing_range_t *source_range,
    uint8_t value
);

frg_ast_value_int_t* frg_ast_new_value_u16(
    const frg_parsing_range_t *source_range,
    uint16_t value
);

frg_ast_value_int_t* frg_ast_new_value_u32(
    const frg_parsing_range_t *source_range,
    uint32_t value
);

frg_ast_value_int_t* frg_ast_new_value_u64(
    const frg_parsing_range_t *source_range,
    uint64_t value
);

frg_ast_value_int_t* frg_ast_new_value_int_from_lexical_cast_result(
    const frg_parsing_range_t *source_range,
    const frg_parse_uint_result_t* result
);

frg_ast_value_float_t* frg_ast_new_value_f32(
    const frg_parsing_range_t *source_range,
    frg_f32_t value
);

frg_ast_value_float_t* frg_ast_new_value_f64(
    const frg_parsing_range_t *source_range,
    frg_f64_t value
);

frg_ast_value_float_t* frg_ast_new_value_float_from_lexical_cast_result(
    const frg_parsing_range_t *source_range,
    const frg_parse_float_result_t* result
);

frg_ast_value_char_t* frg_ast_new_value_char(
    const frg_parsing_range_t *source_range,
    frg_char_t value
);

frg_ast_value_str_t* frg_ast_new_value_str(
    const frg_parsing_range_t *source_range,
    GString* value
);

frg_ast_value_symbol_t* frg_ast_new_value_symbol(
    const frg_parsing_range_t *source_range,
    GString* name
);

frg_ast_value_call_kw_arg_t* frg_ast_new_value_call_kw_arg(
    const frg_parsing_range_t *source_range,
    GString* name,
    frg_ast_t* value
);

frg_ast_value_call_t* frg_ast_new_value_call(
    const frg_parsing_range_t *source_range,
    frg_ast_t* callee,
    GList* args,
    GList* kw_args
);

frg_ast_value_unary_t* frg_ast_new_value_unary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind,
    frg_ast_t* operand
);

frg_ast_value_binary_t* frg_ast_new_value_binary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind,
    frg_ast_t* left,
    frg_ast_t* right
);

void frg_ast_destroy(frg_ast_t** ast);

frg_ast_t* frg_ast_clone(const frg_ast_t* ast);

frg_ast_ty_symbol_t* frg_ast_try_cast_ty_symbol(frg_ast_t* ast);

frg_ast_decl_union_t* frg_ast_try_cast_decl_union(frg_ast_t* ast);

frg_ast_decl_struct_t* frg_ast_try_cast_decl_struct(frg_ast_t* ast);

frg_ast_decl_prop_t* frg_ast_try_cast_decl_prop(frg_ast_t* ast);

frg_ast_decl_iface_t* frg_ast_try_cast_decl_iface(frg_ast_t* ast);

frg_ast_decl_fn_arg_t* frg_ast_try_cast_decl_fn_arg(frg_ast_t* ast);

frg_ast_decl_fn_t* frg_ast_try_cast_decl_fn(frg_ast_t* ast);

frg_ast_decl_var_t* frg_ast_try_cast_decl_var(frg_ast_t* ast);

frg_ast_decl_block_t* frg_ast_try_cast_decl_block(frg_ast_t* ast);

frg_ast_stmt_return_t* frg_ast_try_cast_stmt_return(frg_ast_t* ast);

frg_ast_stmt_if_t* frg_ast_try_cast_stmt_if(frg_ast_t* ast);

frg_ast_stmt_while_t* frg_ast_try_cast_stmt_while(frg_ast_t* ast);

frg_ast_stmt_block_t* frg_ast_try_cast_stmt_block(frg_ast_t* ast);

frg_ast_value_int_t* frg_ast_try_cast_value_int(frg_ast_t* ast);

frg_ast_value_float_t* frg_ast_try_cast_value_float(frg_ast_t* ast);

frg_ast_value_char_t* frg_ast_try_cast_value_char(frg_ast_t* ast);

frg_ast_value_str_t* frg_ast_try_cast_value_str(frg_ast_t* ast);

frg_ast_value_symbol_t* frg_ast_try_cast_value_symbol(frg_ast_t* ast);

frg_ast_value_call_kw_arg_t* frg_ast_try_cast_value_call_kw_arg(frg_ast_t* ast);

frg_ast_value_call_t* frg_ast_try_cast_value_call(frg_ast_t* ast);

frg_ast_value_unary_t* frg_ast_try_cast_value_unary(frg_ast_t* ast);

frg_ast_value_binary_t* frg_ast_try_cast_value_binary(frg_ast_t* ast);

const char* frg_ast_get_decl_name(const frg_ast_t* ast);

const char* frg_ast_get_decl_fn_arg_name(const frg_ast_t* ast);

void frg_ast_print_debug(
    frg_stream_output_t* stream,
    const frg_ast_t* ast,
    frg_indent_t indent
);
