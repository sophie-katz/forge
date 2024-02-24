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

#include <forge/ast/node.h>
#include <forge/assert.h>
#include <forge/memory.h>
#include <forge/debug.h>
#include <stddef.h>
#include <stdlib.h>
#include <forge/ast/node_kind_info.h>

frg_ast_node_t* frg_ast_node_type_bool_new(
    const frg_parsing_range_t *source_range
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_t* node = frg_malloc(sizeof(frg_ast_node_t));
    node->source_range = *source_range;
    node->kind = FRG_AST_NODE_KIND_TYPE_BOOL;

    return node;
}

frg_ast_node_type_int_t* frg_ast_node_type_int_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_type_int_flags_t flags,
    frg_bit_width_t bit_width
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert(frg_is_valid_bit_width_int(bit_width));

    frg_ast_node_type_int_t* node = frg_malloc(sizeof(frg_ast_node_type_int_t));
    node->base.source_range = *source_range;
    node->base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->flags = flags;
    node->bit_width = bit_width;

    return node;
}

frg_ast_node_type_float_t* frg_ast_node_type_float_new(
    const frg_parsing_range_t *source_range,
    frg_bit_width_t bit_width
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert(frg_is_valid_bit_width_float(bit_width));

    frg_ast_node_type_float_t* node = frg_malloc(sizeof(frg_ast_node_type_float_t));
    node->base.source_range = *source_range;
    node->base.kind = FRG_AST_NODE_KIND_TYPE_FLOAT;
    node->bit_width = bit_width;

    return node;
}

frg_ast_node_type_symbol_t* frg_ast_node_type_symbol_new(
    const frg_parsing_range_t *source_range,
    GString* name
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_gstring_non_empty(name);

    frg_ast_node_type_symbol_t* node = frg_malloc(sizeof(frg_ast_node_type_symbol_t));

    node->base.kind = FRG_AST_NODE_KIND_TYPE_SYMBOL;
    node->base.source_range = *source_range;
    node->name = name;

    return node;
}

frg_ast_node_type_pointer_t* frg_ast_node_type_pointer_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_t* value
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(value);

    frg_ast_node_type_pointer_t* node = frg_malloc(sizeof(frg_ast_node_type_pointer_t));

    node->base.kind = FRG_AST_NODE_KIND_TYPE_POINTER;
    node->base.source_range = *source_range;
    node->value = value;

    return node;
}

frg_ast_node_type_function_t* frg_ast_node_type_function_new(
    const frg_parsing_range_t *source_range,
    GList* arguments,
    frg_ast_node_t* variadic_positional_arguments,
    frg_ast_node_t* variadic_keyword_arguments,
    frg_ast_node_t* return_type
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_type_function_t* node = frg_malloc(sizeof(frg_ast_node_type_function_t));

    node->base.kind = FRG_AST_NODE_KIND_TYPE_FUNCTION;
    node->base.source_range = *source_range;
    node->arguments = arguments;
    node->variadic_positional_arguments = variadic_positional_arguments;
    node->variadic_keyword_arguments = variadic_keyword_arguments;
    node->return_type = return_type;

    return node;
}

frg_ast_node_declaration_union_t* frg_ast_node_declaration_union_new(
    const frg_parsing_range_t *source_range,
    GString* name,
    GList* properties
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(properties);

    frg_ast_node_declaration_union_t* node = frg_malloc(sizeof(frg_ast_node_declaration_union_t));

    node->base.kind = FRG_AST_NODE_KIND_DECLARATION_UNION;
    node->base.source_range = *source_range;
    node->name = name;
    node->properties = properties;

    return node;
}

frg_ast_node_declaration_structure_t* frg_ast_node_declaration_structure_new(
    const frg_parsing_range_t *source_range,
    GString* name,
    GList* declarations
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_gstring_non_empty(name);

    frg_ast_node_declaration_structure_t* node = frg_malloc(sizeof(frg_ast_node_declaration_structure_t));

    node->base.kind = FRG_AST_NODE_KIND_DECLARATION_STRUCTURE;
    node->base.source_range = *source_range;
    node->name = name;
    node->declarations = declarations;

    return node;
}

frg_ast_node_declaration_property_t* frg_ast_node_declaration_property_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_declaration_property_flags_t flags,
    GString* name,
    frg_ast_node_t* type
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_gstring_non_empty(name);

    frg_ast_node_declaration_property_t* node = frg_malloc(sizeof(frg_ast_node_declaration_property_t));

    node->base.kind = FRG_AST_NODE_KIND_DECLARATION_PROPERTY;
    node->base.source_range = *source_range;
    node->flags = flags;
    node->name = name;
    node->type = type;

    return node;
}

