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

void _callback_on_messages_matching(const frg_message_buffer_t* message_buffer,
                                    void* mut_user_data) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
}

void test_matching() {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name        = "function-return-type-matching";
  options->on_messages = _callback_on_messages_matching;

  // clang-format off
  options->source_text =
    "fn f() -> bool {\n"
    "  return true;\n"
    "}\n"
  ;
  
  options->ast_debug = 
    "[declaration-block]\n"
    "  declarations[0] = [declaration-function]\n"
    "    flags = none\n"
    "    name = \"f\"\n"
    "    type = [type-function]\n"
    "      variadic-positional-arguments = [null]\n"
    "      variadic-keyword-arguments = [null]\n"
    "      return-type = [type-bool]\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-bool]\n"
    "          value = true"
  ;
  // clang-format on

  // Run test
  // -----------------------------------------------------------------------------------
  frg_testing_test_compilation(options);

  // Cleanup
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_destroy(options);
}

void _callback_on_messages_mismatched(const frg_message_buffer_t* message_buffer,
                                      void* mut_user_data) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(1, message_buffer->error_count);

  frg_message_query_t query = frg_message_query_empty;
  query.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query.with_code           = "ET-5";

  frg_message_t* message;
  TEST_ASSERT_EQUAL(FRG_MESSAGE_BUFFER_QUERY_RESULT_SINGLE,
                    frg_message_buffer_query_single(&message, message_buffer, &query));

  TEST_ASSERT_EQUAL(2, message->source_range.start.line_number);
  TEST_ASSERT_EQUAL(10, message->source_range.start.column_number);
  TEST_ASSERT_EQUAL(4, message->source_range.length);
  TEST_ASSERT_EQUAL_STRING(
    "Cannot return value of type 'i32' in function with return type 'bool'",
    message->text->str);
  TEST_ASSERT_EQUAL(0, g_list_length(message->children));
}

void _callback_on_ast_mismatched(const frg_ast_node_t* ast_node, void* mut_user_data) {
  // Get relevant AST nodes
  TEST_ASSERT_NOT_NULL(ast_node);
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_DECLARATION_BLOCK, ast_node->kind);

  const frg_ast_node_declaration_block_t* declaration_block
    = (const frg_ast_node_declaration_block_t*)ast_node;

  TEST_ASSERT_NOT_NULL(declaration_block->declarations);
  const frg_ast_node_declaration_function_t* declaration_function
    = (const frg_ast_node_declaration_function_t*)declaration_block->declarations->data;
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_DECLARATION_FUNCTION,
                    declaration_function->base.kind);

  TEST_ASSERT_NOT_NULL(declaration_function->type->return_type);
  const frg_ast_node_t* return_type = declaration_function->type->return_type;
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_TYPE_BOOL, return_type->kind);

  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_STATEMENT_BLOCK,
                    declaration_function->body->kind);
  const frg_ast_node_statement_block_t* statement_block
    = (const frg_ast_node_statement_block_t*)declaration_function->body;

  TEST_ASSERT_NOT_NULL(statement_block->statements);
  const frg_ast_node_statement_return_t* statement_return
    = (const frg_ast_node_statement_return_t*)statement_block->statements->data;
  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_STATEMENT_RETURN, statement_return->base.kind);

  TEST_ASSERT_EQUAL(FRG_AST_NODE_KIND_VALUE_INT, statement_return->value->kind);

  // Check their source locations
  TEST_ASSERT_EQUAL(0, declaration_block->base.source_range.start.line_number);
  TEST_ASSERT_EQUAL(0, declaration_block->base.source_range.start.column_number);
  TEST_ASSERT_EQUAL(0, declaration_block->base.source_range.start.offset);
  TEST_ASSERT_EQUAL(0, declaration_block->base.source_range.length);

  TEST_ASSERT_EQUAL(1, declaration_function->base.source_range.start.line_number);
  TEST_ASSERT_EQUAL(1, declaration_function->base.source_range.start.column_number);
  TEST_ASSERT_EQUAL(0, declaration_function->base.source_range.start.offset);
  TEST_ASSERT_EQUAL(33, declaration_function->base.source_range.length);

  TEST_ASSERT_EQUAL(1, declaration_function->type->base.source_range.start.line_number);
  TEST_ASSERT_EQUAL(5,
                    declaration_function->type->base.source_range.start.column_number);
  TEST_ASSERT_EQUAL(4, declaration_function->type->base.source_range.start.offset);
  TEST_ASSERT_EQUAL(10, declaration_function->type->base.source_range.length);

  TEST_ASSERT_EQUAL(
    1, declaration_function->type->return_type->source_range.start.line_number);
  TEST_ASSERT_EQUAL(
    11, declaration_function->type->return_type->source_range.start.column_number);
  TEST_ASSERT_EQUAL(10,
                    declaration_function->type->return_type->source_range.start.offset);
  TEST_ASSERT_EQUAL(4, declaration_function->type->return_type->source_range.length);

  TEST_ASSERT_EQUAL(1, statement_block->base.source_range.start.line_number);
  TEST_ASSERT_EQUAL(16, statement_block->base.source_range.start.column_number);
  TEST_ASSERT_EQUAL(15, statement_block->base.source_range.start.offset);
  TEST_ASSERT_EQUAL(18, statement_block->base.source_range.length);

  TEST_ASSERT_EQUAL(2, statement_return->base.source_range.start.line_number);
  TEST_ASSERT_EQUAL(3, statement_return->base.source_range.start.column_number);
  TEST_ASSERT_EQUAL(19, statement_return->base.source_range.start.offset);
  TEST_ASSERT_EQUAL(12, statement_return->base.source_range.length);

  TEST_ASSERT_EQUAL(2, statement_return->value->source_range.start.line_number);
  TEST_ASSERT_EQUAL(10, statement_return->value->source_range.start.column_number);
  TEST_ASSERT_EQUAL(26, statement_return->value->source_range.start.offset);
  TEST_ASSERT_EQUAL(4, statement_return->value->source_range.length);
}

void test_mismatched() {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_VERIFY;
  options->name        = "function-return-type-mismatched";
  options->on_ast      = _callback_on_ast_mismatched;
  options->on_messages = _callback_on_messages_mismatched;

  // clang-format off
  options->source_text =
    "fn f() -> bool {\n"
    "  return 0i32;\n"
    "}\n"
  ;
  
  options->ast_debug = 
    "[declaration-block]\n"
    "  declarations[0] = [declaration-function]\n"
    "    flags = none\n"
    "    name = \"f\"\n"
    "    type = [type-function]\n"
    "      variadic-positional-arguments = [null]\n"
    "      variadic-keyword-arguments = [null]\n"
    "      return-type = [type-bool]\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-int]\n"
    "          type = [type-int]\n"
    "            flags = none\n"
    "            bit-width = 32\n"
    "          value = 0i32"
  ;
  // clang-format on

  // Run test
  // -----------------------------------------------------------------------------------
  frg_testing_test_compilation(options);

  // Cleanup
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_destroy(options);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_matching);
  RUN_TEST(test_mismatched);
  return UNITY_END();
}
