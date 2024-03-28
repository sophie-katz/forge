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

#include <forge-testing/compilation_test/auxiliary/llvm_ir.h>
#include <forge-testing/compilation_test/utilities.h>
#include <forge-testing/string_comparison.h>
#include <forge/assert.h>
#include <unity.h>

GString* _frg_testing_test_compilation_auxiliary_llvm_ir_format(
  const frg_testing_compilation_test_context_codegen_t* context_codegen) {
  frg_assert_pointer_non_null(context_codegen);
  frg_assert_pointer_non_null(context_codegen->codegen_module);

  frg_stream_output_t* stream_llvm_ir
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_codegen_module_print(stream_llvm_ir, context_codegen->codegen_module);

  return frg_stream_output_destroy_take_buffer(stream_llvm_ir);
}

GString* frg_testing_test_compilation_auxiliary_llvm_ir(
  const frg_testing_compilation_test_options_t* options,
  const frg_testing_compilation_test_context_codegen_t* context_codegen) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(context_codegen);
  frg_assert_pointer_non_null(context_codegen->codegen_module);

  if (options->llvm_ir != NULL) {
    GString* llvm_ir_actual
      = _frg_testing_test_compilation_auxiliary_llvm_ir_format(context_codegen);

    GString* llvm_ir_expected = frg_testing_string_substituter_run(
      options->string_substituter, options->llvm_ir);

    frg_testing_string_comparison_result_t* comparison_result_against_expected
      = frg_testing_compare_strings(
        llvm_ir_expected->str,
        llvm_ir_actual->str,
        FRG_TESTING_STRING_COMPARISON_DEFAULT_CONTEXT_AFTER_FIRST_DIFFERENCE);

    g_string_free(llvm_ir_expected, TRUE);

    bool result = true;

    if (comparison_result_against_expected->kind
        != FRG_TESTING_STRING_COMPARISON_ENTRY_KIND_MATCHING) {
      frg_testing_compilation_print_header("Compared LLVM IR against expected");

      frg_testing_string_comparison_result_print(frg_stream_output_get_stderr(),
                                                 comparison_result_against_expected);

      result = false;
    }

    frg_testing_string_comparison_result_destroy(comparison_result_against_expected);
    g_string_free(llvm_ir_actual, TRUE);

    if (!result) {
      return g_string_new("Generated LLVM IR does not match expected");
    }
  }

  return NULL;
}
