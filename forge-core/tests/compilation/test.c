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

#include "test.h"
#include <dlfcn.h>
#include <forge/assert.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/utilities.h>
#include <forge/linking/link.h>
#include <forge/memory.h>
#include <forge/parsing/parse.h>
#include <forge/parsing/source_context.h>
#include <unity.h>

void _frg_test_compare_multiline(frg_message_buffer_t* mut_message_buffer,
                                 frg_parsing_source_context_t* mut_source_context,
                                 const char* expected,
                                 const char* actual) {
  if (strcmp(expected, actual) == 0) {
    return;
  }

  gchar** lines_expected      = g_strsplit(expected, "\n", -1);
  gchar** lines_actual        = g_strsplit(actual, "\n", -1);

  gchar** lines_expected_iter = lines_expected;
  gchar** lines_actual_iter   = lines_actual;

  while (true) {
    if (*lines_expected_iter != NULL) {
      if (*lines_actual_iter != NULL) {
        // Haves lines from both

        if (strcmp(*lines_expected_iter, *lines_actual_iter) != 0) {
          printf("Expected | %s\n", *lines_expected_iter);
          printf("Actual   | %s\n", *lines_actual_iter);
        } else {
          printf("         | %s\n", *lines_expected_iter);
        }

        lines_expected_iter++;
        lines_actual_iter++;
      } else {
        // Only have expected line

        printf("Expected | %s\n", *lines_expected_iter);

        lines_expected_iter++;
      }
    } else {
      if (*lines_actual_iter != NULL) {
        // Only have actual line

        printf("Actual   | %s\n", *lines_actual_iter);

        lines_actual_iter++;
      } else {
        // Have lines from neither

        break;
      }
    }
  }

  frg_message_buffer_print(frg_stream_output_get_stderr(),
                           mut_source_context,
                           mut_message_buffer,
                           FRG_MESSAGE_SEVERITY_DEBUG,
                           0);

  TEST_FAIL_MESSAGE("Multiline strings do not match");
}

frg_stream_input_t* _frg_test_compilation_new_stream(const char* source_text) {
  TEST_ASSERT_NOT_NULL(source_text);

  size_t length             = strlen(source_text);

  char* source_buffer       = frg_malloc(length + 2);
  source_buffer[length]     = 0;
  source_buffer[length + 1] = 0;

  memcpy(source_buffer, source_text, length);

  return frg_stream_input_new_buffer_with_length(
    source_buffer,
    length + 2,
    FRG_STREAM_INPUT_FLAG_OWNED | FRG_STREAM_INPUT_FLAG_EXTRA_NULL_BYTE);
}

