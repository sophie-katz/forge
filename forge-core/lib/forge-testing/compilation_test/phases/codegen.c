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

#include <forge-testing/compilation_test/phases/codegen.h>
#include <forge-testing/compilation_test/utilities.h>
#include <forge-testing/string_comparison.h>
#include <forge/assert.h>
#include <forge/memory.h>

frg_testing_compilation_test_context_codegen_t*
  frg_testing_compilation_test_context_codegen_new(
    frg_codegen_module_t* codegen_module) {
  frg_assert_pointer_non_null(codegen_module);

  frg_testing_compilation_test_context_codegen_t* context_codegen
    = (frg_testing_compilation_test_context_codegen_t*)frg_malloc(
      sizeof(frg_testing_compilation_test_context_codegen_t));

  context_codegen->codegen_module = codegen_module;

  return context_codegen;
}

void frg_testing_compilation_test_context_codegen_destroy(
  frg_testing_compilation_test_context_codegen_t* context_codegen) {
  frg_assert_pointer_non_null(context_codegen);

  if (context_codegen->codegen_module != NULL) {
    frg_codegen_module_destroy(context_codegen->codegen_module);
  }

  frg_free(context_codegen);
}

frg_testing_compilation_test_context_codegen_t*
  frg_testing_test_compilation_phase_run_codegen(
    const frg_testing_compilation_test_context_parsed_t* context_parsed) {
  frg_assert_pointer_non_null(context_parsed);
  frg_assert_pointer_non_null(context_parsed->ast_node);

  // Codegen
  frg_codegen_module_t* codegen_module = frg_codegen(context_parsed->ast_node);

  // Handle errors
  if (codegen_module == NULL) {
    return NULL;
  }

  // Return context if successful
  return frg_testing_compilation_test_context_codegen_new(codegen_module);
}
