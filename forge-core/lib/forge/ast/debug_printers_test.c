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

#include <forge/ast/debug_printers.h>
#include <unity.h>

// These tests aren't to try and get full coverage, but to confirm that a few
// representative examples work as expected so other functions can have something to
// safely model after.

void setUp() {}

void tearDown() {}

// From 0 spaces
// -------------------------------------------------------------------------------------

// A node that has a non-node property
void test_value_bool_from_0() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_bool_new(
    &frg_global_parsing_range_null, true);

  frg_ast_print_debug_options_t options = frg_ast_print_debug_options_default;
  options.max_depth                     = 2;

  frg_ast_debug_printer_value_bool(stream, ast_node, &options);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("\n  value = true", buffer->str);

  g_string_free(buffer, TRUE);
  frg_ast_node_destroy(ast_node);
}

// A node that has a node property
void test_value_array_repeated_from_0() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_array_repeated_new(
    &frg_global_parsing_range_null,
    5,
    (frg_ast_node_t*)frg_ast_node_value_bool_new(&frg_global_parsing_range_null,
                                                 false));

  frg_ast_print_debug_options_t options = frg_ast_print_debug_options_default;
  options.max_depth                     = 3;

  frg_ast_debug_printer_value_array_repeated(stream, ast_node, &options);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  // clang-format off
  TEST_ASSERT_EQUAL_STRING(
    "\n"
    "  length = 5\n"
    "  element = [value-bool]\n"
    "    value = false",
    buffer->str
  );
  // clang-format on

  g_string_free(buffer, TRUE);
  frg_ast_node_destroy(ast_node);
}

// A node that has a node list property
void test_value_array_from_0() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  GList* elements = NULL;

  elements        = g_list_append(
    elements, frg_ast_node_value_bool_new(&frg_global_parsing_range_null, false));

  elements = g_list_append(
    elements, frg_ast_node_value_bool_new(&frg_global_parsing_range_null, true));

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_array_new(
    &frg_global_parsing_range_null, elements);

  frg_ast_print_debug_options_t options = frg_ast_print_debug_options_default;
  options.max_list_length               = 3;
  options.max_depth                     = 3;

  frg_ast_debug_printer_value_array(stream, ast_node, &options);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  // clang-format off
  TEST_ASSERT_EQUAL_STRING(
    "\n"
    "  elements[0] = [value-bool]\n"
    "    value = false\n"
    "  elements[1] = [value-bool]\n"
    "    value = true",
    buffer->str
  );
  // clang-format on

  g_string_free(buffer, TRUE);
  frg_ast_node_destroy(ast_node);
}

// From 2 spaces
// -------------------------------------------------------------------------------------

// A node that has a non-node property
void test_value_bool_from_2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_bool_new(
    &frg_global_parsing_range_null, true);

  frg_ast_print_debug_options_t options = frg_ast_print_debug_options_default;
  options.max_depth                     = 2;
  options.indentation                   = 2;

  frg_ast_debug_printer_value_bool(stream, ast_node, &options);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  TEST_ASSERT_EQUAL_STRING("\n    value = true", buffer->str);

  g_string_free(buffer, TRUE);
  frg_ast_node_destroy(ast_node);
}

// A node that has a node property
void test_value_array_repeated_from_2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_array_repeated_new(
    &frg_global_parsing_range_null,
    5,
    (frg_ast_node_t*)frg_ast_node_value_bool_new(&frg_global_parsing_range_null,
                                                 false));

  frg_ast_print_debug_options_t options = frg_ast_print_debug_options_default;
  options.max_depth                     = 3;
  options.indentation                   = 2;

  frg_ast_debug_printer_value_array_repeated(stream, ast_node, &options);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  // clang-format off
  TEST_ASSERT_EQUAL_STRING(
    "\n"
    "    length = 5\n"
    "    element = [value-bool]\n"
    "      value = false",
    buffer->str
  );
  // clang-format on

  g_string_free(buffer, TRUE);
  frg_ast_node_destroy(ast_node);
}

// A node that has a node list property
void test_value_array_from_2() {
  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  GList* elements = NULL;

  elements        = g_list_append(
    elements, frg_ast_node_value_bool_new(&frg_global_parsing_range_null, false));

  elements = g_list_append(
    elements, frg_ast_node_value_bool_new(&frg_global_parsing_range_null, true));

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_value_array_new(
    &frg_global_parsing_range_null, elements);

  frg_ast_print_debug_options_t options = frg_ast_print_debug_options_default;
  options.max_list_length               = 3;
  options.max_depth                     = 3;
  options.indentation                   = 2;

  frg_ast_debug_printer_value_array(stream, ast_node, &options);

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  // clang-format off
  TEST_ASSERT_EQUAL_STRING(
    "\n"
    "    elements[0] = [value-bool]\n"
    "      value = false\n"
    "    elements[1] = [value-bool]\n"
    "      value = true",
    buffer->str
  );
  // clang-format on

  g_string_free(buffer, TRUE);
  frg_ast_node_destroy(ast_node);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_value_bool_from_0);
  RUN_TEST(test_value_array_repeated_from_0);
  RUN_TEST(test_value_array_from_0);
  RUN_TEST(test_value_bool_from_2);
  RUN_TEST(test_value_array_repeated_from_2);
  RUN_TEST(test_value_array_from_2);
  return UNITY_END();
}
