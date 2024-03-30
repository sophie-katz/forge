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
  bool left;
  bool right;
  bool returned;
} frg_test_logical_or_parameters_t;

void _callback_on_shared_library_handle(void* mut_shared_library_handle,
                                        void* mut_user_data) {
  const frg_test_logical_or_parameters_t* parameters
    = (const frg_test_logical_or_parameters_t*)mut_user_data;

  frg_testing_assert_function_returns_value_bool(
    frg_testing_test_compilation_get_function(mut_shared_library_handle, "f"),
    parameters->returned);
}

void _test_parameters(const frg_test_logical_or_parameters_t* parameters) {
  // Create default options
  // -----------------------------------------------------------------------------------
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  // Configure options
  // -----------------------------------------------------------------------------------
  frg_testing_string_substituter_add_str(
    options->string_substituter, "left", parameters->left ? "true" : "false");
  frg_testing_string_substituter_add_str(
    options->string_substituter, "right", parameters->right ? "true" : "false");
  frg_testing_string_substituter_add_str(
    options->string_substituter, "returned", parameters->returned ? "true" : "false");

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = "logical-or-%(left)-%(right)";
  options->on_messages              = _callback_on_messages_empty;
  options->on_shared_library_handle = _callback_on_shared_library_handle;
  options->mut_user_data            = (void*)parameters;

  // clang-format off
  options->source_text =
    "fn f() -> bool {\n"
    "  return %(left) || %(right);\n"
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
    "        value = [value-logical-or]\n"
    "          left = [value-bool]\n"
    "            value = %(left)\n"
    "          right = [value-bool]\n"
    "            value = %(right)"
  ;

  options->llvm_ir =
    "; ModuleID = 'forge'\n"
    "source_filename = \"forge\"\n"
    "\n"
    "define i1 @f() {\n"
    "entry:\n"
    "  ret i1 %(returned)\n"
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

void test_true_true() {
  frg_test_logical_or_parameters_t parameters
    = { .left = true, .right = true, .returned = true };

  _test_parameters(&parameters);
}

void test_true_false() {
  frg_test_logical_or_parameters_t parameters
    = { .left = true, .right = false, .returned = true };

  _test_parameters(&parameters);
}

void test_false_true() {
  frg_test_logical_or_parameters_t parameters
    = { .left = false, .right = true, .returned = true };

  _test_parameters(&parameters);
}

void test_false_false() {
  frg_test_logical_or_parameters_t parameters
    = { .left = false, .right = false, .returned = false };

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
  TEST_ASSERT_EQUAL(3, message->source_range.length);
  TEST_ASSERT_EQUAL_STRING("Operator ||'s left-hand side must be 'bool', but is 'u8'",
                           message->text->str);
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
  options->name        = "logical-or-unsupported-type-left";
  options->on_messages = _callback_on_messages_unsupported_type_left;

  // clang-format off
  options->source_text =
    "fn f() -> bool {\n"
    "  return 0u8 || true;\n"
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
    "        value = [value-logical-or]\n"
    "          left = [value-int]\n"
    "            type = [type-int]\n"
    "              flags = unsigned\n"
    "              bit-width = 8\n"
    "            value = 0u8\n"
    "          right = [value-bool]\n"
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
  TEST_ASSERT_EQUAL(18, message->source_range.start.column_number);
  TEST_ASSERT_EQUAL(3, message->source_range.length);
  TEST_ASSERT_EQUAL_STRING("Operator ||'s right-hand side must be 'bool', but is 'u8'",
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
  options->name        = "logical-or-unsupported-type-right";
  options->on_messages = _callback_on_messages_unsupported_type_right;

  // clang-format off
  options->source_text =
    "fn f() -> bool {\n"
    "  return true || 0u8;\n"
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
    "        value = [value-logical-or]\n"
    "          left = [value-bool]\n"
    "            value = true\n"
    "          right = [value-int]\n"
    "            type = [type-int]\n"
    "              flags = unsigned\n"
    "              bit-width = 8\n"
    "            value = 0u8"
  ;
  // clang-format on

  // Run test
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
  TEST_ASSERT_EQUAL(3, message_left->source_range.length);
  TEST_ASSERT_EQUAL_STRING("Operator ||'s left-hand side must be 'bool', but is 'u8'",
                           message_left->text->str);
  TEST_ASSERT_EQUAL(0, g_list_length(message_left->children));

  TEST_ASSERT_EQUAL(2, message_right->source_range.start.line_number);
  TEST_ASSERT_EQUAL(17, message_right->source_range.start.column_number);
  TEST_ASSERT_EQUAL(3, message_right->source_range.length);
  TEST_ASSERT_EQUAL_STRING("Operator ||'s right-hand side must be 'bool', but is 'u8'",
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
  options->name        = "logical-or-unsupported-type-both";
  options->on_messages = _callback_on_messages_unsupported_type_both;

  // clang-format off
  options->source_text =
    "fn f() -> bool {\n"
    "  return 0u8 || 0u8;\n"
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
    "        value = [value-logical-or]\n"
    "          left = [value-int]\n"
    "            type = [type-int]\n"
    "              flags = unsigned\n"
    "              bit-width = 8\n"
    "            value = 0u8\n"
    "          right = [value-int]\n"
    "            type = [type-int]\n"
    "              flags = unsigned\n"
    "              bit-width = 8\n"
    "            value = 0u8"
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
  RUN_TEST(test_true_true);
  RUN_TEST(test_true_false);
  RUN_TEST(test_false_true);
  RUN_TEST(test_false_false);
  RUN_TEST(test_unsupported_type_left);
  RUN_TEST(test_unsupported_type_right);
  RUN_TEST(test_unsupported_type_both);
  return UNITY_END();
}
