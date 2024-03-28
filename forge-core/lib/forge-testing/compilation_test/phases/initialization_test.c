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

#include <forge-testing/compilation_test/phases/initialization.h>
#include <unity.h>

void setUp() {
  frg_stream_output_init();
}

void tearDown() {
  frg_stream_output_cleanup();
}

int callback_counter = 0;

void callback(const frg_message_buffer_t* message_buffer, void* mut_user_data) {
  TEST_ASSERT_NULL(mut_user_data);
  TEST_ASSERT_NOT_NULL(message_buffer);
  callback_counter++;
}

void test_cleanup_successful_no_errors() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;

  callback_counter     = 0;
  options->on_messages = callback;

  char buffer[10];
  memset(buffer, 0, sizeof(buffer));

  frg_stream_input_t* input
    = frg_stream_input_new_buffer(buffer, FRG_STREAM_INPUT_FLAG_NONE);

  frg_parsing_source_t* source = frg_parsing_source_new("test.frg", input);

  frg_parsing_source_context_t* source_context = frg_parsing_source_context_new();
  frg_parsing_source_context_add_source(source_context, source);

  frg_testing_compilation_test_context_initialized_t* context_initialized
    = frg_testing_compilation_test_context_initialized_new(
      g_string_new("test.frg"), source_context, source, frg_message_buffer_new());

  GString* message = frg_testing_test_compilation_phase_cleanup_initialization(
    options, context_initialized);

  TEST_ASSERT_NULL(message);
  TEST_ASSERT_EQUAL(1, callback_counter);

  frg_testing_compilation_test_options_destroy(options);
}

void test_cleanup_unsuccessful_errors() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_PARSE;

  callback_counter     = 0;
  options->on_messages = callback;

  char buffer[10];
  memset(buffer, 0, sizeof(buffer));

  frg_stream_input_t* input
    = frg_stream_input_new_buffer(buffer, FRG_STREAM_INPUT_FLAG_NONE);

  frg_parsing_source_t* source = frg_parsing_source_new("test.frg", input);

  frg_parsing_source_context_t* source_context = frg_parsing_source_context_new();
  frg_parsing_source_context_add_source(source_context, source);

  frg_testing_compilation_test_context_initialized_t* context_initialized
    = frg_testing_compilation_test_context_initialized_new(
      g_string_new("test.frg"), source_context, source, frg_message_buffer_new());

  frg_message_emit(context_initialized->message_buffer,
                   FRG_MESSAGE_SEVERITY_ERROR,
                   "test.frg",
                   "TEST",
                   "test error");

  GString* message = frg_testing_test_compilation_phase_cleanup_initialization(
    options, context_initialized);

  TEST_ASSERT_NULL(message);
  TEST_ASSERT_EQUAL(1, callback_counter);

  frg_testing_compilation_test_options_destroy(options);
}

void test_cleanup_unsuccessful_no_errors() {
  frg_testing_compilation_test_options_t* options
    = frg_testing_compilation_test_options_new_default();

  options->kind        = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_PARSE;

  callback_counter     = 0;
  options->on_messages = callback;

  char buffer[10];
  memset(buffer, 0, sizeof(buffer));

  frg_stream_input_t* input
    = frg_stream_input_new_buffer(buffer, FRG_STREAM_INPUT_FLAG_NONE);

  frg_parsing_source_t* source = frg_parsing_source_new("test.frg", input);

  frg_parsing_source_context_t* source_context = frg_parsing_source_context_new();
  frg_parsing_source_context_add_source(source_context, source);

  frg_testing_compilation_test_context_initialized_t* context_initialized
    = frg_testing_compilation_test_context_initialized_new(
      g_string_new("test.frg"), source_context, source, frg_message_buffer_new());

  GString* message = frg_testing_test_compilation_phase_cleanup_initialization(
    options, context_initialized);

  TEST_ASSERT_EQUAL_STRING("Expected compilation to fail, but no errors were emitted",
                           message->str);
  TEST_ASSERT_EQUAL(0, callback_counter);

  g_string_free(message, TRUE);

  frg_testing_compilation_test_options_destroy(options);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_cleanup_successful_no_errors);
  RUN_TEST(test_cleanup_unsuccessful_errors);
  RUN_TEST(test_cleanup_unsuccessful_no_errors);
  return UNITY_END();
}
