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
  const char* left_text;
  frg_ast_node_value_int_t expected_left;
  uint32_t right;
  const char* returned_llvm_ir;
  frg_ast_node_value_int_t expected_returned;
} frg_test_bit_shift_right_parameters_t;

void _callback_on_shared_library_handle(void* mut_shared_library_handle,
                                        void* mut_user_data) {
  const frg_test_bit_shift_right_parameters_t* parameters
    = (const frg_test_bit_shift_right_parameters_t*)mut_user_data;

  frg_testing_assert_function_returns_value_int(
    frg_testing_test_compilation_get_function(mut_shared_library_handle, "f"),
    &parameters->expected_returned);
}

void _test_parameters(const frg_test_bit_shift_right_parameters_t* parameters) {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_string_substituter_add_value_int(
    options, "left", parameters->left_text, &parameters->expected_left);
  frg_ast_node_value_int_t right_value = {
    .type = {.flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED, .bit_width = 32}
  };
  memset(&right_value.value, 0, sizeof(right_value.value));
  right_value.value.as_u32 = parameters->right;
  frg_testing_compilation_test_options_string_substituter_add_value_int(
    options, "right", NULL, &right_value);
  frg_testing_compilation_test_options_string_substituter_add_value_int(
    options, "returned", NULL, &parameters->expected_returned);
  frg_testing_string_substituter_add_str(
    options->string_substituter, "returned-llvm-ir", parameters->returned_llvm_ir);

  options->kind = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name
    = "bit-operations-shift-right-%(returned-type-keyword)-%(left-text)-%(right)";
  options->on_messages              = _callback_on_messages_empty;
  options->on_shared_library_handle = _callback_on_shared_library_handle;
  options->mut_user_data            = (void*)parameters;

  // clang-format off
  options->source_text =
    "fn f() -> %(returned-type-keyword) {\n"
    "  return %(left-text) >> %(right)u32;\n"
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
    "        value = [value-bit-shift-right]\n"
    "          left = [value-int]\n"
    "            type = [type-int]\n"
    "              flags = %(left-type-flags)\n"
    "              bit-width = %(left-type-bit-width)\n"
    "            value = %(left)%(left-type-keyword)\n"
    "          right = [value-int]\n"
    "            type = [type-int]\n"
    "              flags = %(right-type-flags)\n"
    "              bit-width = %(right-type-bit-width)\n"
    "            value = %(right)%(right-type-keyword)"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i%(returned-type-bit-width) @f() {\n"
    "entry:\n"
    "  ret i%(returned-type-bit-width) %(returned-llvm-ir)\n"
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

void test_u8_0_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 0;

  _test_parameters(&parameters);
}

void test_u8_0_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 0;

  _test_parameters(&parameters);
}

void test_u8_0_7() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 7,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 0;

  _test_parameters(&parameters);
}

void test_u8_1_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "1",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 1;

  _test_parameters(&parameters);
}

void test_u8_1_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 0;

  _test_parameters(&parameters);
}

void test_u8_1_7() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 7,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 0;

  _test_parameters(&parameters);
}

void test_u8_max_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "255u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "-1",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 255;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 255;

  _test_parameters(&parameters);
}

void test_u8_max_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "255u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "127",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 255;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 127;

  _test_parameters(&parameters);
}

void test_u8_max_7() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "255u8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 7,
        .returned_llvm_ir  = "1",
        .expected_returned = {
          .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u8 = 255;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u8 = 1;

  _test_parameters(&parameters);
}

void test_u16_0_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 0;

  _test_parameters(&parameters);
}

void test_u16_0_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 0;

  _test_parameters(&parameters);
}

void test_u16_0_15() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 15,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 0;

  _test_parameters(&parameters);
}

void test_u16_1_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "1",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 1;

  _test_parameters(&parameters);
}

void test_u16_1_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 0;

  _test_parameters(&parameters);
}

void test_u16_1_15() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 15,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 0;

  _test_parameters(&parameters);
}

void test_u16_max_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "65535u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "-1",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 65535;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 65535;

  _test_parameters(&parameters);
}

void test_u16_max_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "65535u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "32767",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 65535;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 32767;

  _test_parameters(&parameters);
}

