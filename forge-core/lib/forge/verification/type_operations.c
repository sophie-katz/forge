// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#include <forge/assert.h>
#include <forge/ast/node_kind_info.h>
#include <forge/verification/type_operations.h>

frg_ast_node_type_int_flags_t
  _frg_verification_get_numeric_containing_type_combine_int_flags(
    frg_ast_node_type_int_flags_t a, frg_ast_node_type_int_flags_t b) {
  frg_assert_int_equal_to(a, b);
  return a;
}

frg_ast_node_t* _frg_verification_get_numeric_containing_type_ints_same_sign(
  const frg_ast_node_type_int_t* a, const frg_ast_node_type_int_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_pointer_non_null(b);
  frg_assert(frg_is_valid_bit_width_int(a->bit_width));
  frg_assert(frg_is_valid_bit_width_int(b->bit_width));

  return (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null,
    _frg_verification_get_numeric_containing_type_combine_int_flags(a->flags, b->flags),
    MAX(a->bit_width, b->bit_width));
}

frg_ast_node_t* _frg_verification_get_numeric_containing_type_different_signs(
  const frg_ast_node_type_int_t* a, const frg_ast_node_type_int_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_pointer_non_null(b);
  frg_assert(frg_is_valid_bit_width_int(a->bit_width));
  frg_assert(frg_is_valid_bit_width_int(b->bit_width));

  return (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_TYPE_INT_FLAG_NONE,
    MIN(64, MAX(a->bit_width, b->bit_width) * 2));
}

frg_ast_node_t* _frg_verification_get_numeric_containing_type_ints(
  const frg_ast_node_type_int_t* a, const frg_ast_node_type_int_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_pointer_non_null(b);
  frg_assert(frg_is_valid_bit_width_int(a->bit_width));
  frg_assert(frg_is_valid_bit_width_int(b->bit_width));

  if ((a->flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED)
      == (b->flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED)) {
    return _frg_verification_get_numeric_containing_type_ints_same_sign(a, b);
  } else {
    return _frg_verification_get_numeric_containing_type_different_signs(a, b);
  }
}

frg_ast_node_t* _frg_verification_get_numeric_containing_type_floats(
  const frg_ast_node_type_float_t* a, const frg_ast_node_type_float_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_pointer_non_null(b);
  frg_assert(frg_is_valid_bit_width_float(a->bit_width));
  frg_assert(frg_is_valid_bit_width_float(b->bit_width));

  return (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null,
                                                      MAX(a->bit_width, b->bit_width));
}

frg_ast_node_t* _frg_verification_get_numeric_containing_type_int_and_float(
  const frg_ast_node_type_int_t* a, const frg_ast_node_type_float_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_pointer_non_null(b);
  frg_assert(frg_is_valid_bit_width_int(a->bit_width));
  frg_assert(frg_is_valid_bit_width_float(b->bit_width));

  if (b->bit_width == 64) {
    return (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null,
                                                        64);
  } else {
    if (a->bit_width < 32) {
      return (frg_ast_node_t*)frg_ast_node_type_float_new(
        &frg_global_parsing_range_null, 32);
    } else if (a->bit_width == 32) {
      if (a->flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED) {
        return (frg_ast_node_t*)frg_ast_node_type_float_new(
          &frg_global_parsing_range_null, 64);
      } else {
        return (frg_ast_node_t*)frg_ast_node_type_float_new(
          &frg_global_parsing_range_null, 32);
      }
    } else {
      return (frg_ast_node_t*)frg_ast_node_type_float_new(
        &frg_global_parsing_range_null, 64);
    }
  }
}

frg_ast_node_t* frg_verification_get_numeric_containing_type(const frg_ast_node_t* a,
                                                             const frg_ast_node_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_pointer_non_null(b);

  if (a->kind == FRG_AST_NODE_KIND_TYPE_INT) {
    if (b->kind == FRG_AST_NODE_KIND_TYPE_INT) {
      return _frg_verification_get_numeric_containing_type_ints(
        (const frg_ast_node_type_int_t*)a, (const frg_ast_node_type_int_t*)b);
    } else if (b->kind == FRG_AST_NODE_KIND_TYPE_FLOAT) {
      return _frg_verification_get_numeric_containing_type_int_and_float(
        (const frg_ast_node_type_int_t*)a, (const frg_ast_node_type_float_t*)b);
    } else {
      return NULL;
    }
  } else if (a->kind == FRG_AST_NODE_KIND_TYPE_FLOAT) {
    if (b->kind == FRG_AST_NODE_KIND_TYPE_INT) {
      return _frg_verification_get_numeric_containing_type_int_and_float(
        (const frg_ast_node_type_int_t*)b, (const frg_ast_node_type_float_t*)a);
    } else if (b->kind == FRG_AST_NODE_KIND_TYPE_FLOAT) {
      return _frg_verification_get_numeric_containing_type_floats(
        (const frg_ast_node_type_float_t*)a, (const frg_ast_node_type_float_t*)b);
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

frg_ast_node_t* frg_verification_resolve_type(frg_message_buffer_t* mut_message_buffer,
                                              const frg_ast_scope_t* scope,
                                              const frg_ast_node_t* node) {
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(scope);
  frg_assert_pointer_non_null(node);

  frg_ast_type_resolver_t type_resolver
    = frg_ast_node_kind_info_get(node->kind)->_type_resolver;

  frg_assert_pointer_non_null(type_resolver);

  return type_resolver(mut_message_buffer, scope, node);
}
