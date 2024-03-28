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

#include <forge-testing/compilation_test/options.h>
#include <forge/assert.h>
#include <forge/memory.h>

frg_testing_compilation_test_options_t*
  frg_testing_compilation_test_options_new_default() {
  frg_testing_compilation_test_options_t* options
    = (frg_testing_compilation_test_options_t*)frg_malloc(
      sizeof(frg_testing_compilation_test_options_t));

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = NULL;
  options->source_text              = NULL;
  options->ast_debug                = NULL;
  options->llvm_ir                  = NULL;
  options->on_ast                   = NULL;
  options->on_messages              = NULL;
  options->on_shared_library_handle = NULL;
  options->mut_user_data            = NULL;
  options->string_substituter       = frg_testing_string_substituter_new();

  return options;
}

void frg_testing_compilation_test_options_destroy(
  frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);

  frg_testing_string_substituter_destroy(options->string_substituter);

  frg_free(options);
}

void frg_testing_compilation_test_options_validate(
  const frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);
  frg_assert_string_non_empty(options->name);

  // TODO: Have this run after substitution
  // for (const char* name_iter = options->name; *name_iter != 0; name_iter++) {
  //   frg_assert_with_message(
  //     *name_iter == '-' || (*name_iter >= 'a' && *name_iter <= 'z')
  //       || (*name_iter >= '0' && *name_iter <= '9'),
  //     "test names must be kebab case with only lowercase ASCII letters and numbers");
  // }

  frg_assert_pointer_non_null(options->source_text);
}