frg_ast_node_declaration_interface_t* frg_ast_node_declaration_interface_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_declaration_interface_flags_t flags,
    GString* name,
    GList* extends,
    GList* declarations
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_gstring_non_empty(name);

    frg_ast_node_declaration_interface_t* node = frg_malloc(sizeof(frg_ast_node_declaration_interface_t));

    node->base.kind = FRG_AST_NODE_KIND_DECLARATION_INTERFACE;
    node->base.source_range = *source_range;
    node->flags = flags;
    node->name = name;
    node->extends = extends;
    node->declarations = declarations;

    return node;
}

frg_ast_node_declaration_function_argument_t* frg_ast_node_declaration_function_argument_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_declaration_function_argument_flags_t flags,
    frg_ast_node_declaration_property_t* property,
    frg_ast_node_t* default_value
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(property);

    frg_ast_node_declaration_function_argument_t* node = frg_malloc(sizeof(frg_ast_node_declaration_function_argument_t));

    node->base.kind = FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT;
    node->base.source_range = *source_range;
    node->flags = flags;
    node->property = property;
    node->default_value = default_value;

    return node;
}

frg_ast_node_declaration_function_t* frg_ast_node_declaration_function_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_declaration_function_flags_t flags,
    GString* name,
    frg_ast_node_type_function_t* type,
    frg_ast_node_t* body
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(type);

    frg_ast_node_declaration_function_t* node = frg_malloc(sizeof(frg_ast_node_declaration_function_t));

    node->base.kind = FRG_AST_NODE_KIND_DECLARATION_FUNCTION;
    node->base.source_range = *source_range;
    node->flags = flags;
    node->name = name;
    node->type = type;
    node->body = body;

    return node;
}

frg_ast_node_declaration_variable_t* frg_ast_node_declaration_variable_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_declaration_property_t* property,
    frg_ast_node_t* initial_value
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(property);

    frg_ast_node_declaration_variable_t* node = frg_malloc(sizeof(frg_ast_node_declaration_variable_t));

    node->base.kind = FRG_AST_NODE_KIND_DECLARATION_VARIABLE;
    node->base.source_range = *source_range;
    node->property = property;
    node->initial_value = initial_value;

    return node;
}

frg_ast_node_declaration_block_t* frg_ast_node_declaration_block_new(
    const frg_parsing_range_t *source_range,
    GList* declarations
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_declaration_block_t* node = frg_malloc(sizeof(frg_ast_node_declaration_block_t));

    node->base.kind = FRG_AST_NODE_KIND_DECLARATION_BLOCK;
    node->base.source_range = *source_range;
    node->declarations = declarations;

    return node;
}

frg_ast_node_statement_return_t* frg_ast_node_statement_return_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_t* value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_statement_return_t* node = frg_malloc(sizeof(frg_ast_node_statement_return_t));

    node->base.kind = FRG_AST_NODE_KIND_STATEMENT_RETURN;
    node->base.source_range = *source_range;
    node->value = value;

    return node;
}

frg_ast_node_statement_if_t* frg_ast_node_statement_if_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_t* condition,
    frg_ast_node_t* then_clause,
    frg_ast_node_t* else_clause
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(condition);
    frg_assert_pointer_non_null(then_clause);

    frg_ast_node_statement_if_t* node = frg_malloc(sizeof(frg_ast_node_statement_if_t));

    node->base.kind = FRG_AST_NODE_KIND_STATEMENT_IF;
    node->base.source_range = *source_range;
    node->condition = condition;
    node->then_clause = then_clause;
    node->else_clause = else_clause;

    return node;
}

frg_ast_node_statement_while_t* frg_ast_node_statement_while_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_t* condition,
    frg_ast_node_t* body
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(condition);

    frg_ast_node_statement_while_t* node = frg_malloc(sizeof(frg_ast_node_statement_while_t));

    node->base.kind = FRG_AST_NODE_KIND_STATEMENT_WHILE;
    node->base.source_range = *source_range;
    node->condition = condition;
    node->body = body;

    return node;
}

frg_ast_node_statement_block_t* frg_ast_node_statement_block_new(
    const frg_parsing_range_t *source_range,
    GList* statements
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_statement_block_t* node = frg_malloc(sizeof(frg_ast_node_statement_block_t));

    node->base.kind = FRG_AST_NODE_KIND_STATEMENT_BLOCK;
    node->base.source_range = *source_range;
    node->statements = statements;

    return node;
}

