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

#include <forge-testing/compilation_test/phases/parsing.h>
#include <forge/assert.h>
#include <forge/memory.h>
#include <forge/parsing/parse.h>

frg_testing_compilation_test_context_parsed_t*
  frg_testing_compilation_test_context_parsed_new(frg_ast_node_t* ast_node) {
  frg_assert_pointer_non_null(ast_node);

  frg_testing_compilation_test_context_parsed_t* context_parsed
    = (frg_testing_compilation_test_context_parsed_t*)frg_malloc(
      sizeof(frg_testing_compilation_test_context_parsed_t));

  context_parsed->ast_node = ast_node;

  return context_parsed;
}

void frg_testing_compilation_test_context_parsed_destroy(
  frg_testing_compilation_test_context_parsed_t* context_parsed) {
  frg_assert_pointer_non_null(context_parsed);

  if (context_parsed->ast_node != NULL) {
    frg_ast_node_destroy(context_parsed->ast_node);
  }

  frg_free(context_parsed);
}

frg_testing_compilation_test_context_parsed_t*
  frg_testing_test_compilation_phase_run_parsing(
    const frg_testing_compilation_test_context_initialized_t* context_initialized) {
  frg_assert_pointer_non_null(context_initialized);
  frg_assert_pointer_non_null(context_initialized->mut_source);
  frg_assert_pointer_non_null(context_initialized->message_buffer);

  // Parse
  frg_ast_node_t* ast_node
    = frg_parse(context_initialized->message_buffer, context_initialized->mut_source);

  // Handle errors
  if (ast_node == NULL) {
    return NULL;
  }

  // Return context if successful
  return frg_testing_compilation_test_context_parsed_new(ast_node);
}