void test_u16_max_15() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "65535u16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 15,
        .returned_llvm_ir  = "1",
        .expected_returned = {
          .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u16 = 65535;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u16 = 1;

  _test_parameters(&parameters);
}

void test_u32_0_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 0;

  _test_parameters(&parameters);
}

void test_u32_0_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 0;

  _test_parameters(&parameters);
}

void test_u32_0_31() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 31,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 0;

  _test_parameters(&parameters);
}

void test_u32_1_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "1",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 1;

  _test_parameters(&parameters);
}

void test_u32_1_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 0;

  _test_parameters(&parameters);
}

void test_u32_1_31() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 31,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 0;

  _test_parameters(&parameters);
}

void test_u32_max_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "4294967295u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "-1",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 4294967295;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 4294967295;

  _test_parameters(&parameters);
}

void test_u32_max_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "4294967295u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "2147483647",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 4294967295;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 2147483647;

  _test_parameters(&parameters);
}

void test_u32_max_31() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "4294967295u32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 31,
        .returned_llvm_ir  = "1",
        .expected_returned = {
          .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u32 = 4294967295;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u32 = 1;

  _test_parameters(&parameters);
}

void test_u64_0_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 0;

  _test_parameters(&parameters);
}

void test_u64_0_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 0;

  _test_parameters(&parameters);
}

void test_u64_0_63() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 63,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 0;

  _test_parameters(&parameters);
}

void test_u64_1_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "1",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 1;

  _test_parameters(&parameters);
}

void test_u64_1_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 0;

  _test_parameters(&parameters);
}

void test_u64_1_63() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 63,
        .returned_llvm_ir  = "0",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 0;

  _test_parameters(&parameters);
}

void test_u64_max_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "18446744073709551615u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 0,
        .returned_llvm_ir  = "-1",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 18446744073709551615LU;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 18446744073709551615LU;

  _test_parameters(&parameters);
}

void test_u64_max_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "18446744073709551615u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 1,
        .returned_llvm_ir  = "9223372036854775807",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 18446744073709551615LU;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 9223372036854775807LU;

  _test_parameters(&parameters);
}

void test_u64_max_63() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "18446744073709551615u64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } },
        .right             = 63,
        .returned_llvm_ir  = "1",
        .expected_returned = {
          .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_u64 = 18446744073709551615LU;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_u64 = 1;

  _test_parameters(&parameters);
}

void test_i8_0_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 0;

  _test_parameters(&parameters);
}

void test_i8_0_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 0;

  _test_parameters(&parameters);
}

void test_i8_0_7() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 7,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 0;

  _test_parameters(&parameters);
}

void test_i8_1_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "1",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 1;

  _test_parameters(&parameters);
}

void test_i8_1_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 0;

  _test_parameters(&parameters);
}

void test_i8_1_7() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 7,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 0;

  _test_parameters(&parameters);
}

void test_i8_max_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "255i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "-1",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = -1;

  _test_parameters(&parameters);
}

void test_i8_max_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "255i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "127",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 127;

  _test_parameters(&parameters);
}

void test_i8_max_7() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "255i8",
        .expected_left
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 7,
        .returned_llvm_ir = "1",
        .expected_returned
        = { .type = { .bit_width = 8, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i8 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i8 = 1;

  _test_parameters(&parameters);
}

void test_i16_0_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 0;

  _test_parameters(&parameters);
}

void test_i16_0_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 0;

  _test_parameters(&parameters);
}

void test_i16_0_15() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 15,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 0;

  _test_parameters(&parameters);
}

void test_i16_1_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "1",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 1;

  _test_parameters(&parameters);
}

void test_i16_1_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 0;

  _test_parameters(&parameters);
}

void test_i16_1_15() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 15,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 0;

  _test_parameters(&parameters);
}

void test_i16_max_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "65535i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "-1",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = -1;

  _test_parameters(&parameters);
}

void test_i16_max_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "65535i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "32767",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 32767;

  _test_parameters(&parameters);
}

