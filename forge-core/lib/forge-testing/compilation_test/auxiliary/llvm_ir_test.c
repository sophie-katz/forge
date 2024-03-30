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

#include <forge-testing/compilation_test/auxiliary/llvm_ir.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

void test_successful_no_expected_llvm_ir() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_declaration_block_new(
    &frg_global_parsing_range_null, NULL);

  frg_codegen_module_t* codegen_module = frg_codegen(ast_node);

  frg_testing_compilation_test_context_codegen_t* context_codegen
    = frg_testing_compilation_test_context_codegen_new(codegen_module);

  GString* message
    = frg_testing_test_compilation_auxiliary_llvm_ir(options, context_codegen);

  TEST_ASSERT_NULL(message);

  frg_testing_compilation_test_context_codegen_destroy(context_codegen);
  frg_ast_node_destroy(ast_node);
  frg_testing_compilation_test_options_destroy(options);
}

void test_successful_expected_llvm_ir() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->llvm_ir         = "; ModuleID = 'forge'\nsource_filename = \"forge\"\n";

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_declaration_block_new(
    &frg_global_parsing_range_null, NULL);

  frg_codegen_module_t* codegen_module = frg_codegen(ast_node);

  frg_testing_compilation_test_context_codegen_t* context_codegen
    = frg_testing_compilation_test_context_codegen_new(codegen_module);

  GString* message
    = frg_testing_test_compilation_auxiliary_llvm_ir(options, context_codegen);

  TEST_ASSERT_NULL(message);

  frg_testing_compilation_test_context_codegen_destroy(context_codegen);
  frg_ast_node_destroy(ast_node);
  frg_testing_compilation_test_options_destroy(options);
}

void test_unsuccessful_not_matching() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->llvm_ir         = "; ModuleID = 'forge'\nsource_filename = \"asdf\"\n";

  frg_ast_node_t* ast_node = (frg_ast_node_t*)frg_ast_node_declaration_block_new(
    &frg_global_parsing_range_null, NULL);

  frg_codegen_module_t* codegen_module = frg_codegen(ast_node);

  frg_testing_compilation_test_context_codegen_t* context_codegen
    = frg_testing_compilation_test_context_codegen_new(codegen_module);

  GString* message
    = frg_testing_test_compilation_auxiliary_llvm_ir(options, context_codegen);

  TEST_ASSERT_EQUAL_STRING("Generated LLVM IR does not match expected", message->str);

  g_string_free(message, TRUE);
  frg_testing_compilation_test_context_codegen_destroy(context_codegen);
  frg_ast_node_destroy(ast_node);
  frg_testing_compilation_test_options_destroy(options);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_successful_no_expected_llvm_ir);
  RUN_TEST(test_successful_expected_llvm_ir);
  RUN_TEST(test_unsuccessful_not_matching);
  return UNITY_END();
}