frg_ast_node_value_bool_t* frg_ast_node_value_bool_new(
    const frg_parsing_range_t *source_range,
    bool value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_bool_t* node = frg_malloc(sizeof(frg_ast_node_value_bool_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_BOOL;
    node->base.source_range = *source_range;
    node->value = value;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_i8(
    const frg_parsing_range_t *source_range,
    int8_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_int_t* node = frg_malloc(sizeof(frg_ast_node_value_int_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_INT;
    node->base.source_range = *source_range;
    node->value.as_i8 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 8;
    node->type.flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_i16(
    const frg_parsing_range_t *source_range,
    int16_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_int_t* node = frg_malloc(sizeof(frg_ast_node_value_int_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_INT;
    node->base.source_range = *source_range;
    node->value.as_i16 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 16;
    node->type.flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_i32(
    const frg_parsing_range_t *source_range,
    int32_t value
) {
    frg_assert_pointer_non_null(source_range);
    
    frg_ast_node_value_int_t* node = frg_malloc(sizeof(frg_ast_node_value_int_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_INT;
    node->base.source_range = *source_range;
    node->value.as_i32 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 32;
    node->type.flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_i64(
    const frg_parsing_range_t *source_range,
    int64_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_int_t* node = frg_malloc(sizeof(frg_ast_node_value_int_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_INT;
    node->base.source_range = *source_range;
    node->value.as_i64 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 64;
    node->type.flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_u8(
    const frg_parsing_range_t *source_range,
    uint8_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_int_t* node = frg_malloc(sizeof(frg_ast_node_value_int_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_INT;
    node->base.source_range = *source_range;
    node->value.as_u8 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 8;
    node->type.flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_u16(
    const frg_parsing_range_t *source_range,
    uint16_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_int_t* node = frg_malloc(sizeof(frg_ast_node_value_int_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_INT;
    node->base.source_range = *source_range;
    node->value.as_u16 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 16;
    node->type.flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_u32(
    const frg_parsing_range_t *source_range,
    uint32_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_int_t* node = frg_malloc(sizeof(frg_ast_node_value_int_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_INT;
    node->base.source_range = *source_range;
    node->value.as_u32 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 32;
    node->type.flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_u64(
    const frg_parsing_range_t *source_range,
    uint64_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_int_t* node = frg_malloc(sizeof(frg_ast_node_value_int_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_INT;
    node->base.source_range = *source_range;
    node->value.as_u64 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_INT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 64;
    node->type.flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED;

    return node;
}

frg_ast_node_value_int_t* frg_ast_node_value_int_new_from_lexical_cast_result(
    const frg_parsing_range_t *source_range,
    const frg_parse_uint_result_t* result
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(result);

    if (result->bit_width == 8) {
        if (result->is_signed) {
            return frg_ast_node_value_int_new_i8(source_range, (int8_t)result->value);
        } else {
            return frg_ast_node_value_int_new_u8(source_range, (uint8_t)result->value);
        }
    } else if (result->bit_width == 16) {
        if (result->is_signed) {
            return frg_ast_node_value_int_new_i16(source_range, (int16_t)result->value);
        } else {
            return frg_ast_node_value_int_new_u16(source_range, (uint16_t)result->value);
        }
    } else if (result->bit_width == 32) {
        if (result->is_signed) {
            return frg_ast_node_value_int_new_i32(source_range, (int32_t)result->value);
        } else {
            return frg_ast_node_value_int_new_u32(source_range, (uint32_t)result->value);
        }
    } else if (result->bit_width == 64) {
        if (result->is_signed) {
            return frg_ast_node_value_int_new_i64(source_range, (int64_t)result->value);
        } else {
            return frg_ast_node_value_int_new_u64(source_range, (uint64_t)result->value);
        }
    } else {
        frg_die("unexpected bit width %i", result->bit_width);
    }
}

frg_ast_node_value_float_t* frg_ast_node_value_float_new_f32(
    const frg_parsing_range_t *source_range,
    frg_f32_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_float_t* node = frg_malloc(sizeof(frg_ast_node_value_float_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_FLOAT;
    node->base.source_range = *source_range;
    node->value.as_f32 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_FLOAT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 32;

    return node;
}

frg_ast_node_value_float_t* frg_ast_node_value_float_new_f64(
    const frg_parsing_range_t *source_range,
    frg_f64_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_float_t* node = frg_malloc(sizeof(frg_ast_node_value_float_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_FLOAT;
    node->base.source_range = *source_range;
    node->value.as_f64 = value;
    node->type.base.kind = FRG_AST_NODE_KIND_TYPE_FLOAT;
    node->type.base.source_range = frg_global_parsing_range_null;
    node->type.bit_width = 64;

    return node;
}

frg_ast_node_value_float_t* frg_ast_node_value_float_new_from_lexical_cast_result(
    const frg_parsing_range_t *source_range,
    const frg_parse_float_result_t* result
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(result);

    if (result->bit_width == 32) {
        return frg_ast_node_value_float_new_f32(source_range, (frg_f32_t)result->value);
    } else if (result->bit_width == 64) {
        return frg_ast_node_value_float_new_f64(source_range, (frg_f64_t)result->value);
    } else {
        frg_die("unexpected bit width %i", result->bit_width);
    }
}

frg_ast_node_value_character_t* frg_ast_node_value_character_new(
    const frg_parsing_range_t *source_range,
    frg_character_t value
) {
    frg_assert_pointer_non_null(source_range);

    frg_ast_node_value_character_t* node = frg_malloc(sizeof(frg_ast_node_value_character_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_CHARACTER;
    node->base.source_range = *source_range;
    node->value = value;

    return node;
}

frg_ast_node_value_string_t* frg_ast_node_value_string_new(
    const frg_parsing_range_t *source_range,
    GString* value
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(value);

    frg_ast_node_value_string_t* node = frg_malloc(sizeof(frg_ast_node_value_string_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_STRING;
    node->base.source_range = *source_range;
    node->value = value;

    return node;
}

frg_ast_node_value_symbol_t* frg_ast_node_value_symbol_new(
    const frg_parsing_range_t *source_range,
    GString* name
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_gstring_non_empty(name);

    frg_ast_node_value_symbol_t* node = frg_malloc(sizeof(frg_ast_node_value_symbol_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_SYMBOL;
    node->base.source_range = *source_range;
    node->name = name;

    return node;
}

frg_ast_node_value_call_keyword_argument_t* frg_ast_node_value_call_keyword_argument_new(
    const frg_parsing_range_t *source_range,
    GString* name,
    frg_ast_node_t* value
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(value);

    frg_ast_node_value_call_keyword_argument_t* node = frg_malloc(sizeof(frg_ast_node_value_call_keyword_argument_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT;
    node->base.source_range = *source_range;
    node->name = name;
    node->value = value;

    return node;
}

frg_ast_node_value_call_t* frg_ast_node_value_call_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_t* callee,
    GList* arguments,
    GList* keyword_arguments
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert_pointer_non_null(callee);

    frg_ast_node_value_call_t* node = frg_malloc(sizeof(frg_ast_node_value_call_t));

    node->base.kind = FRG_AST_NODE_KIND_VALUE_CALL;
    node->base.source_range = *source_range;
    node->callee = callee;
    node->arguments = arguments;
    node->keyword_arguments = keyword_arguments;

    return node;
}

frg_ast_node_value_unary_t* frg_ast_node_value_unary_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_kind_t kind,
    frg_ast_node_t* operand
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert(frg_ast_node_kind_info_get(kind)->flags & FRG_AST_NODE_KIND_FLAG_VALUE_UNARY);
    frg_assert_pointer_non_null(operand);

    frg_ast_node_value_unary_t* node = frg_malloc(sizeof(frg_ast_node_value_unary_t));

    node->base.kind = kind;
    node->base.source_range = *source_range;
    node->operand = operand;

    return node;
}

frg_ast_node_value_binary_t* frg_ast_node_value_binary_new(
    const frg_parsing_range_t *source_range,
    frg_ast_node_kind_t kind,
    frg_ast_node_t* left,
    frg_ast_node_t* right
) {
    frg_assert_pointer_non_null(source_range);
    frg_assert(frg_ast_node_kind_info_get(kind)->flags & FRG_AST_NODE_KIND_FLAG_VALUE_BINARY);
    frg_assert_pointer_non_null(left);
    frg_assert_pointer_non_null(right);

    frg_ast_node_value_binary_t* node = frg_malloc(sizeof(frg_ast_node_value_binary_t));

    node->base.kind = kind;
    node->base.source_range = *source_range;
    node->left = left;
    node->right = right;

    return node;
}

void frg_ast_node_destroy(frg_ast_node_t* node) {
    frg_assert_pointer_non_null(node);

    frg_ast_destructor_t destructor = frg_ast_node_kind_info_get(node->kind)->_destructor;
    
    if (destructor != NULL) {
        destructor(node);
    }

    frg_free(node);
}
