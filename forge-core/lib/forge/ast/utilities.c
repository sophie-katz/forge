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

#include <forge/assert.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>
#include <forge/debug.h>

frg_ast_node_t* frg_ast_clone(const frg_ast_node_t* node) {
  if (node == NULL) {
    return NULL;
  }

  return frg_ast_node_kind_info_get(node->kind)->_cloner(node);
}

bool frg_ast_compare(const frg_ast_node_t* a, const frg_ast_node_t* b) {
  if (a == NULL) {
    if (b == NULL) {
      return true;
    } else {
      return false;
    }
  } else {
    if (b == NULL) {
      return false;
    } else {
      if (a->kind != b->kind) {
        return false;
      }

      frg_ast_comparer_t comparer = frg_ast_node_kind_info_get(a->kind)->_comparer;

      frg_assert_pointer_non_null(comparer);

      return comparer(a, b);
    }
  }
}

const char* frg_ast_declaration_name_get(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert(frg_ast_node_kind_info_get(node->kind)->flags
             & FRG_AST_NODE_KIND_FLAG_DECLARATION);

  frg_ast_declaration_name_getter_t declaration_name_getter
    = frg_ast_node_kind_info_get(node->kind)->_declaration_name_getter;

  frg_assert_pointer_non_null(declaration_name_getter);

  return declaration_name_getter(node);
}

const char* frg_ast_declaration_function_argument_name_get(const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(node);
  frg_assert_int_equal_to(node->kind, FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT);

  return ((const frg_ast_node_declaration_property_t*)(((const frg_ast_node_declaration_function_argument_t*)
                                                          node)
                                                         ->property))
    ->name->str;
}

void frg_ast_type_int_print(frg_stream_output_t* mut_stream,
                            const frg_ast_node_type_int_t* node) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_is_valid_bit_width_int(node->bit_width));

  frg_print_uint_suffix(mut_stream,
                        (node->flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED) == 0,
                        node->bit_width);
}

void frg_ast_value_int_print(frg_stream_output_t* mut_stream,
                             const frg_ast_node_value_int_t* node,
                             frg_base_t base,
                             frg_column_number_t separate_every_n) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_is_valid_base(base));
  frg_assert_int_non_negative(separate_every_n);

  if (node->type.flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED) {
    frg_ast_value_int_print_unsigned(mut_stream, node, base, separate_every_n);
  } else {
    frg_ast_value_int_print_signed(mut_stream, node, base, separate_every_n);
  }
}

void frg_ast_value_int_print_unsigned(frg_stream_output_t* mut_stream,
                                      const frg_ast_node_value_int_t* node,
                                      frg_base_t base,
                                      frg_column_number_t separate_every_n) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_is_valid_base(base));
  frg_assert_int_non_negative(separate_every_n);

  switch (node->type.bit_width) {
  case 8:
    frg_print_uint(mut_stream, node->value.as_u8, base, separate_every_n);
    break;
  case 16:
    frg_print_uint(mut_stream, node->value.as_u16, base, separate_every_n);
    break;
  case 32:
    frg_print_uint(mut_stream, node->value.as_u32, base, separate_every_n);
    break;
  case 64:
    frg_print_uint(mut_stream, node->value.as_u64, base, separate_every_n);
    break;
  default:
    frg_die("Unsupported bit width: %u", node->type.bit_width);
  }
}

void frg_ast_value_int_print_signed(frg_stream_output_t* mut_stream,
                                    const frg_ast_node_value_int_t* node,
                                    frg_base_t base,
                                    frg_column_number_t separate_every_n) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_is_valid_base(base));
  frg_assert_int_non_negative(separate_every_n);

  switch (node->type.bit_width) {
  case 8:
    if (node->value.as_i8 < 0) {
      frg_stream_output_write_character(mut_stream, '-');
      frg_print_uint(mut_stream, -node->value.as_i8, base, separate_every_n);
    } else {
      frg_print_uint(mut_stream, node->value.as_i8, base, separate_every_n);
    }
    break;
  case 16:
    if (node->value.as_i16 < 0) {
      frg_stream_output_write_character(mut_stream, '-');
      frg_print_uint(mut_stream, -node->value.as_i16, base, separate_every_n);
    } else {
      frg_print_uint(mut_stream, node->value.as_i16, base, separate_every_n);
    }
    break;
  case 32:
    if (node->value.as_i32 < 0) {
      frg_stream_output_write_character(mut_stream, '-');
      frg_print_uint(mut_stream, -node->value.as_i32, base, separate_every_n);
    } else {
      frg_print_uint(mut_stream, node->value.as_i32, base, separate_every_n);
    }
    break;
  case 64:
    if (node->value.as_i64 < 0) {
      frg_stream_output_write_character(mut_stream, '-');
      frg_print_uint(mut_stream, -node->value.as_i64, base, separate_every_n);
    } else {
      frg_print_uint(mut_stream, node->value.as_i64, base, separate_every_n);
    }
    break;
  default:
    frg_die("Unsupported bit width: %u", node->type.bit_width);
  }
}

void frg_ast_type_float_print(frg_stream_output_t* mut_stream,
                              const frg_ast_node_type_float_t* node) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_is_valid_bit_width_float(node->bit_width));

  frg_print_float_suffix(mut_stream, node->bit_width);
}

void frg_ast_value_float_print(frg_stream_output_t* mut_stream,
                               const frg_ast_node_value_float_t* node,
                               frg_base_t base,
                               frg_column_number_t separate_every_n) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(node);
  frg_assert(frg_is_valid_base(base));
  frg_assert_int_non_negative(separate_every_n);

  switch (node->type.bit_width) {
  case 32:
    frg_print_float(mut_stream, node->value.as_f32, base, separate_every_n, false);
    break;
  case 64:
    frg_print_float(mut_stream, node->value.as_f64, base, separate_every_n, false);
    break;
  default:
    frg_die("Unsupported bit width: %u", node->type.bit_width);
  }
}

frg_int_attributes_case_t frg_get_case_for_type_int(
  const frg_ast_node_type_int_t* type) {
  frg_assert_pointer_non_null(type);

  return frg_get_case_for_int_attributes(
    (type->flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED) == 0, type->bit_width);
}

const frg_ast_node_t* frg_ast_get_surrounding_node_with_kind(const GList* parents,
                                                             frg_ast_node_kind_t kind) {
  for (const GList* it = parents; it != NULL; it = it->next) {
    const frg_ast_node_t* parent = it->data;
    if (parent->kind == kind) {
      return parent;
    }
  }

  return NULL;
}