void test_i16_max_15() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "65535i16",
        .expected_left
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 15,
        .returned_llvm_ir = "1",
        .expected_returned
        = { .type = { .bit_width = 16, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i16 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i16 = 1;

  _test_parameters(&parameters);
}

void test_i32_0_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 0;

  _test_parameters(&parameters);
}

void test_i32_0_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 0;

  _test_parameters(&parameters);
}

void test_i32_0_31() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 31,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 0;

  _test_parameters(&parameters);
}

void test_i32_1_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "1",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 1;

  _test_parameters(&parameters);
}

void test_i32_1_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 0;

  _test_parameters(&parameters);
}

void test_i32_1_31() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 31,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 0;

  _test_parameters(&parameters);
}

void test_i32_max_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "4294967295i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "-1",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = -1;

  _test_parameters(&parameters);
}

void test_i32_max_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "4294967295i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "2147483647",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 2147483647;

  _test_parameters(&parameters);
}

void test_i32_max_31() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "4294967295i32",
        .expected_left
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 31,
        .returned_llvm_ir = "1",
        .expected_returned
        = { .type = { .bit_width = 32, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i32 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i32 = 1;

  _test_parameters(&parameters);
}

void test_i64_0_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 0;

  _test_parameters(&parameters);
}

void test_i64_0_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 0;

  _test_parameters(&parameters);
}

void test_i64_0_63() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "0i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 63,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = 0;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 0;

  _test_parameters(&parameters);
}

void test_i64_1_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "1",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 1;

  _test_parameters(&parameters);
}

void test_i64_1_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 0;

  _test_parameters(&parameters);
}

void test_i64_1_63() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "1i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 63,
        .returned_llvm_ir = "0",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = 1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 0;

  _test_parameters(&parameters);
}

void test_i64_max_0() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "18446744073709551615i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 0,
        .returned_llvm_ir = "-1",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = -1;

  _test_parameters(&parameters);
}

void test_i64_max_1() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "18446744073709551615i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 1,
        .returned_llvm_ir = "9223372036854775807",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 9223372036854775807L;

  _test_parameters(&parameters);
}

void test_i64_max_63() {
  frg_test_bit_shift_right_parameters_t parameters
    = { .left_text = "18446744073709551615i64",
        .expected_left
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } },
        .right            = 63,
        .returned_llvm_ir = "1",
        .expected_returned
        = { .type = { .bit_width = 64, .flags = FRG_AST_NODE_TYPE_INT_FLAG_NONE } } };

  memset(&parameters.expected_left.value, 0, sizeof(parameters.expected_left.value));
  parameters.expected_left.value.as_i64 = -1;
  memset(
    &parameters.expected_returned.value, 0, sizeof(parameters.expected_returned.value));
  parameters.expected_returned.value.as_i64 = 1;

  _test_parameters(&parameters);
}

void _callback_on_messages_unsupported_type_left(
  const frg_message_buffer_t* message_buffer, void* mut_user_data) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(1, message_buffer->error_count);

  frg_message_query_t query = frg_message_query_empty;
  query.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query.with_code           = "ET-6";

  frg_message_t* message;
  TEST_ASSERT_EQUAL(FRG_MESSAGE_BUFFER_QUERY_RESULT_SINGLE,
                    frg_message_buffer_query_single(&message, message_buffer, &query));

  TEST_ASSERT_EQUAL(2, message->source_range.start.line_number);
  TEST_ASSERT_EQUAL(10, message->source_range.start.column_number);
  TEST_ASSERT_EQUAL(4, message->source_range.length);
  TEST_ASSERT_EQUAL_STRING(
    "Operator >>'s left-hand side must be integer, but is 'bool'", message->text->str);
  TEST_ASSERT_EQUAL(0, g_list_length(message->children));
}

void test_unsupported_type_left() {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_VERIFY;
  options->name        = "bit-shift-right-unsupported-type-left";
  options->on_messages = _callback_on_messages_unsupported_type_left;

  // clang-format off
options->source_text =
  "fn f() -> bool {\n"
  "  return true >> 0u32;\n"
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
  "        value = [value-bit-shift-right]\n"
  "          left = [value-bool]\n"
  "            value = true\n"
  "          right = [value-int]\n"
  "            type = [type-int]\n"
  "              flags = unsigned\n"
  "              bit-width = 32\n"
  "            value = 0u32"
;
  // clang-format on

  // Run test
  //
  // -----------------------------------------------------------------------------------
  frg_testing_test_compilation(options);

  // Cleanup
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_destroy(options);
}

