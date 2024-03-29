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

void _callback_on_messages_empty(const frg_message_buffer_t* message_buffer,
                                 void* mut_user_data) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(0, message_buffer->error_count);
}

typedef struct {
  const char* operand_text;
  frg_ast_node_value_int_t expected_operand;
  frg_ast_node_value_int_t expected_returned;
} frg_test_bit_not_parameters_t;

void _callback_on_shared_library_handle(void* mut_shared_library_handle,
                                        void* mut_user_data) {
  const frg_test_bit_not_parameters_t* parameters
    = (const frg_test_bit_not_parameters_t*)mut_user_data;

  frg_testing_assert_function_returns_value_int(
    frg_testing_test_compilation_get_function(mut_shared_library_handle, "f"),
    &parameters->expected_returned);
}

void _test_parameters(const frg_test_bit_not_parameters_t* parameters) {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_string_substituter_add_value_int(
    options, "operand", parameters->operand_text, &parameters->expected_operand);
  frg_testing_compilation_test_options_string_substituter_add_value_int(
    options, "returned", NULL, &parameters->expected_returned);

  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name        = "bit-operations-not-%(returned-type-keyword)-%(operand-text)";
  options->on_messages = _callback_on_messages_empty;
  options->on_shared_library_handle = _callback_on_shared_library_handle;
  options->mut_user_data            = (void*)parameters;

  // clang-format off
  options->source_text =
    "fn f() -> %(returned-type-keyword) {\n"
    "  return ~%(operand-text);\n"
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
    "        flags = %(returned-type-flags)\n"
    "        bit-width = %(returned-type-bit-width)\n"
    "    body = [statement-block]\n"
    "      statements[0] = [statement-return]\n"
    "        value = [value-bit-not]\n"
    "          operand = [value-int]\n"
    "            type = [type-int]\n"
    "              flags = %(operand-type-flags)\n"
    "              bit-width = %(operand-type-bit-width)\n"
    "            value = %(operand)%(operand-type-keyword)"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i%(returned-type-bit-width) @f() {\n"
    "entry:\n"
    "  ret i%(returned-type-bit-width) %(returned-signed)\n"
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
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "0u8",
        .expected_operand
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u8 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 255;

  _test_parameters(&parameters);
}

void test_u8_1() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "1u8",
        .expected_operand
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u8 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 254;

  _test_parameters(&parameters);
}

void test_u8_max() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "255u8",
        .expected_operand
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u8 = 255;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 0;

  _test_parameters(&parameters);
}

void test_u16_0() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "0u16",
        .expected_operand
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u16 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 65535;

  _test_parameters(&parameters);
}

void test_u16_1() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "1u16",
        .expected_operand
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u16 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 65534;

  _test_parameters(&parameters);
}

void test_u16_max() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "65535u16",
        .expected_operand
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u16 = 65535;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 0;

  _test_parameters(&parameters);
}

void test_u32_0() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "0u32",
        .expected_operand
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u32 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 4294967295;

  _test_parameters(&parameters);
}

void test_u32_1() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "1u32",
        .expected_operand
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u32 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 4294967294;

  _test_parameters(&parameters);
}

void test_u32_max() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "4294967295u32",
        .expected_operand
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u32 = 4294967295;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 0;

  _test_parameters(&parameters);
}

void test_u64_0() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "0u64",
        .expected_operand
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u64 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 18446744073709551615LU;

  _test_parameters(&parameters);
}

void test_u64_1() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "1u64",
        .expected_operand
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u64 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 18446744073709551614LU;

  _test_parameters(&parameters);
}

void test_u64_max() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "18446744073709551615u64",
        .expected_operand
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_u64 = 18446744073709551615LU;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 0;

  _test_parameters(&parameters);
}

void test_i8_0() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "0i8",
        .expected_operand
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i8 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = -1;

  _test_parameters(&parameters);
}

