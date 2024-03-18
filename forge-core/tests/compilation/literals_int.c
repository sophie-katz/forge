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

const frg_ast_node_value_int_t* _callback_on_ast_base(const frg_ast_node_t* ast_node) {
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

  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_VALUE_INT, statement_return->value->kind);

  return (const frg_ast_node_value_int_t*)statement_return->value;
}

void _callback_on_ast_u8_0(const frg_ast_node_t* ast_node) {
  const frg_ast_node_value_int_t* value_int = _callback_on_ast_base(ast_node);

  TEST_ASSERT_EQUAL(8, value_int->type.bit_width);
  TEST_ASSERT_TRUE(value_int->type.flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED);
  TEST_ASSERT_EQUAL(0, value_int->value.as_u8);
}

void _callback_on_ast_u8_1(const frg_ast_node_t* ast_node) {
  const frg_ast_node_value_int_t* value_int = _callback_on_ast_base(ast_node);

  TEST_ASSERT_EQUAL(8, value_int->type.bit_width);
  TEST_ASSERT_TRUE(value_int->type.flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED);
  TEST_ASSERT_EQUAL(1, value_int->value.as_u8);
}

void _callback_on_ast_u8_max(const frg_ast_node_t* ast_node) {
  const frg_ast_node_value_int_t* value_int = _callback_on_ast_base(ast_node);

  TEST_ASSERT_EQUAL(8, value_int->type.bit_width);
  TEST_ASSERT_TRUE(value_int->type.flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED);
  TEST_ASSERT_EQUAL(0xff, value_int->value.as_u8);
}

void _callback_on_messages_empty(const frg_message_buffer_t* message_buffer) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
}

void _callback_on_shared_library_handle_u8_0(void* mut_shared_library_handle) {
  uint8_t (*return_u8_0)() = frg_testing_test_compilation_get_function(
    mut_shared_library_handle, "return_u8_0");

  TEST_ASSERT_EQUAL(0, return_u8_0());
}

void _callback_on_shared_library_handle_u8_1(void* mut_shared_library_handle) {
  uint8_t (*return_u8_1)() = frg_testing_test_compilation_get_function(
    mut_shared_library_handle, "return_u8_1");

  TEST_ASSERT_EQUAL(1, return_u8_1());
}

void _callback_on_shared_library_handle_u8_max(void* mut_shared_library_handle) {
  uint8_t (*return_u8_max)() = frg_testing_test_compilation_get_function(
    mut_shared_library_handle, "return_u8_max");

  TEST_ASSERT_EQUAL(0xff, return_u8_max());
}

void test_return_u8_0() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = "return-u8-0";

  // clang-format off
  options->source_text =
    "fn return_u8_0() -> u8 {\n"
    "  return 0u8;\n"
    "}\n"
  ;
  
  options->ast_debug = 
    "[declaration-block]\n"
    "  declarations[0] = [declaration-function]\n"
    "    flags = none\n"
    "    name = \"return_u8_0\"\n"
    "    type = [type-function]\n"
    "      variadic-positional-arguments = [null]\n"
    "      variadic-keyword-arguments = [null]\n"
    "      return-type = [type-int]\n"
    "        flags = unsigned\n"
    "        bit-width = 8\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-int]\n"
    "          type = [type-int]\n"
    "            flags = unsigned\n"
    "            bit-width = 8\n"
    "          value = 0u8"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i8 @return_u8_0() {\n"
    "entry:\n"
    "  ret i8 0\n"
    "}\n"
  ;
  // clang-format on

  options->on_ast                   = _callback_on_ast_u8_0;
  options->on_messages              = _callback_on_messages_empty;
  options->on_shared_library_handle = _callback_on_shared_library_handle_u8_0;

  frg_testing_test_compilation(options);

  frg_testing_compilation_test_options_destroy(options);
}

void test_return_u8_1() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = "return-u8-1";

  // clang-format off
  options->source_text =
    "fn return_u8_1() -> u8 {\n"
    "  return 1u8;\n"
    "}\n"
  ;
  
  options->ast_debug = 
    "[declaration-block]\n"
    "  declarations[0] = [declaration-function]\n"
    "    flags = none\n"
    "    name = \"return_u8_1\"\n"
    "    type = [type-function]\n"
    "      variadic-positional-arguments = [null]\n"
    "      variadic-keyword-arguments = [null]\n"
    "      return-type = [type-int]\n"
    "        flags = unsigned\n"
    "        bit-width = 8\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-int]\n"
    "          type = [type-int]\n"
    "            flags = unsigned\n"
    "            bit-width = 8\n"
    "          value = 1u8"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i8 @return_u8_1() {\n"
    "entry:\n"
    "  ret i8 1\n"
    "}\n"
  ;
  // clang-format on

  options->on_ast                   = _callback_on_ast_u8_1;
  options->on_messages              = _callback_on_messages_empty;
  options->on_shared_library_handle = _callback_on_shared_library_handle_u8_1;

  frg_testing_test_compilation(options);

  frg_testing_compilation_test_options_destroy(options);
}

void test_return_u8_max() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = "return-u8-max";

  // clang-format off
  options->source_text =
    "fn return_u8_max() -> u8 {\n"
    "  return 255u8;\n"
    "}\n"
  ;
  
  options->ast_debug = 
    "[declaration-block]\n"
    "  declarations[0] = [declaration-function]\n"
    "    flags = none\n"
    "    name = \"return_u8_max\"\n"
    "    type = [type-function]\n"
    "      variadic-positional-arguments = [null]\n"
    "      variadic-keyword-arguments = [null]\n"
    "      return-type = [type-int]\n"
    "        flags = unsigned\n"
    "        bit-width = 8\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-int]\n"
    "          type = [type-int]\n"
    "            flags = unsigned\n"
    "            bit-width = 8\n"
    "          value = 255u8"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i8 @return_u8_max() {\n"
    "entry:\n"
    "  ret i8 -1\n"
    "}\n"
  ;
  // clang-format on

  options->on_ast                   = _callback_on_ast_u8_max;
  options->on_messages              = _callback_on_messages_empty;
  options->on_shared_library_handle = _callback_on_shared_library_handle_u8_max;

  frg_testing_test_compilation(options);

  frg_testing_compilation_test_options_destroy(options);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_return_u8_0);
  RUN_TEST(test_return_u8_1);
  RUN_TEST(test_return_u8_max);
  return UNITY_END();
}