void _callback_on_messages_unsupported_type_right(
  const frg_message_buffer_t* message_buffer, void* mut_user_data) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(1, message_buffer->error_count);

  frg_message_query_t query = frg_message_query_empty;
  query.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query.with_code           = "ET-6";

  frg_message_t* message;
  TEST_ASSERT_EQUAL(FRG_MESSAGE_BUFFER_QUERY_RESULT_SINGLE,
                    frg_message_buffer_query_single(&message, message_buffer, &query));

  TEST_ASSERT_EQUAL(2, message->source_range.start.line_number);
  TEST_ASSERT_EQUAL(17, message->source_range.start.column_number);
  TEST_ASSERT_EQUAL(4, message->source_range.length);
  TEST_ASSERT_EQUAL_STRING("Operator >>'s right-hand side must be 'u32', but is 'i32'",
                           message->text->str);
  TEST_ASSERT_EQUAL(0, g_list_length(message->children));
}

void test_unsupported_type_right() {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_VERIFY;
  options->name        = "bit-shift-right-unsupported-type-right";
  options->on_messages = _callback_on_messages_unsupported_type_right;

  // clang-format off
options->source_text =
  "fn f() -> u8 {\n"
  "  return 0u8 >> 0i32;\n"
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
  "        flags = unsigned\n"
  "        bit-width = 8\n"
  "    body = [statement-block]\n"
  "      statements[0] = [statement-return]\n"
  "        value = [value-bit-shift-right]\n"
  "          left = [value-int]\n"
  "            type = [type-int]\n"
  "              flags = unsigned\n"
  "              bit-width = 8\n"
  "            value = 0u8\n"
  "          right = [value-int]\n"
  "            type = [type-int]\n"
  "              flags = none\n"
  "              bit-width = 32\n"
  "            value = 0i32"
;
  // clang-format on

  // Run test
  //
  // -----------------------------------------------------------------------------------
  frg_testing_test_compilation(options);

  // Cleanup
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_destroy(options);
}

void _callback_on_messages_unsupported_type_both(
  const frg_message_buffer_t* message_buffer, void* mut_user_data) {
  TEST_ASSERT_EQUAL(0, message_buffer->warning_count);
  TEST_ASSERT_EQUAL(2, message_buffer->error_count);

  frg_message_query_t query = frg_message_query_empty;
  query.with_severity       = FRG_MESSAGE_SEVERITY_ERROR;
  query.with_code           = "ET-6";

  GList* messages           = frg_message_buffer_query(message_buffer, &query);
  TEST_ASSERT_EQUAL(2, g_list_length(messages));

  frg_message_t* message_left  = (frg_message_t*)g_list_nth_data(messages, 0);
  frg_message_t* message_right = (frg_message_t*)g_list_nth_data(messages, 1);

  g_list_free(messages);

  TEST_ASSERT_EQUAL(2, message_left->source_range.start.line_number);
  TEST_ASSERT_EQUAL(10, message_left->source_range.start.column_number);
  TEST_ASSERT_EQUAL(4, message_left->source_range.length);
  TEST_ASSERT_EQUAL_STRING(
    "Operator >>'s left-hand side must be integer, but is 'bool'",
    message_left->text->str);
  TEST_ASSERT_EQUAL(0, g_list_length(message_left->children));

  TEST_ASSERT_EQUAL(2, message_right->source_range.start.line_number);
  TEST_ASSERT_EQUAL(18, message_right->source_range.start.column_number);
  TEST_ASSERT_EQUAL(4, message_right->source_range.length);
  TEST_ASSERT_EQUAL_STRING("Operator >>'s right-hand side must be 'u32', but is 'bool'",
                           message_right->text->str);
  TEST_ASSERT_EQUAL(0, g_list_length(message_right->children));
}

