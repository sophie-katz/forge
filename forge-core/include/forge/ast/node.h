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

#include <forge/enums.h>
#include <forge/lexical_casts.h>
#include <forge/parsing/domain.h>
#include <forge/types.h>
#include <glib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  frg_ast_node_kind_t kind;
  frg_parsing_range_t source_range;
} frg_ast_node_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_type_int_flags_t flags;
  frg_bit_width_t bit_width;
} frg_ast_node_type_int_t;

typedef struct {
  frg_ast_node_t base;
  frg_bit_width_t bit_width;
} frg_ast_node_type_float_t;

typedef struct {
  frg_ast_node_t base;
  GString* name;
} frg_ast_node_type_symbol_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_type_pointer_flags_t flags;
  frg_ast_node_t* value;
} frg_ast_node_type_pointer_t;

typedef struct {
  frg_ast_node_t base;
  GList* arguments;
  frg_ast_node_t* variadic_positional_arguments;
  frg_ast_node_t* variadic_keyword_arguments;
  frg_ast_node_t* return_type;
} frg_ast_node_type_function_t;

typedef struct {
  frg_ast_node_t base;
  GString* name;
  GList* properties;
} frg_ast_node_declaration_union_t;

typedef struct {
  frg_ast_node_t base;
  GString* name;
  GList* declarations;
} frg_ast_node_declaration_structure_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_declaration_property_flags_t flags;
  GString* name;
  frg_ast_node_t* type;
} frg_ast_node_declaration_property_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_declaration_interface_flags_t flags;
  GString* name;
  GList* extends;
  GList* declarations;
} frg_ast_node_declaration_interface_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_declaration_function_argument_flags_t flags;
  frg_ast_node_declaration_property_t* property;
  frg_ast_node_t* default_value;
} frg_ast_node_declaration_function_argument_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_declaration_function_flags_t flags;
  GString* name;
  frg_ast_node_type_function_t* type;
  frg_ast_node_t* body;
} frg_ast_node_declaration_function_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_declaration_property_t* property;
  frg_ast_node_t* initial_value;
} frg_ast_node_declaration_variable_t;

typedef struct {
  frg_ast_node_t base;
  GList* declarations;
} frg_ast_node_declaration_block_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_t* value;
} frg_ast_node_statement_return_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_t* condition;
  frg_ast_node_t* then_clause;
  frg_ast_node_t* else_clause;
} frg_ast_node_statement_if_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_t* condition;
  frg_ast_node_t* body;
} frg_ast_node_statement_while_t;

typedef struct {
  frg_ast_node_t base;
  GList* statements;
} frg_ast_node_statement_block_t;

typedef struct {
  frg_ast_node_t base;
  bool value;
} frg_ast_node_value_bool_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_type_int_t type;

  union {
    int8_t as_i8;
    int16_t as_i16;
    int32_t as_i32;
    int64_t as_i64;
    uint8_t as_u8;
    uint16_t as_u16;
    uint32_t as_u32;
    uint64_t as_u64;
  } value;
} frg_ast_node_value_int_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_type_float_t type;

  union {
    frg_f32_t as_f32;
    frg_f64_t as_f64;
  } value;
} frg_ast_node_value_float_t;

typedef struct {
  frg_ast_node_t base;
  frg_character_t value;
} frg_ast_node_value_character_t;

typedef struct {
  frg_ast_node_t base;
  GString* value;
} frg_ast_node_value_string_t;

typedef struct {
  frg_ast_node_t base;
  GString* name;
} frg_ast_node_value_symbol_t;

typedef struct {
  frg_ast_node_t base;
  GString* name;
  frg_ast_node_t* value;
} frg_ast_node_value_call_keyword_argument_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_t* callee;
  GList* arguments;
  GList* keyword_arguments;
} frg_ast_node_value_call_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_t* operand;
} frg_ast_node_value_unary_t;

typedef struct {
  frg_ast_node_t base;
  frg_ast_node_t* left;
  frg_ast_node_t* right;
} frg_ast_node_value_binary_t;

frg_ast_node_t* frg_ast_node_type_bool_new(const frg_parsing_range_t* source_range);

frg_ast_node_type_int_t* frg_ast_node_type_int_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_type_int_flags_t flags,
  frg_bit_width_t bit_width);

frg_ast_node_type_float_t* frg_ast_node_type_float_new(
  const frg_parsing_range_t* source_range, frg_bit_width_t bit_width);

frg_ast_node_type_symbol_t* frg_ast_node_type_symbol_new(
  const frg_parsing_range_t* source_range, GString* name);

frg_ast_node_type_pointer_t* frg_ast_node_type_pointer_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_type_pointer_flags_t flags,
  frg_ast_node_t* value);