void test_i8_1() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "1i8",
        .expected_operand
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i8 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = -2;

  _test_parameters(&parameters);
}

void test_i8_max() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "255i8",
        .expected_operand
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i8 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 0;

  _test_parameters(&parameters);
}

void test_i16_0() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "0i16",
        .expected_operand
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i16 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = -1;

  _test_parameters(&parameters);
}

void test_i16_1() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "1i16",
        .expected_operand
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i16 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = -2;

  _test_parameters(&parameters);
}

void test_i16_max() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "65535i16",
        .expected_operand
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i16 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 0;

  _test_parameters(&parameters);
}

void test_i32_0() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "0i32",
        .expected_operand
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i32 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = -1;

  _test_parameters(&parameters);
}

void test_i32_1() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "1i32",
        .expected_operand
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i32 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = -2;

  _test_parameters(&parameters);
}

void test_i32_max() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "4294967295i32",
        .expected_operand
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i32 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 0;

  _test_parameters(&parameters);
}

void test_i64_0() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "0i64",
        .expected_operand
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i64 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = -1;

  _test_parameters(&parameters);
}

void test_i64_1() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "1i64",
        .expected_operand
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i64 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = -2;

  _test_parameters(&parameters);
}

void test_i64_max() {
  frg_test_bit_not_parameters_t parameters
    = { .operand_text = "18446744073709551615i64",
        .expected_operand
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(
    &parameters.expected_operand.value, 0, sizeof(parameters.expected_operand.value));
  parameters.expected_operand.value.as_i64 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 0;

  _test_parameters(&parameters);
}

void _callback_on_messages_unsupported_type(const frg_message_buffer_t* message_buffer,
                                            void* mut_user_data) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(1, message_buffer->error_count);

  frg_message_query_t query = frg_message_query_empty;
  query.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query.with_code           = "ET-6";

  frg_message_t* message;
  TEST_ASSERT_EQUAL(FRG_MESSAGE_BUFFER_QUERY_RESULT_SINGLE,
                    frg_message_buffer_query_single(&message, message_buffer, &query));

  TEST_ASSERT_EQUAL(2, message->source_range.start.line_number);
  TEST_ASSERT_EQUAL(11, message->source_range.start.column_number);
  TEST_ASSERT_EQUAL(4, message->source_range.length);
  TEST_ASSERT_EQUAL_STRING("Operator ~'s operand must be int, but is 'bool'",
                           message->text->str);
  TEST_ASSERT_EQUAL(0, g_list_length(message->children));
}

void test_unsupported_type() {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name        = "bit-not-unsupported-type";
  options->on_messages = _callback_on_messages_unsupported_type;

  // clang-format off
  options->source_text =
    "fn f() -> bool {\n"
    "  return ~true;\n"
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
    "        value = [value-bit-not]\n"
    "          operand = [value-bool]\n"
    "            value = true"
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
  RUN_TEST(test_u8_0);
  RUN_TEST(test_u8_1);
  RUN_TEST(test_u8_max);
  RUN_TEST(test_u16_0);
  RUN_TEST(test_u16_1);
  RUN_TEST(test_u16_max);
  RUN_TEST(test_u32_0);
  RUN_TEST(test_u32_1);
  RUN_TEST(test_u32_max);
  RUN_TEST(test_u64_0);
  RUN_TEST(test_u64_1);
  RUN_TEST(test_u64_max);
  RUN_TEST(test_i8_0);
  RUN_TEST(test_i8_1);
  RUN_TEST(test_i8_max);
  RUN_TEST(test_i16_0);
  RUN_TEST(test_i16_1);
  RUN_TEST(test_i16_max);
  RUN_TEST(test_i32_0);
  RUN_TEST(test_i32_1);
  RUN_TEST(test_i32_max);
  RUN_TEST(test_i64_0);
  RUN_TEST(test_i64_1);
  RUN_TEST(test_i64_max);
  RUN_TEST(test_unsupported_type);
  return UNITY_END();
}