void test_unsupported_type_both() {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_VERIFY;
  options->name        = "bit-shift-right-unsupported-type-both";
  options->on_messages = _callback_on_messages_unsupported_type_both;

  // clang-format off
  options->source_text =
    "fn f() -> bool {\n"
    "  return true >> true;\n"
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
    "        value = [value-bit-shift-right]\n"
    "          left = [value-bool]\n"
    "            value = true\n"
    "          right = [value-bool]\n"
    "            value = true"
  ;
  // clang-format on

  // Run test
  //
  // -----------------------------------------------------------------------------------
  frg_testing_test_compilation(options);

  // Cleanup
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_destroy(options);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_u8_0_0);
  RUN_TEST(test_u8_0_1);
  RUN_TEST(test_u8_0_7);
  RUN_TEST(test_u8_1_0);
  RUN_TEST(test_u8_1_1);
  RUN_TEST(test_u8_1_7);
  RUN_TEST(test_u8_max_0);
  RUN_TEST(test_u8_max_1);
  RUN_TEST(test_u8_max_7);
  RUN_TEST(test_u16_0_0);
  RUN_TEST(test_u16_0_1);
  RUN_TEST(test_u16_0_15);
  RUN_TEST(test_u16_1_0);
  RUN_TEST(test_u16_1_1);
  RUN_TEST(test_u16_1_15);
  RUN_TEST(test_u16_max_0);
  RUN_TEST(test_u16_max_1);
  RUN_TEST(test_u16_max_15);
  RUN_TEST(test_u32_0_0);
  RUN_TEST(test_u32_0_1);
  RUN_TEST(test_u32_0_31);
  RUN_TEST(test_u32_1_0);
  RUN_TEST(test_u32_1_1);
  RUN_TEST(test_u32_1_31);
  RUN_TEST(test_u32_max_0);
  RUN_TEST(test_u32_max_1);
  RUN_TEST(test_u32_max_31);
  RUN_TEST(test_u64_0_0);
  RUN_TEST(test_u64_0_1);
  RUN_TEST(test_u64_0_63);
  RUN_TEST(test_u64_1_0);
  RUN_TEST(test_u64_1_1);
  RUN_TEST(test_u64_1_63);
  RUN_TEST(test_u64_max_0);
  RUN_TEST(test_u64_max_1);
  RUN_TEST(test_u64_max_63);
  RUN_TEST(test_i8_0_0);
  RUN_TEST(test_i8_0_1);
  RUN_TEST(test_i8_0_7);
  RUN_TEST(test_i8_1_0);
  RUN_TEST(test_i8_1_1);
  RUN_TEST(test_i8_1_7);
  RUN_TEST(test_i8_max_0);
  RUN_TEST(test_i8_max_1);
  RUN_TEST(test_i8_max_7);
  RUN_TEST(test_i16_0_0);
  RUN_TEST(test_i16_0_1);
  RUN_TEST(test_i16_0_15);
  RUN_TEST(test_i16_1_0);
  RUN_TEST(test_i16_1_1);
  RUN_TEST(test_i16_1_15);
  RUN_TEST(test_i16_max_0);
  RUN_TEST(test_i16_max_1);
  RUN_TEST(test_i16_max_15);
  RUN_TEST(test_i32_0_0);
  RUN_TEST(test_i32_0_1);
  RUN_TEST(test_i32_0_31);
  RUN_TEST(test_i32_1_0);
  RUN_TEST(test_i32_1_1);
  RUN_TEST(test_i32_1_31);
  RUN_TEST(test_i32_max_0);
  RUN_TEST(test_i32_max_1);
  RUN_TEST(test_i32_max_31);
  RUN_TEST(test_i64_0_0);
  RUN_TEST(test_i64_0_1);
  RUN_TEST(test_i64_0_63);
  RUN_TEST(test_i64_1_0);
  RUN_TEST(test_i64_1_1);
  RUN_TEST(test_i64_1_63);
  RUN_TEST(test_i64_max_0);
  RUN_TEST(test_i64_max_1);
  RUN_TEST(test_i64_max_63);
  RUN_TEST(test_unsupported_type_left);
  RUN_TEST(test_unsupported_type_right);
  RUN_TEST(test_unsupported_type_both);
  return UNITY_END();
}