void frg_test_compilation(const char* name,
                          const char* source_text,
                          const char* ast_debug,
                          const char* llvm_ir,
                          frg_test_compilation_callback callback) {
  TEST_ASSERT_NOT_NULL(source_text);

  GString* source_path = g_string_new(NULL);
  g_string_printf(source_path, "test-compilation-%s.frg", name);
  gchar* current_dir           = NULL;

  // Create source
  frg_parsing_source_t* source = frg_parsing_source_new(
    source_path->str, _frg_test_compilation_new_stream(source_text));

  // Create source context
  frg_parsing_source_context_t* source_context = frg_parsing_source_context_new();
  frg_parsing_source_context_add_source(source_context, source);

  // Create message buffer
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  // Parse
  frg_ast_node_t* node                 = frg_parse(message_buffer, source);

  // Check AST
  if (ast_debug != NULL) {
    if (node == NULL) {
      frg_message_buffer_print(frg_stream_output_get_stderr(),
                               source_context,
                               message_buffer,
                               FRG_MESSAGE_SEVERITY_DEBUG,
                               0);

      TEST_FAIL_MESSAGE("Unexpected error while parsing AST");
    }

    frg_stream_output_t* stream
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

    frg_ast_print_debug(stream, node, &frg_ast_print_debug_options_default);

    GString* ast_debug_actual = frg_stream_output_destroy_take_buffer(stream);

    _frg_test_compare_multiline(
      message_buffer, source_context, ast_debug, ast_debug_actual->str);

    g_string_free(ast_debug_actual, TRUE);
  } else {
    if (node != NULL) {
      frg_message_buffer_print(frg_stream_output_get_stderr(),
                               source_context,
                               message_buffer,
                               FRG_MESSAGE_SEVERITY_DEBUG,
                               0);

      TEST_FAIL_MESSAGE("Unexpected success while parsing AST");
    }
  }

  // Check LLVM IR
  frg_codegen_module_t* codegen_module = NULL;

  if (node != NULL) {
    codegen_module = frg_codegen(node);

    frg_stream_output_t* stream
      = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

    frg_codegen_module_print(stream, codegen_module);

    GString* llvm_ir_actual = frg_stream_output_destroy_take_buffer(stream);

    _frg_test_compare_multiline(
      message_buffer, source_context, llvm_ir, llvm_ir_actual->str);

    g_string_free(llvm_ir_actual, TRUE);
  } else {
    if (callback != NULL) {
      frg_message_buffer_print(frg_stream_output_get_stderr(),
                               source_context,
                               message_buffer,
                               FRG_MESSAGE_SEVERITY_DEBUG,
                               0);

      callback(message_buffer, NULL);
    }
  }

  // Link into shared object
  if (codegen_module != NULL) {
    GString* path_object = g_string_new(NULL);
    current_dir          = g_get_current_dir();
    g_string_printf(path_object, "%s/test-compilation-%s.o", current_dir, name);
    g_free(current_dir);

    if (!frg_codegen_module_write_object_file(
          message_buffer, codegen_module, path_object->str)) {
      frg_message_buffer_print(frg_stream_output_get_stderr(),
                               source_context,
                               message_buffer,
                               FRG_MESSAGE_SEVERITY_DEBUG,
                               0);

      TEST_FAIL_MESSAGE("Unexpected error while writing object file");
    }

    frg_linking_configuration_t* linking_configuration
      = frg_linking_configuration_detect();

    GString* path_shared_object = g_string_new(NULL);
    current_dir                 = g_get_current_dir();
    g_string_printf(path_shared_object, "%s/test-compilation-%s.so", current_dir, name);
    g_free(current_dir);

    GList* objects = NULL;

    objects        = g_list_append(objects, path_object);

    if (!frg_link(message_buffer,
                  linking_configuration,
                  FRG_LINKING_MODE_SHARED_LIBRARY,
                  path_shared_object->str,
                  objects)) {
      frg_message_buffer_print(frg_stream_output_get_stderr(),
                               source_context,
                               message_buffer,
                               FRG_MESSAGE_SEVERITY_DEBUG,
                               0);

      TEST_FAIL_MESSAGE("Unexpected error while linking shared library");
    }

    void* shared_library = dlopen(path_shared_object->str, RTLD_NOW | RTLD_GLOBAL);

    if (shared_library == NULL) {
      GString* message = g_string_new(NULL);
      g_string_printf(message, "dlopen failed: %s", dlerror());
      TEST_FAIL_MESSAGE(message->str);
      g_string_free(message, TRUE);
    }

    if (callback != NULL) {
      frg_message_buffer_print(frg_stream_output_get_stderr(),
                               source_context,
                               message_buffer,
                               FRG_MESSAGE_SEVERITY_DEBUG,
                               0);

      callback(message_buffer, shared_library);
    }

    if (dlclose(shared_library) != 0) {
      GString* message = g_string_new(NULL);
      g_string_printf(message, "dlclose failed: %s", dlerror());
      TEST_FAIL_MESSAGE(message->str);
      g_string_free(message, TRUE);
    }

    g_list_free(objects);
    g_string_free(path_shared_object, TRUE);
    g_string_free(path_object, TRUE);
    frg_linking_configuration_destroy(linking_configuration);
  } else {
    frg_message_buffer_print(frg_stream_output_get_stderr(),
                             source_context,
                             message_buffer,
                             FRG_MESSAGE_SEVERITY_DEBUG,
                             0);

    if (callback != NULL) {
      callback(message_buffer, NULL);
    }
  }

  // Cleanup
  if (codegen_module != NULL) {
    frg_codegen_module_destroy(codegen_module);
  }

  if (node != NULL) {
    frg_ast_node_destroy(node);
  }

  frg_message_buffer_destroy(message_buffer);
  frg_parsing_source_context_destroy(source_context);
  g_string_free(source_path, TRUE);
}

void* frg_test_compilation_get_function(void* mut_shared_library, const char* name) {
  TEST_ASSERT_NOT_NULL(mut_shared_library);
  TEST_ASSERT_NOT_EMPTY(name);

  void* symbol = dlsym(mut_shared_library, name);

  if (symbol == NULL) {
    GString* message = g_string_new(NULL);
    g_string_printf(message, "dlsym failed: %s", dlerror());
    TEST_FAIL_MESSAGE(message->str);
    g_string_free(message, TRUE);
  }

  return symbol;
}
