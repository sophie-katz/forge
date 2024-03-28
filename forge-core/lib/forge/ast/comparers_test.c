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

#include <forge/ast/comparers.h>
#include <forge/ast/node.h>
#include <unity.h>

// These tests aren't to try and get full coverage, but to confirm that type comparison
// works as expected until the rest of the functions are implemented.

void setUp() {}

void tearDown() {}

void test_comparer_always_true() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_primary_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_KIND_TYPE_BOOL);
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_primary_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_KIND_TYPE_BOOL);

  TEST_ASSERT_TRUE(frg_ast_comparer_always_true(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_int_matching() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32);
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32);

  TEST_ASSERT_TRUE(frg_ast_comparer_type_int(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_int_mismatched_flags() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32);
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED, 32);

  TEST_ASSERT_FALSE(frg_ast_comparer_type_int(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_int_mismatched_bit_widths() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32);
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 64);

  TEST_ASSERT_FALSE(frg_ast_comparer_type_int(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_float_matching() {
  frg_ast_node_t* a
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);
  frg_ast_node_t* b
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);

  TEST_ASSERT_TRUE(frg_ast_comparer_type_float(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_float_mismatched_bit_widths() {
  frg_ast_node_t* a
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);
  frg_ast_node_t* b
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 64);

  TEST_ASSERT_FALSE(frg_ast_comparer_type_float(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_symbol_matching() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_symbol_new(
    &frg_global_parsing_range_null, g_string_new("x"));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_symbol_new(
    &frg_global_parsing_range_null, g_string_new("x"));

  TEST_ASSERT_TRUE(frg_ast_comparer_type_symbol(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_symbol_mismatched_names() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_symbol_new(
    &frg_global_parsing_range_null, g_string_new("x"));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_symbol_new(
    &frg_global_parsing_range_null, g_string_new("y"));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_symbol(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_pointer_matching() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_pointer_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_TYPE_POINTER_FLAG_NONE,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_pointer_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_TYPE_POINTER_FLAG_NONE,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_TRUE(frg_ast_comparer_type_pointer(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_pointer_mismatched_flags() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_pointer_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_TYPE_POINTER_FLAG_NONE,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_pointer_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_TYPE_POINTER_FLAG_CONSTANT,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_pointer(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_pointer_mismatched_values() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_pointer_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_TYPE_POINTER_FLAG_NONE,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_pointer_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_TYPE_POINTER_FLAG_NONE,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_VOID));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_pointer(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_array_matching() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_array_new(
    &frg_global_parsing_range_null,
    1,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_array_new(
    &frg_global_parsing_range_null,
    1,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_TRUE(frg_ast_comparer_type_array(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_array_mismatched_lengths() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_array_new(
    &frg_global_parsing_range_null,
    1,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_array_new(
    &frg_global_parsing_range_null,
    2,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_array(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_array_mismatched_values() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_array_new(
    &frg_global_parsing_range_null,
    1,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_array_new(
    &frg_global_parsing_range_null,
    1,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_VOID));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_array(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_matching() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_TRUE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_mismatched_return_type() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_VOID));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_matching_with_arguments() {
  GList* a_arguments = NULL;

  a_arguments
    = g_list_append(a_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  a_arguments
    = g_list_append(a_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_VOID));

  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    a_arguments,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  GList* b_arguments = NULL;

  b_arguments
    = g_list_append(b_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  b_arguments
    = g_list_append(b_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_VOID));

  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    b_arguments,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_TRUE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_mismatched_arguments() {
  GList* a_arguments = NULL;

  a_arguments
    = g_list_append(a_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  a_arguments
    = g_list_append(a_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_VOID));

  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    a_arguments,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  GList* b_arguments = NULL;

  b_arguments
    = g_list_append(b_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  b_arguments
    = g_list_append(b_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    b_arguments,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_mismatched_arguments_lengths() {
  GList* a_arguments = NULL;

  a_arguments
    = g_list_append(a_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  a_arguments
    = g_list_append(a_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_VOID));

  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    a_arguments,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  GList* b_arguments = NULL;

  b_arguments
    = g_list_append(b_arguments,
                    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    b_arguments,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_matching_with_variadic_positional_arguments() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL),
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL),
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_TRUE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_mismatched_variadic_positional_arguments() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL),
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_VOID));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_matching_with_variadic_keyword_arguments() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL),
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL),
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_TRUE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_comparer_type_function_mismatched_variadic_keyword_arguments() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_VOID),
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));
  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_function_new(
    &frg_global_parsing_range_null,
    NULL,
    NULL,
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL),
    frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                  FRG_AST_NODE_KIND_TYPE_BOOL));

  TEST_ASSERT_FALSE(frg_ast_comparer_type_function(a, b));

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_comparer_always_true);
  RUN_TEST(test_comparer_type_int_matching);
  RUN_TEST(test_comparer_type_int_mismatched_flags);
  RUN_TEST(test_comparer_type_int_mismatched_bit_widths);
  RUN_TEST(test_comparer_type_float_matching);
  RUN_TEST(test_comparer_type_float_mismatched_bit_widths);
  RUN_TEST(test_comparer_type_symbol_matching);
  RUN_TEST(test_comparer_type_symbol_mismatched_names);
  RUN_TEST(test_comparer_type_pointer_matching);
  RUN_TEST(test_comparer_type_pointer_mismatched_flags);
  RUN_TEST(test_comparer_type_pointer_mismatched_values);
  RUN_TEST(test_comparer_type_array_matching);
  RUN_TEST(test_comparer_type_array_mismatched_lengths);
  RUN_TEST(test_comparer_type_array_mismatched_values);
  RUN_TEST(test_comparer_type_function_matching);
  RUN_TEST(test_comparer_type_function_mismatched_return_type);
  RUN_TEST(test_comparer_type_function_matching_with_arguments);
  RUN_TEST(test_comparer_type_function_mismatched_arguments);
  RUN_TEST(test_comparer_type_function_mismatched_arguments_lengths);
  RUN_TEST(test_comparer_type_function_matching_with_variadic_positional_arguments);
  RUN_TEST(test_comparer_type_function_mismatched_variadic_positional_arguments);
  RUN_TEST(test_comparer_type_function_matching_with_variadic_keyword_arguments);
  RUN_TEST(test_comparer_type_function_mismatched_variadic_keyword_arguments);
  return UNITY_END();
}
