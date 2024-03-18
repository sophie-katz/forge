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
#include <forge-testing/compilation_test/utilities.h>
#include <forge/assert.h>
#include <forge/memory.h>
#include <unity.h>

frg_testing_compilation_test_context_initialized_t*
  frg_testing_compilation_test_context_initialized_new(
    GString* source_path,
    frg_parsing_source_context_t* source_context,
    frg_parsing_source_t* mut_source,
    frg_message_buffer_t* message_buffer) {
  frg_assert_gstring_non_empty(source_path);
  frg_assert_pointer_non_null(source_context);
  frg_assert_pointer_non_null(message_buffer);

  frg_testing_compilation_test_context_initialized_t* context_initialized
    = (frg_testing_compilation_test_context_initialized_t*)frg_malloc(
      sizeof(frg_testing_compilation_test_context_initialized_t));

  context_initialized->source_path    = source_path;
  context_initialized->source_context = source_context;
  context_initialized->mut_source     = mut_source;
  context_initialized->message_buffer = message_buffer;

  return context_initialized;
}

void frg_testing_compilation_test_context_initialized_destroy(
  frg_testing_compilation_test_context_initialized_t* context_initialized) {
  frg_assert_pointer_non_null(context_initialized);

  if (context_initialized->message_buffer != NULL) {
    frg_message_buffer_destroy(context_initialized->message_buffer);
  }

  if (context_initialized->source_context != NULL) {
    frg_parsing_source_context_destroy(context_initialized->source_context);
  }

  if (context_initialized->source_path != NULL) {
    g_string_free(context_initialized->source_path, TRUE);
  }

  frg_free(context_initialized);
}

GString* _frg_testing_test_initialization_phase_parse_format_source_path(
  const frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);
  frg_assert_string_non_empty(options->name);

  GString* source_path = g_string_new(NULL);
  g_string_printf(source_path, "test-compilation-%s.frg", options->name);
  return source_path;
}

frg_stream_input_t* _frg_testing_test_initialization_phase_parse_create_input_stream(
  const frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(options->source_text);

  size_t length             = strlen(options->source_text);

  char* source_buffer       = frg_malloc(length + 2);
  source_buffer[length]     = 0;
  source_buffer[length + 1] = 0;

  memcpy(source_buffer, options->source_text, length);

  return frg_stream_input_new_buffer_with_length(
    source_buffer,
    length + 2,
    FRG_STREAM_INPUT_FLAG_OWNED | FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE);
}

frg_testing_compilation_test_context_initialized_t*
  frg_testing_test_compilation_phase_run_initialization(
    const frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);

  GString* source_path
    = _frg_testing_test_initialization_phase_parse_format_source_path(options);

  frg_stream_input_t* input
    = _frg_testing_test_initialization_phase_parse_create_input_stream(options);

  frg_parsing_source_t* source = frg_parsing_source_new(source_path->str, input);

  frg_parsing_source_context_t* source_context = frg_parsing_source_context_new();
  frg_parsing_source_context_add_source(source_context, source);

  return frg_testing_compilation_test_context_initialized_new(
    source_path, source_context, source, frg_message_buffer_new());
}

GString* frg_testing_test_compilation_phase_cleanup_initialization(
  const frg_testing_compilation_test_options_t* options,
  frg_testing_compilation_test_context_initialized_t* context_initialized) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(context_initialized);
  frg_assert_pointer_non_null(context_initialized->source_context);
  frg_assert_pointer_non_null(context_initialized->message_buffer);

  // Print messages if any
  if (context_initialized->message_buffer->message_count > 0) {
    frg_testing_compilation_print_header("Messages after compilation is complete");

    frg_message_buffer_print(frg_stream_output_get_stderr(),
                             context_initialized->source_context,
                             context_initialized->message_buffer,
                             FRG_MESSAGE_SEVERITY_DEBUG,
                             0);
  }

  // Check for common issues
  if (options->kind != FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS
      && context_initialized->message_buffer->error_count == 0) {
    frg_testing_compilation_test_context_initialized_destroy(context_initialized);
    return g_string_new("Expected compilation to fail, but no errors were emitted");
  }

  // Run message callback if set
  if (options->on_messages != NULL) {
    options->on_messages(context_initialized->message_buffer);
  }

  // Cleanup memory
  frg_testing_compilation_test_context_initialized_destroy(context_initialized);

  return NULL;
}
