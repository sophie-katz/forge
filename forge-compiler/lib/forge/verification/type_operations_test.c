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

#include <forge/verification/type_operations.h>
#include <unity.h>

void setUp() {}

void tearDown() {}

void test_get_numeric_containing_type_non_numeric() {
  frg_ast_node_t* a = frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                    FRG_AST_NODE_KIND_TYPE_BOOL);

  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_symbol_new(
    &frg_global_parsing_range_null, g_string_new("b"));

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NULL(containing);

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_non_numeric_and_numeric() {
  frg_ast_node_t* a = frg_ast_node_type_primary_new(&frg_global_parsing_range_null,
                                                    FRG_AST_NODE_KIND_TYPE_BOOL);

  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NULL(containing);

  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_i8_i8() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 8);

  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 8);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, containing->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)containing)->flags);
  TEST_ASSERT_EQUAL(8, ((frg_ast_node_type_int_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_i8_i64() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 8);

  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 64);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, containing->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)containing)->flags);
  TEST_ASSERT_EQUAL(64, ((frg_ast_node_type_int_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_i8_u32() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 8);

  frg_ast_node_t* b = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED, 32);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, containing->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)containing)->flags);
  TEST_ASSERT_EQUAL(64, ((frg_ast_node_type_int_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_f32_f32() {
  frg_ast_node_t* a
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);

  frg_ast_node_t* b
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FLOAT, containing->kind);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_float_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_f32_f64() {
  frg_ast_node_t* a
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);

  frg_ast_node_t* b
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 64);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FLOAT, containing->kind);
  TEST_ASSERT_EQUAL(64, ((frg_ast_node_type_float_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_i8_f32() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 8);

  frg_ast_node_t* b
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FLOAT, containing->kind);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_float_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_i32_f32() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32);

  frg_ast_node_t* b
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FLOAT, containing->kind);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_float_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_u32_f32() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED, 32);

  frg_ast_node_t* b
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FLOAT, containing->kind);
  TEST_ASSERT_EQUAL(64, ((frg_ast_node_type_float_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_get_numeric_containing_type_i64_f32() {
  frg_ast_node_t* a = (frg_ast_node_t*)frg_ast_node_type_int_new(
    &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 64);

  frg_ast_node_t* b
    = (frg_ast_node_t*)frg_ast_node_type_float_new(&frg_global_parsing_range_null, 32);

  frg_ast_node_t* containing = frg_verification_get_numeric_containing_type(a, b);

  TEST_ASSERT_NOT_NULL(containing);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FLOAT, containing->kind);
  TEST_ASSERT_EQUAL(64, ((frg_ast_node_type_float_t*)containing)->bit_width);

  frg_ast_node_destroy(containing);
  frg_ast_node_destroy(a);
  frg_ast_node_destroy(b);
}

void test_resolve_type_declaration_property() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_declaration_property_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
    g_string_new("x"),
    (frg_ast_node_t*)frg_ast_node_type_int_new(
      &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32));

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_declaration_function_argument() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node
    = (frg_ast_node_t*)frg_ast_node_declaration_function_argument_new(
      &frg_global_parsing_range_null,
      FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_NONE,
      frg_ast_node_declaration_property_new(
        &frg_global_parsing_range_null,
        FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
        g_string_new("x"),
        (frg_ast_node_t*)frg_ast_node_type_int_new(
          &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32)),
      NULL);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_declaration_function_i32_none() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  GList* arguments                     = NULL;

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_declaration_function_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE,
    g_string_new("f"),
    frg_ast_node_type_function_new(
      &frg_global_parsing_range_null,
      arguments,
      NULL,
      NULL,
      (frg_ast_node_t*)frg_ast_node_type_int_new(
        &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32)),
    NULL);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FUNCTION, resolved->kind);
  TEST_ASSERT_NULL(((frg_ast_node_type_function_t*)resolved)->arguments);
  TEST_ASSERT_NULL(
    ((frg_ast_node_type_function_t*)resolved)->variadic_positional_arguments);
  TEST_ASSERT_NULL(
    ((frg_ast_node_type_function_t*)resolved)->variadic_keyword_arguments);
  TEST_ASSERT_NOT_NULL(((frg_ast_node_type_function_t*)resolved)->return_type);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT,
                    ((frg_ast_node_type_function_t*)resolved)->return_type->kind);
  TEST_ASSERT_EQUAL(
    FRG_AST_NODE_TYPE_INT_FLAG_NONE,
    ((frg_ast_node_type_int_t*)((frg_ast_node_type_function_t*)resolved)->return_type)
      ->flags);
  TEST_ASSERT_EQUAL(
    32,
    ((frg_ast_node_type_int_t*)((frg_ast_node_type_function_t*)resolved)->return_type)
      ->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_declaration_function_i32_i32_i32() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  GList* arguments                     = NULL;

  arguments                            = g_list_append(
    arguments,
    frg_ast_node_declaration_function_argument_new(
      &frg_global_parsing_range_null,
      FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_NONE,
      frg_ast_node_declaration_property_new(
        &frg_global_parsing_range_null,
        FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
        g_string_new("a"),
        (frg_ast_node_t*)frg_ast_node_type_int_new(
          &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32)),
      NULL));

  arguments = g_list_append(
    arguments,
    frg_ast_node_declaration_function_argument_new(
      &frg_global_parsing_range_null,
      FRG_AST_NODE_DECLARATION_FUNCTION_ARGUMENT_FLAG_NONE,
      frg_ast_node_declaration_property_new(
        &frg_global_parsing_range_null,
        FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
        g_string_new("b"),
        (frg_ast_node_t*)frg_ast_node_type_int_new(
          &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32)),
      NULL));

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_declaration_function_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_DECLARATION_FUNCTION_FLAG_NONE,
    g_string_new("f"),
    frg_ast_node_type_function_new(
      &frg_global_parsing_range_null,
      arguments,
      NULL,
      NULL,
      (frg_ast_node_t*)frg_ast_node_type_int_new(
        &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32)),
    NULL);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FUNCTION, resolved->kind);
  TEST_ASSERT_NOT_NULL(((frg_ast_node_type_function_t*)resolved)->arguments);
  TEST_ASSERT_NULL(
    ((frg_ast_node_type_function_t*)resolved)->variadic_positional_arguments);
  TEST_ASSERT_NULL(
    ((frg_ast_node_type_function_t*)resolved)->variadic_keyword_arguments);
  TEST_ASSERT_NOT_NULL(((frg_ast_node_type_function_t*)resolved)->return_type);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT,
                    ((frg_ast_node_type_function_t*)resolved)->return_type->kind);
  TEST_ASSERT_EQUAL(
    FRG_AST_NODE_TYPE_INT_FLAG_NONE,
    ((frg_ast_node_type_int_t*)((frg_ast_node_type_function_t*)resolved)->return_type)
      ->flags);
  TEST_ASSERT_EQUAL(
    32,
    ((frg_ast_node_type_int_t*)((frg_ast_node_type_function_t*)resolved)->return_type)
      ->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_declaration_assignment() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_declaration_assignment_new(
    &frg_global_parsing_range_null,
    frg_ast_node_declaration_property_new(
      &frg_global_parsing_range_null,
      FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
      g_string_new("x"),
      (frg_ast_node_t*)frg_ast_node_type_int_new(
        &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32)),
    NULL);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_bool() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node                 = (frg_ast_node_t*)frg_ast_node_value_bool_new(
    &frg_global_parsing_range_null, true);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_BOOL, resolved->kind);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_i32() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_value_int_new_i32(
    &frg_global_parsing_range_null, 5);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_u16() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_value_int_new_u16(
    &frg_global_parsing_range_null, 5);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(16, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_f32() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_value_float_new_f32(
    &frg_global_parsing_range_null, 5.2);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FLOAT, resolved->kind);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_float_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_f64() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_value_float_new_f64(
    &frg_global_parsing_range_null, 5.2);

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_FLOAT, resolved->kind);
  TEST_ASSERT_EQUAL(64, ((frg_ast_node_type_float_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_symbol_undefined() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node                 = (frg_ast_node_t*)frg_ast_node_value_symbol_new(
    &frg_global_parsing_range_null, g_string_new("x"));

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NULL(resolved);

  TEST_ASSERT_EQUAL(1, message_buffer->message_count);
  TEST_ASSERT_EQUAL(1, message_buffer->error_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
}

void test_resolve_type_value_symbol() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* declaration
    = (frg_ast_node_t*)frg_ast_node_declaration_assignment_new(
      &frg_global_parsing_range_null,
      frg_ast_node_declaration_property_new(
        &frg_global_parsing_range_null,
        FRG_AST_NODE_DECLARATION_PROPERTY_FLAG_NONE,
        g_string_new("x"),
        (frg_ast_node_t*)frg_ast_node_type_int_new(
          &frg_global_parsing_range_null, FRG_AST_NODE_TYPE_INT_FLAG_NONE, 32)),
      NULL);

  frg_ast_scope_add_ast(scope, declaration);

  frg_ast_node_t* node = (frg_ast_node_t*)frg_ast_node_value_symbol_new(
    &frg_global_parsing_range_null, g_string_new("x"));

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(declaration);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_bit_not_i32() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node                 = (frg_ast_node_t*)frg_ast_node_value_unary_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_KIND_VALUE_BIT_NOT,
    (frg_ast_node_t*)frg_ast_node_value_int_new_i32(&frg_global_parsing_range_null, 5));

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_bit_not_u64() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node                 = (frg_ast_node_t*)frg_ast_node_value_unary_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_KIND_VALUE_BIT_NOT,
    (frg_ast_node_t*)frg_ast_node_value_int_new_u64(&frg_global_parsing_range_null, 5));

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(64, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_add_i32_i64() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node                 = (frg_ast_node_t*)frg_ast_node_value_binary_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_KIND_VALUE_ADD,
    (frg_ast_node_t*)frg_ast_node_value_int_new_i32(&frg_global_parsing_range_null, 5),
    (frg_ast_node_t*)frg_ast_node_value_int_new_i64(&frg_global_parsing_range_null, 5));

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(64, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

void test_resolve_type_value_add_assign_i32_i64() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_ast_scope_t* scope               = frg_ast_scope_new();

  frg_ast_node_t* node                 = (frg_ast_node_t*)frg_ast_node_value_binary_new(
    &frg_global_parsing_range_null,
    FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN,
    (frg_ast_node_t*)frg_ast_node_value_int_new_i32(&frg_global_parsing_range_null, 5),
    (frg_ast_node_t*)frg_ast_node_value_int_new_i64(&frg_global_parsing_range_null, 5));

  frg_ast_node_t* resolved = frg_verification_resolve_type(message_buffer, scope, node);

  TEST_ASSERT_NOT_NULL(resolved);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_INT, resolved->kind);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_TYPE_INT_FLAG_NONE,
                    ((frg_ast_node_type_int_t*)resolved)->flags);
  TEST_ASSERT_EQUAL(32, ((frg_ast_node_type_int_t*)resolved)->bit_width);

  TEST_ASSERT_EQUAL(0, message_buffer->message_count);

  frg_message_buffer_destroy(message_buffer);
  frg_ast_scope_destroy(scope);
  frg_ast_node_destroy(node);
  frg_ast_node_destroy(resolved);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_get_numeric_containing_type_non_numeric);
  RUN_TEST(test_get_numeric_containing_type_non_numeric_and_numeric);
  RUN_TEST(test_get_numeric_containing_type_i8_i8);
  RUN_TEST(test_get_numeric_containing_type_i8_i64);
  RUN_TEST(test_get_numeric_containing_type_i8_u32);
  RUN_TEST(test_get_numeric_containing_type_f32_f32);
  RUN_TEST(test_get_numeric_containing_type_f32_f64);
  RUN_TEST(test_get_numeric_containing_type_i8_f32);
  RUN_TEST(test_get_numeric_containing_type_i32_f32);
  RUN_TEST(test_get_numeric_containing_type_u32_f32);
  RUN_TEST(test_get_numeric_containing_type_i64_f32);
  RUN_TEST(test_resolve_type_declaration_property);
  RUN_TEST(test_resolve_type_declaration_function_argument);
  RUN_TEST(test_resolve_type_declaration_function_i32_none);
  RUN_TEST(test_resolve_type_declaration_function_i32_i32_i32);
  RUN_TEST(test_resolve_type_declaration_assignment);
  RUN_TEST(test_resolve_type_value_bool);
  RUN_TEST(test_resolve_type_value_i32);
  RUN_TEST(test_resolve_type_value_u16);
  RUN_TEST(test_resolve_type_value_f32);
  RUN_TEST(test_resolve_type_value_f64);
  RUN_TEST(test_resolve_type_value_symbol_undefined);
  RUN_TEST(test_resolve_type_value_symbol);
  RUN_TEST(test_resolve_type_value_bit_not_i32);
  RUN_TEST(test_resolve_type_value_bit_not_u64);
  RUN_TEST(test_resolve_type_value_add_i32_i64);
  RUN_TEST(test_resolve_type_value_add_assign_i32_i64);
  return UNITY_END();
}