frg_ast_node_type_function_t* frg_ast_node_type_function_new(
  const frg_parsing_range_t* source_range,
  GList* arguments,
  frg_ast_node_t* variadic_positional_arguments,
  frg_ast_node_t* variadic_keyword_arguments,
  frg_ast_node_t* return_type);

frg_ast_node_declaration_union_t* frg_ast_node_declaration_union_new(
  const frg_parsing_range_t* source_range, GString* name, GList* properties);

frg_ast_node_declaration_structure_t* frg_ast_node_declaration_structure_new(
  const frg_parsing_range_t* source_range, GString* name, GList* declarations);

frg_ast_node_declaration_property_t* frg_ast_node_declaration_property_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_declaration_property_flags_t flags,
  GString* name,
  frg_ast_node_t* type);

frg_ast_node_declaration_interface_t* frg_ast_node_declaration_interface_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_declaration_interface_flags_t flags,
  GString* name,
  GList* extends,
  GList* declarations);

frg_ast_node_declaration_function_argument_t*
  frg_ast_node_declaration_function_argument_new(
    const frg_parsing_range_t* source_range,
    frg_ast_node_declaration_function_argument_flags_t flags,
    frg_ast_node_declaration_property_t* property,
    frg_ast_node_t* default_value);

frg_ast_node_declaration_function_t* frg_ast_node_declaration_function_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_declaration_function_flags_t flags,
  GString* name,
  frg_ast_node_type_function_t* type,
  frg_ast_node_t* body);

frg_ast_node_declaration_variable_t* frg_ast_node_declaration_variable_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_declaration_property_t* property,
  frg_ast_node_t* initial_value);

frg_ast_node_declaration_block_t* frg_ast_node_declaration_block_new(
  const frg_parsing_range_t* source_range, GList* declarations);

frg_ast_node_statement_return_t* frg_ast_node_statement_return_new(
  const frg_parsing_range_t* source_range, frg_ast_node_t* value);

frg_ast_node_statement_if_t* frg_ast_node_statement_if_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_t* condition,
  frg_ast_node_t* then_clause,
  frg_ast_node_t* else_clause);

frg_ast_node_statement_while_t* frg_ast_node_statement_while_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_t* condition,
  frg_ast_node_t* body);

frg_ast_node_statement_block_t* frg_ast_node_statement_block_new(
  const frg_parsing_range_t* source_range, GList* statements);

frg_ast_node_value_bool_t* frg_ast_node_value_bool_new(
  const frg_parsing_range_t* source_range, bool value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_i8(
  const frg_parsing_range_t* source_range, int8_t value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_i16(
  const frg_parsing_range_t* source_range, int16_t value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_i32(
  const frg_parsing_range_t* source_range, int32_t value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_i64(
  const frg_parsing_range_t* source_range, int64_t value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_u8(
  const frg_parsing_range_t* source_range, uint8_t value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_u16(
  const frg_parsing_range_t* source_range, uint16_t value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_u32(
  const frg_parsing_range_t* source_range, uint32_t value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_u64(
  const frg_parsing_range_t* source_range, uint64_t value);

frg_ast_node_value_int_t* frg_ast_node_value_int_new_from_lexical_cast_result(
  const frg_parsing_range_t* source_range, const frg_parse_uint_result_t* result);

frg_ast_node_value_float_t* frg_ast_node_value_float_new_f32(
  const frg_parsing_range_t* source_range, frg_f32_t value);

frg_ast_node_value_float_t* frg_ast_node_value_float_new_f64(
  const frg_parsing_range_t* source_range, frg_f64_t value);

frg_ast_node_value_float_t* frg_ast_node_value_float_new_from_lexical_cast_result(
  const frg_parsing_range_t* source_range, const frg_parse_float_result_t* result);

frg_ast_node_value_character_t* frg_ast_node_value_character_new(
  const frg_parsing_range_t* source_range, frg_character_t value);

frg_ast_node_value_string_t* frg_ast_node_value_string_new(
  const frg_parsing_range_t* source_range, GString* value);

frg_ast_node_value_symbol_t* frg_ast_node_value_symbol_new(
  const frg_parsing_range_t* source_range, GString* name);

frg_ast_node_value_call_keyword_argument_t*
  frg_ast_node_value_call_keyword_argument_new(const frg_parsing_range_t* source_range,
                                               GString* name,
                                               frg_ast_node_t* value);

frg_ast_node_value_call_t* frg_ast_node_value_call_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_t* callee,
  GList* arguments,
  GList* keyword_arguments);

frg_ast_node_value_unary_t* frg_ast_node_value_unary_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_kind_t kind,
  frg_ast_node_t* operand);

frg_ast_node_value_binary_t* frg_ast_node_value_binary_new(
  const frg_parsing_range_t* source_range,
  frg_ast_node_kind_t kind,
  frg_ast_node_t* left,
  frg_ast_node_t* right);

void frg_ast_node_destroy(frg_ast_node_t* node);
