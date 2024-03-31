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
#include <forge/ast/comparers.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>

bool _frg_ast_comparer_compare_lists(const GList* a, const GList* b) {
  const GList* iter_a = a;
  const GList* iter_b = b;

  while (iter_a != NULL && iter_b != NULL) {
    if (!frg_ast_compare((const frg_ast_node_t*)iter_a->data,
                         (const frg_ast_node_t*)iter_b->data)) {
      return false;
    }

    iter_a = iter_a->next;
    iter_b = iter_b->next;
  }

  return (iter_a == NULL) == (iter_b == NULL);
}

bool frg_ast_comparer_always_true(const frg_ast_node_t* a, const frg_ast_node_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_pointer_non_null(b);
  frg_assert_int_equal_to(a->kind, b->kind);

  return true;
}

bool frg_ast_comparer_type_int(const frg_ast_node_t* a, const frg_ast_node_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_int_equal_to(a->kind, FRG_AST_NODE_KIND_TYPE_INT);
  frg_assert_pointer_non_null(b);
  frg_assert_int_equal_to(b->kind, FRG_AST_NODE_KIND_TYPE_INT);

  frg_ast_node_type_int_t* a_casted = (frg_ast_node_type_int_t*)a;
  frg_ast_node_type_int_t* b_casted = (frg_ast_node_type_int_t*)b;

  if (a_casted->flags != b_casted->flags) {
    return false;
  }

  if (a_casted->bit_width != b_casted->bit_width) {
    return false;
  }

  return true;
}

bool frg_ast_comparer_type_float(const frg_ast_node_t* a, const frg_ast_node_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_int_equal_to(a->kind, FRG_AST_NODE_KIND_TYPE_FLOAT);
  frg_assert_pointer_non_null(b);
  frg_assert_int_equal_to(b->kind, FRG_AST_NODE_KIND_TYPE_FLOAT);

  frg_ast_node_type_float_t* a_casted = (frg_ast_node_type_float_t*)a;
  frg_ast_node_type_float_t* b_casted = (frg_ast_node_type_float_t*)b;

  if (a_casted->bit_width != b_casted->bit_width) {
    return false;
  }

  return true;
}

bool frg_ast_comparer_type_symbol(const frg_ast_node_t* a, const frg_ast_node_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_int_equal_to(a->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);
  frg_assert_pointer_non_null(b);
  frg_assert_int_equal_to(b->kind, FRG_AST_NODE_KIND_TYPE_SYMBOL);

  frg_ast_node_type_symbol_t* a_casted = (frg_ast_node_type_symbol_t*)a;
  frg_ast_node_type_symbol_t* b_casted = (frg_ast_node_type_symbol_t*)b;

  if (strcmp(a_casted->name->str, b_casted->name->str) != 0) {
    return false;
  }

  return true;
}

bool frg_ast_comparer_type_pointer(const frg_ast_node_t* a, const frg_ast_node_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_int_equal_to(a->kind, FRG_AST_NODE_KIND_TYPE_POINTER);
  frg_assert_pointer_non_null(b);
  frg_assert_int_equal_to(b->kind, FRG_AST_NODE_KIND_TYPE_POINTER);

  frg_ast_node_type_pointer_t* a_casted = (frg_ast_node_type_pointer_t*)a;
  frg_ast_node_type_pointer_t* b_casted = (frg_ast_node_type_pointer_t*)b;

  if (a_casted->flags != b_casted->flags) {
    return false;
  }

  if (!frg_ast_compare(a_casted->value, b_casted->value)) {
    return false;
  }

  return true;
}

bool frg_ast_comparer_type_array(const frg_ast_node_t* a, const frg_ast_node_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_int_equal_to(a->kind, FRG_AST_NODE_KIND_TYPE_ARRAY);
  frg_assert_pointer_non_null(b);
  frg_assert_int_equal_to(b->kind, FRG_AST_NODE_KIND_TYPE_ARRAY);

  frg_ast_node_type_array_t* a_casted = (frg_ast_node_type_array_t*)a;
  frg_ast_node_type_array_t* b_casted = (frg_ast_node_type_array_t*)b;

  if (a_casted->length != b_casted->length) {
    return false;
  }

  if (!frg_ast_compare(a_casted->value, b_casted->value)) {
    return false;
  }

  return true;
}

bool frg_ast_comparer_type_function(const frg_ast_node_t* a, const frg_ast_node_t* b) {
  frg_assert_pointer_non_null(a);
  frg_assert_int_equal_to(a->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);
  frg_assert_pointer_non_null(b);
  frg_assert_int_equal_to(b->kind, FRG_AST_NODE_KIND_TYPE_FUNCTION);

  frg_ast_node_type_function_t* a_casted = (frg_ast_node_type_function_t*)a;
  frg_ast_node_type_function_t* b_casted = (frg_ast_node_type_function_t*)b;

  if (!_frg_ast_comparer_compare_lists(a_casted->arguments, b_casted->arguments)) {
    return false;
  }

  if (!frg_ast_compare(a_casted->variadic_positional_arguments,
                       b_casted->variadic_positional_arguments)) {
    return false;
  }

  if (!frg_ast_compare(a_casted->variadic_keyword_arguments,
                       b_casted->variadic_keyword_arguments)) {
    return false;
  }

  if (!frg_ast_compare(a_casted->return_type, b_casted->return_type)) {
    return false;
  }

  return true;
}
