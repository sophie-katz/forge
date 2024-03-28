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
#include <forge-testing/compilation_test/utilities.h>
#include <forge-testing/string_comparison.h>
#include <forge/assert.h>
#include <forge/ast/print_debug.h>
#include <forge/ast/utilities.h>
#include <unity.h>

GString* _frg_testing_test_compilation_auxiliary_print_ast_debug(
  const frg_ast_node_t* ast_node) {
  frg_assert_pointer_non_null(ast_node);

  frg_stream_output_t* ast_debug_stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_print_debug(ast_debug_stream, ast_node, &frg_ast_print_debug_options_default);

  return frg_stream_output_destroy_take_buffer(ast_debug_stream);
}

bool _frg_testing_test_compilation_auxiliary_compare_ast_debug(const char* header,
                                                               const char* expected,
                                                               const char* actual) {
  frg_assert_string_non_empty(header);
  frg_assert_string_non_empty(expected);
  frg_assert_string_non_empty(actual);

  frg_testing_string_comparison_result_t* comparison_result_against_expected
    = frg_testing_compare_strings(
      expected,
      actual,
      FRG_TESTING_STRING_COMPARISON_DEFAULT_CONTEXT_AFTER_FIRST_DIFFERENCE);

  if (comparison_result_against_expected->kind
      != FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_MATCHING) {
    frg_testing_compilation_print_header(header);

    frg_testing_string_comparison_result_print(frg_stream_output_get_stderr(),
                                               comparison_result_against_expected);

    frg_testing_string_comparison_result_destroy(comparison_result_against_expected);

    return false;
  }

  frg_testing_string_comparison_result_destroy(comparison_result_against_expected);

  return true;
}

GString* frg_testing_test_compilation_auxiliary_ast(
  const frg_testing_compilation_test_options_t* options,
  const frg_testing_compilation_test_context_parsed_t* context_parsed) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(context_parsed);
  frg_assert_pointer_non_null(context_parsed->ast_node);

  // Compare parsed AST vs expected
  GString* ast_debug_parsed
    = _frg_testing_test_compilation_auxiliary_print_ast_debug(context_parsed->ast_node);

  if (options->ast_debug != NULL) {
    GString* ast_debug_expected = frg_testing_string_substituter_run(
      options->string_substituter, options->ast_debug);

    if (!_frg_testing_test_compilation_auxiliary_compare_ast_debug(
          "Compared expected AST against parsed",
          ast_debug_expected->str,
          ast_debug_parsed->str)) {
      g_string_free(ast_debug_parsed, TRUE);
      g_string_free(ast_debug_expected, TRUE);
      return g_string_new("Parsed AST does not match expected");
    } else {
      g_string_free(ast_debug_expected, TRUE);
    }
  }

  // Compare cloned AST vs parsed
  frg_ast_node_t* ast_cloned = frg_ast_clone(context_parsed->ast_node);
  GString* ast_debug_cloned
    = _frg_testing_test_compilation_auxiliary_print_ast_debug(ast_cloned);

  bool result = _frg_testing_test_compilation_auxiliary_compare_ast_debug(
    "Compared parsed AST against cloned", ast_debug_parsed->str, ast_debug_cloned->str);

  g_string_free(ast_debug_parsed, TRUE);
  g_string_free(ast_debug_cloned, TRUE);
  frg_ast_node_destroy(ast_cloned);

  if (!result) {
    return g_string_new("Cloned AST does not match parsed");
  }

  // Run callback
  if (options->on_ast != NULL) {
    options->on_ast(context_parsed->ast_node, options->mut_user_data);
  }

  return NULL;
}
