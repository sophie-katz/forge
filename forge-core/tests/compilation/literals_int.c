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
#include <forge/ast/utilities.h>
#include <forge/streams/output.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

typedef struct {
  const char* value_text;
  frg_ast_node_value_int_t expected_value;
} frg_test_literals_int_parameters_t;

void _callback_on_ast(const frg_ast_node_t* ast_node, void* mut_user_data) {
  const frg_test_literals_int_parameters_t* parameters
    = (const frg_test_literals_int_parameters_t*)mut_user_data;

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

  TEST_ASSERT_EQUAL(
    parameters->expected_value.type.bit_width,
    ((const frg_ast_node_value_int_t*)statement_return->value)->type.bit_width);
  TEST_ASSERT_EQUAL(
    parameters->expected_value.type.flags,
    ((const frg_ast_node_value_int_t*)statement_return->value)->type.flags);
  TEST_ASSERT_EQUAL_MEMORY(
    &parameters->expected_value.value,
    &((const frg_ast_node_value_int_t*)statement_return->value)->value,
    sizeof(parameters->expected_value.value));
}

void _callback_on_messages(const frg_message_buffer_t* message_buffer,
                           void* mut_user_data) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
}

void _callback_on_shared_library_handle(void* mut_shared_library_handle,
                                        void* mut_user_data) {
  const frg_test_literals_int_parameters_t* parameters
    = (const frg_test_literals_int_parameters_t*)mut_user_data;

  frg_ast_node_value_int_t actual_value;

  memset(&actual_value.value, 0, sizeof(actual_value.value));

  uint8_t (*f_8)();
  uint16_t (*f_16)();
  uint32_t (*f_32)();
  uint64_t (*f_64)();

  switch (parameters->expected_value.type.bit_width) {
  case 8:
    f_8 = frg_testing_test_compilation_get_function(mut_shared_library_handle, "f");
    actual_value.value.as_u8 = f_8();
  case 16:
    f_16 = frg_testing_test_compilation_get_function(mut_shared_library_handle, "f");
    actual_value.value.as_u16 = f_16();
  case 32:
    f_32 = frg_testing_test_compilation_get_function(mut_shared_library_handle, "f");
    actual_value.value.as_u32 = f_32();
  case 64:
    f_64 = frg_testing_test_compilation_get_function(mut_shared_library_handle, "f");
    actual_value.value.as_u64 = f_64();
  }

  TEST_ASSERT_EQUAL_MEMORY(
    &parameters->expected_value.value, &actual_value.value, sizeof(actual_value.value));
}

void _test_parameters(const frg_test_literals_int_parameters_t* parameters) {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  frg_testing_string_substituter_add_printf(
    options->string_substituter,
    "forge-type",
    "%c%u",
    parameters->expected_value.type.flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED ? 'u'
                                                                                : 'i',
    parameters->expected_value.type.bit_width);

  frg_testing_string_substituter_add_printf(options->string_substituter,
                                            "bit-width",
                                            "%u",
                                            parameters->expected_value.type.bit_width);

  frg_testing_string_substituter_add_printf(options->string_substituter,
                                            "flags",
                                            "%s",
                                            parameters->expected_value.type.flags
                                                & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED
                                              ? "unsigned"
                                              : "none");

  frg_testing_string_substituter_add_str(
    options->string_substituter, "value-text", parameters->value_text);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_value_int_print_unsigned(stream, &parameters->expected_value, 10, 0);

  frg_testing_string_substituter_add_string(
    options->string_substituter,
    "value-decimal-unsigned",
    frg_stream_output_destroy_take_buffer(stream));

  stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_value_int_print_signed(stream, &parameters->expected_value, 10, 0);

  frg_testing_string_substituter_add_string(
    options->string_substituter,
    "value-decimal-signed",
    frg_stream_output_destroy_take_buffer(stream));

  stream = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_value_int_print(stream, &parameters->expected_value, 10, 0);

  frg_testing_string_substituter_add_string(
    options->string_substituter,
    "value-decimal",
    frg_stream_output_destroy_take_buffer(stream));

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = "return-%(forge-type)-%(value-text)";
  options->on_ast                   = _callback_on_ast;
  options->on_messages              = _callback_on_messages;
  options->on_shared_library_handle = _callback_on_shared_library_handle;
  options->mut_user_data            = (void*)parameters;

  // clang-format off
  options->source_text =
    "fn f() -> %(forge-type) {\n"
    "  return %(value-text);\n"
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
    "      return-type = [type-int]\n"
    "        flags = %(flags)\n"
    "        bit-width = %(bit-width)\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-int]\n"
    "          type = [type-int]\n"
    "            flags = %(flags)\n"
    "            bit-width = %(bit-width)\n"
    "          value = %(value-decimal)%(forge-type)"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i%(bit-width) @f() {\n"
    "entry:\n"
    "  ret i%(bit-width) %(value-decimal-signed)\n"
    "}\n"
  ;
  // clang-format on

  // Run test
  // -----------------------------------------------------------------------------------
  frg_testing_test_compilation(options);

  // Cleanup
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_destroy(options);
}

void test_u8_0() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "0u8",
        .expected_value = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u8 = 0;

  _test_parameters(&parameters);
}

