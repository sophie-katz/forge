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

#include <forge-testing/compilation_test/auxiliary/ast.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

int callback_counter = 0;

void callback(const frg_ast_node_t* ast_node, void* mut_user_data) {
  TEST_ASSERT_NULL(mut_user_data);
  TEST_ASSERT_NOT_NULL(ast_node);
  callback_counter++;
}

void test_successful_no_callback() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_bool_new(
    &frg_global_parsing_range_null, true);

  frg_testing_compilation_test_context_parsed_t* context_parsed
    = frg_testing_compilation_test_context_parsed_new(ast_node);

  frg_testing_test_compilation_auxiliary_ast(options, context_parsed);

  frg_testing_compilation_test_context_parsed_destroy(context_parsed);
  frg_testing_compilation_test_options_destroy(options);
}

void test_successful_no_expected_ast() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  callback_counter         = 0;
  options->on_ast          = callback;

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_bool_new(
    &frg_global_parsing_range_null, true);

  frg_testing_compilation_test_context_parsed_t* context_parsed
    = frg_testing_compilation_test_context_parsed_new(ast_node);

  GString* message
    = frg_testing_test_compilation_auxiliary_ast(options, context_parsed);

  TEST_ASSERT_NULL(message);
  TEST_ASSERT_EQUAL(1, callback_counter);

  frg_testing_compilation_test_context_parsed_destroy(context_parsed);
  frg_testing_compilation_test_options_destroy(options);
}

void test_successful_expected_ast() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  callback_counter         = 0;
  options->on_ast          = callback;

  options->ast_debug       = "[value-bool]\n  value = true";

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_bool_new(
    &frg_global_parsing_range_null, true);

  frg_testing_compilation_test_context_parsed_t* context_parsed
    = frg_testing_compilation_test_context_parsed_new(ast_node);

  GString* message
    = frg_testing_test_compilation_auxiliary_ast(options, context_parsed);

  TEST_ASSERT_NULL(message);
  TEST_ASSERT_EQUAL(1, callback_counter);

  frg_testing_compilation_test_context_parsed_destroy(context_parsed);
  frg_testing_compilation_test_options_destroy(options);
}

void test_unsuccessful_not_matching() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  callback_counter         = 0;
  options->on_ast          = callback;

  options->ast_debug       = "[value-bool]\n  value = false";

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_bool_new(
    &frg_global_parsing_range_null, true);

  frg_testing_compilation_test_context_parsed_t* context_parsed
    = frg_testing_compilation_test_context_parsed_new(ast_node);

  GString* message
    = frg_testing_test_compilation_auxiliary_ast(options, context_parsed);

  TEST_ASSERT_EQUAL_STRING("Parsed AST does not match expected", message->str);
  TEST_ASSERT_EQUAL(0, callback_counter);

  g_string_free(message, TRUE);
  frg_testing_compilation_test_context_parsed_destroy(context_parsed);
  frg_testing_compilation_test_options_destroy(options);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_successful_no_callback);
  RUN_TEST(test_successful_no_expected_ast);
  RUN_TEST(test_successful_expected_ast);
  RUN_TEST(test_unsuccessful_not_matching);
  return UNITY_END();
}
