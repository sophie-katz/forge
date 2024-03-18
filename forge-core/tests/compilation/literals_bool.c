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

#include <forge-testing/compilation_test/test.h>
#include <forge/streams/output.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

void _callback_on_ast(const frg_ast_node_t* ast_node) {
  TEST_ASSERT_NOT_NULL(ast_node);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_DECLARATION_BLOCK, ast_node->kind);

  const frg_ast_node_declaration_block_t* declaration_block
    = (const frg_ast_node_declaration_block_t*)ast_node;

  TEST_ASSERT_NOT_NULL(declaration_block->declarations);
  const frg_ast_node_declaration_function_t* declaration_function
    = (const frg_ast_node_declaration_function_t*)declaration_block->declarations->data;
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_DECLARATION_FUNCTION,
                    declaration_function->base.kind);

  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_STATEMENT_BLOCK,
                    declaration_function->body->kind);
  const frg_ast_node_statement_block_t* statement_block
    = (const frg_ast_node_statement_block_t*)declaration_function->body;

  TEST_ASSERT_NOT_NULL(statement_block->statements);
  const frg_ast_node_statement_return_t* statement_return
    = (const frg_ast_node_statement_return_t*)statement_block->statements->data;
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_STATEMENT_RETURN, statement_return->base.kind);

  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_VALUE_BOOL, statement_return->value->kind);
}

void _callback_on_messages(const frg_message_buffer_t* message_buffer) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
}

void _callback_on_shared_library_handle_return_true(void* mut_shared_library_handle) {
  uint32_t (*return_true)() = frg_testing_test_compilation_get_function(
    mut_shared_library_handle, "return_true");

  TEST_ASSERT_EQUAL(1, return_true());
}

void _callback_on_shared_library_handle_return_false(void* mut_shared_library_handle) {
  uint32_t (*return_false)() = frg_testing_test_compilation_get_function(
    mut_shared_library_handle, "return_false");

  TEST_ASSERT_EQUAL(0, return_false());
}

void test_return_true() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = "return-true";

  // clang-format off
  options->source_text =
    "fn return_true() -> bool {\n"
    "  return true;\n"
    "}\n"
  ;
  
  options->ast_debug = 
    "[declaration-block]\n"
    "  declarations[0] = [declaration-function]\n"
    "    flags = none\n"
    "    name = \"return_true\"\n"
    "    type = [type-function]\n"
    "      variadic-positional-arguments = [null]\n"
    "      variadic-keyword-arguments = [null]\n"
    "      return-type = [type-bool]\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-bool]\n"
    "          value = true"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i1 @return_true() {\n"
    "entry:\n"
    "  ret i1 true\n"
    "}\n"
  ;
  // clang-format on

  options->on_ast                   = _callback_on_ast;
  options->on_messages              = _callback_on_messages;
  options->on_shared_library_handle = _callback_on_shared_library_handle_return_true;

  frg_testing_test_compilation(options);

  frg_testing_compilation_test_options_destroy(options);
}

void test_return_false() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = "return-false";

  // clang-format off
  options->source_text =
    "fn return_false() -> bool {\n"
    "  return false;\n"
    "}\n"
  ;
  
  options->ast_debug = 
    "[declaration-block]\n"
    "  declarations[0] = [declaration-function]\n"
    "    flags = none\n"
    "    name = \"return_false\"\n"
    "    type = [type-function]\n"
    "      variadic-positional-arguments = [null]\n"
    "      variadic-keyword-arguments = [null]\n"
    "      return-type = [type-bool]\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-bool]\n"
    "          value = false"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i1 @return_false() {\n"
    "entry:\n"
    "  ret i1 false\n"
    "}\n"
  ;
  // clang-format on

  options->on_ast                   = _callback_on_ast;
  options->on_messages              = _callback_on_messages;
  options->on_shared_library_handle = _callback_on_shared_library_handle_return_false;

  frg_testing_test_compilation(options);

  frg_testing_compilation_test_options_destroy(options);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_return_true);
  RUN_TEST(test_return_false);
  return UNITY_END();
}