void test_u8_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "1u8",
        .expected_value = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u8 = 1;

  _test_parameters(&parameters);
}

void test_u8_max() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "255u8",
        .expected_value = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u8 = 0xff;

  _test_parameters(&parameters);
}

void test_u8_overflow_by_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "256u8",
        .expected_value = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u8 = 0;

  _test_parameters(&parameters);
}

void test_u8_overflow_by_2() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "257u8",
        .expected_value = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u8 = 1;

  _test_parameters(&parameters);
}

void test_u16_0() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "0u16",
        .expected_value = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u16 = 0;

  _test_parameters(&parameters);
}

void test_u16_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "1u16",
        .expected_value = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u16 = 1;

  _test_parameters(&parameters);
}

void test_u16_max() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "65535u16",
        .expected_value = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u16 = 0xffff;

  _test_parameters(&parameters);
}

void test_u16_overflow_by_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "65536u16",
        .expected_value = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u16 = 0;

  _test_parameters(&parameters);
}

void test_u16_overflow_by_2() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "65537u16",
        .expected_value = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u16 = 1;

  _test_parameters(&parameters);
}

void test_u32_0() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "0u32",
        .expected_value = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u32 = 0;

  _test_parameters(&parameters);
}

void test_u32_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "1u32",
        .expected_value = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u32 = 1;

  _test_parameters(&parameters);
}

void test_u32_max() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "4294967295u32",
        .expected_value = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u32 = 0xffffffff;

  _test_parameters(&parameters);
}

void test_u32_overflow_by_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "4294967296u32",
        .expected_value = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u32 = 0;

  _test_parameters(&parameters);
}

void test_u32_overflow_by_2() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "4294967297u32",
        .expected_value = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u32 = 1;

  _test_parameters(&parameters);
}

void test_u64_0() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "0u64",
        .expected_value = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u64 = 0;

  _test_parameters(&parameters);
}

void test_u64_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "1u64",
        .expected_value = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u64 = 1;

  _test_parameters(&parameters);
}

void test_u64_max() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "18446744073709551615u64",
        .expected_value = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u64 = 0xffffffffffffffffL;

  _test_parameters(&parameters);
}

void test_u64_overflow_by_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "18446744073709551616u64",
        .expected_value = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u64 = 0;

  _test_parameters(&parameters);
}

void test_u64_overflow_by_2() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text     = "18446744073709551617u64",
        .expected_value = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_u64 = 1;

  _test_parameters(&parameters);
}

void test_i8_0() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text = "0i8",
        .expected_value
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_i8 = 0;

  _test_parameters(&parameters);
}

void test_i8_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text = "1i8",
        .expected_value
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_i8 = 1;

  _test_parameters(&parameters);
}

void test_i8_max() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text = "127i8",
        .expected_value
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_i8 = 127;

  _test_parameters(&parameters);
}

void test_i8_overflow_by_1() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text = "128i8",
        .expected_value
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_i8 = -128;

  _test_parameters(&parameters);
}

void test_i8_overflow_by_2() {
  frg_test_literals_int_parameters_t parameters
    = { .value_text = "129i8",
        .expected_value
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_value.value, 0, sizeof(parameters.expected_value.value));
  parameters.expected_value.value.as_i8 = -127;

  _test_parameters(&parameters);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_u8_0);
  RUN_TEST(test_u8_1);
  RUN_TEST(test_u8_max);
  RUN_TEST(test_u8_overflow_by_1);
  RUN_TEST(test_u8_overflow_by_2);
  RUN_TEST(test_u16_0);
  RUN_TEST(test_u16_1);
  RUN_TEST(test_u16_max);
  RUN_TEST(test_u16_overflow_by_1);
  RUN_TEST(test_u16_overflow_by_2);
  RUN_TEST(test_u32_0);
  RUN_TEST(test_u32_1);
  RUN_TEST(test_u32_max);
  RUN_TEST(test_u32_overflow_by_1);
  RUN_TEST(test_u32_overflow_by_2);
  RUN_TEST(test_u64_0);
  RUN_TEST(test_u64_1);
  RUN_TEST(test_u64_max);
  RUN_TEST(test_u64_overflow_by_1);
  RUN_TEST(test_u64_overflow_by_2);
  RUN_TEST(test_i8_0);
  RUN_TEST(test_i8_1);
  RUN_TEST(test_i8_max);
  RUN_TEST(test_i8_overflow_by_1);
  RUN_TEST(test_i8_overflow_by_2);
  return UNITY_END();
}
