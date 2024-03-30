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

#include <dlfcn.h>
#include <forge-testing/compilation_test/auxiliary/ast.h>
#include <forge-testing/compilation_test/auxiliary/llvm_ir.h>
#include <forge-testing/compilation_test/auxiliary/shared_object.h>
#include <forge-testing/compilation_test/auxiliary/verification.h>
#include <forge-testing/compilation_test/phases/codegen.h>
#include <forge-testing/compilation_test/phases/initialization.h>
#include <forge-testing/compilation_test/phases/linking.h>
#include <forge-testing/compilation_test/phases/parsing.h>
#include <forge-testing/compilation_test/test.h>
#include <forge-testing/compilation_test/utilities.h>
#include <forge/assert.h>
#include <forge/ast/print_debug.h>
#include <forge/ast/utilities.h>
#include <unity.h>

GString* _frg_testing_test_compilation_check_initialization_early_exit(
  const frg_testing_compilation_test_context_initialized_t* context_initialized) {
  if (context_initialized == NULL) {
    return g_string_new("Initialization failed");
  }

  return NULL;
}

GString* _frg_testing_test_compilation_check_parsing_early_exit(
  const frg_testing_compilation_test_options_t* options,
  const frg_testing_compilation_test_context_initialized_t* context_initialized,
  const frg_testing_compilation_test_context_parsed_t* context_parsed) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(context_initialized);

  if (options->kind == FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_PARSE) {
    if (context_parsed != NULL) {
      frg_testing_compilation_print_header("AST");

      frg_ast_print_debug(frg_stream_output_get_stderr(),
                          context_parsed->ast_node,
                          &frg_ast_print_debug_options_default);

      return g_string_new("Parsing succeeded unexpectedly");
    }
  } else {
    if (context_parsed == NULL) {
      frg_testing_compilation_print_header("Messages from parsing");

      frg_message_buffer_print(frg_stream_output_get_stderr(),
                               context_initialized->source_context,
                               context_initialized->message_buffer,
                               FRG_MESSAGE_SEVERITY_DEBUG,
                               0);

      return g_string_new("Parsing failed unexpectedly");
    }
  }

  return NULL;
}

GString* _frg_testing_test_compilation_check_verification_early_exit(
  const frg_testing_compilation_test_options_t* options,
  const frg_testing_compilation_test_context_initialized_t* context_initialized,
  const GString* verification_message) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(context_initialized);

  if (options->kind == FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_UNABLE_TO_VERIFY) {
    if (context_initialized->message_buffer->error_count == 0
        && verification_message == NULL) {
      return g_string_new("Verification succeeded unexpectedly");
    } else {
      return NULL;
    }
  } else {
    if (verification_message != NULL) {
      GString* message = g_string_new(NULL);
      g_string_printf(message,
                      "Verification failed unexpectedly with message: %s",
                      verification_message->str);
      return message;
    } else {
      return NULL;
    }
  }
}

GString* _frg_testing_test_compilation_check_codegen_early_exit(
  const frg_testing_compilation_test_context_parsed_t* context_parsed,
  const frg_testing_compilation_test_context_codegen_t* context_codegen) {
  frg_assert_pointer_non_null(context_parsed);

  if (context_codegen == NULL) {
    frg_testing_compilation_print_header("AST");

    frg_ast_print_debug(frg_stream_output_get_stderr(),
                        context_parsed->ast_node,
                        &frg_ast_print_debug_options_default);

    return g_string_new("Codegen failed unexpectedly");
  }

  return NULL;
}

GString* _frg_testing_test_compilation_check_linking_early_exit(
  const frg_testing_compilation_test_context_parsed_t* context_parsed,
  const frg_testing_compilation_test_context_codegen_t* context_codegen,
  const frg_testing_compilation_test_context_linked_t* context_linked) {
  frg_assert_pointer_non_null(context_parsed);
  frg_assert_pointer_non_null(context_codegen);

  if (context_linked == NULL) {
    frg_testing_compilation_print_header("AST");

    frg_ast_print_debug(frg_stream_output_get_stderr(),
                        context_parsed->ast_node,
                        &frg_ast_print_debug_options_default);

    frg_testing_compilation_print_header("LLVM IR");

    frg_codegen_module_print(frg_stream_output_get_stderr(),
                             context_codegen->codegen_module);

    return g_string_new("Linking failed unexpectedly");
  }

  return NULL;
}

GString* frg_testing_test_compilation_take_message(
  const frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);

  // Validate options
  frg_testing_compilation_test_options_validate(options);

  // Run phase: initialization
  frg_testing_compilation_test_context_initialized_t* context_initialized
    = frg_testing_test_compilation_phase_run_initialization(options);

  // Early exit if initialization failed
  GString* message = _frg_testing_test_compilation_check_initialization_early_exit(
    context_initialized);
  if (message != NULL) {
    if (context_initialized != NULL) {
      frg_testing_compilation_test_context_initialized_destroy(context_initialized);
    }

    return message;
  }

  // Run phase: parsing
  frg_testing_compilation_test_context_parsed_t* context_parsed
    = frg_testing_test_compilation_phase_run_parsing(context_initialized);

  // Early exit if parsing failed positively or negatively
  message = _frg_testing_test_compilation_check_parsing_early_exit(
    options, context_initialized, context_parsed);
  if (message != NULL) {
    if (context_initialized != NULL) {
      frg_testing_compilation_test_context_parsed_destroy(context_parsed);
    }

    frg_testing_compilation_test_context_initialized_destroy(context_initialized);
    return message;
  }

  // Run auxiliary tests: AST
  message = frg_testing_test_compilation_auxiliary_ast(options, context_parsed);
  if (message != NULL) {
    frg_testing_compilation_test_context_parsed_destroy(context_parsed);
    frg_testing_compilation_test_context_initialized_destroy(context_initialized);
    return message;
  }

  // Run auxiliary tests: verification
  GString* verification_message = frg_testing_test_compilation_auxiliary_verification(
    options, context_initialized, context_parsed);

  // Early exit if verification failed positively or negatively
  message = _frg_testing_test_compilation_check_verification_early_exit(
    options, context_initialized, verification_message);
  if (verification_message != NULL) {
    g_string_free(verification_message, TRUE);
  }

  if (message != NULL) {
    frg_testing_compilation_test_context_parsed_destroy(context_parsed);
    frg_testing_compilation_test_context_initialized_destroy(context_initialized);
    return message;
  }

  if (context_initialized->message_buffer->error_count == 0) {
    // Run phase: codegen
    frg_testing_compilation_test_context_codegen_t* context_codegen
      = frg_testing_test_compilation_phase_run_codegen(context_parsed);

    // Early exit if codegen failed
    message = _frg_testing_test_compilation_check_codegen_early_exit(context_parsed,
                                                                     context_codegen);
    if (message != NULL) {
      if (context_codegen != NULL) {
        frg_testing_compilation_test_context_codegen_destroy(context_codegen);
      }

      frg_testing_compilation_test_context_parsed_destroy(context_parsed);
      frg_testing_compilation_test_context_initialized_destroy(context_initialized);

      return message;
    }

    // Run auxiliary tests: LLVM IR
    message = frg_testing_test_compilation_auxiliary_llvm_ir(options, context_codegen);
    if (message != NULL) {
      frg_testing_compilation_test_context_codegen_destroy(context_codegen);
      frg_testing_compilation_test_context_parsed_destroy(context_parsed);
      frg_testing_compilation_test_context_initialized_destroy(context_initialized);
      return message;
    }

    // Run phase: linking
    frg_testing_compilation_test_context_linked_t* context_linked
      = frg_testing_test_compilation_phase_run_linking(
        &message, options, context_initialized, context_codegen);
    if (message != NULL) {
      if (context_linked != NULL) {
        frg_testing_compilation_test_context_linked_destroy(context_linked);
      }

      frg_testing_compilation_test_context_codegen_destroy(context_codegen);
      frg_testing_compilation_test_context_parsed_destroy(context_parsed);
      frg_testing_compilation_test_context_initialized_destroy(context_initialized);

      return message;
    }

    // Early exit if linking failed
    message = _frg_testing_test_compilation_check_linking_early_exit(
      context_parsed, context_codegen, context_linked);
    if (message != NULL) {
      if (context_linked != NULL) {
        frg_testing_compilation_test_context_linked_destroy(context_linked);
      }

      frg_testing_compilation_test_context_codegen_destroy(context_codegen);
      frg_testing_compilation_test_context_parsed_destroy(context_parsed);
      frg_testing_compilation_test_context_initialized_destroy(context_initialized);

      return message;
    }

    // Run auxiliary tests: shared object
    message
      = frg_testing_test_compilation_auxiliary_shared_object(options, context_linked);
    if (message != NULL) {
      frg_testing_compilation_test_context_linked_destroy(context_linked);
      frg_testing_compilation_test_context_codegen_destroy(context_codegen);
      frg_testing_compilation_test_context_parsed_destroy(context_parsed);
      frg_testing_compilation_test_context_initialized_destroy(context_initialized);
      return message;
    }

    // Cleanup phase: linking
    if (context_linked != NULL) {
      frg_testing_compilation_test_context_linked_destroy(context_linked);
    }

    // Cleanup phase: codegen
    if (context_codegen != NULL) {
      frg_testing_compilation_test_context_codegen_destroy(context_codegen);
    }
  }

  // Cleanup phase: parsing
  if (context_parsed != NULL) {
    frg_testing_compilation_test_context_parsed_destroy(context_parsed);
  }

  // Cleanup phase: initialization
  message = frg_testing_test_compilation_phase_cleanup_initialization(
    options, context_initialized);
  if (message != NULL) {
    return message;
  }

  return NULL;
}

void frg_testing_test_compilation(
  const frg_testing_compilation_test_options_t* options) {
  GString* message = frg_testing_test_compilation_take_message(options);

  if (message != NULL) {
    TEST_FAIL_MESSAGE(message->str);
    g_string_free(message, TRUE);
  }
}

void* frg_testing_test_compilation_get_function(void* mut_shared_library_handle,
                                                const char* name) {
  frg_assert_pointer_non_null(mut_shared_library_handle);
  frg_assert_string_non_empty(name);

  void* symbol = dlsym(mut_shared_library_handle, name);

  if (symbol == NULL) {
    GString* message = g_string_new(NULL);
    g_string_printf(message, "dlsym failed: %s", dlerror());
    TEST_FAIL_MESSAGE(message->str);
    g_string_free(message, TRUE);
  }

  return symbol;
}

void frg_testing_assert_function_returns_value_bool(void* function, bool expected) {
  uint32_t actual = ((uint32_t(*)())function)();
  TEST_ASSERT_EQUAL(expected ? 1 : 0, actual != 0 ? 1 : 0);
}

void frg_testing_assert_function_returns_value_int(
  void* function, const frg_ast_node_value_int_t* expected) {
  switch (frg_get_case_for_type_int(&expected->type)) {
  case frg_get_case_for_int_attributes(true, 8):
    TEST_ASSERT_EQUAL(expected->value.as_i8, ((int8_t(*)())function)());
    break;
  case frg_get_case_for_int_attributes(true, 16):
    TEST_ASSERT_EQUAL(expected->value.as_i16, ((int16_t(*)())function)());
    break;
  case frg_get_case_for_int_attributes(true, 32):
    TEST_ASSERT_EQUAL(expected->value.as_i32, ((int32_t(*)())function)());
    break;
  case frg_get_case_for_int_attributes(true, 64):
    TEST_ASSERT_EQUAL(expected->value.as_i64, ((int64_t(*)())function)());
    break;
  case frg_get_case_for_int_attributes(false, 8):
    TEST_ASSERT_EQUAL(expected->value.as_u8, ((uint8_t(*)())function)());
    break;
  case frg_get_case_for_int_attributes(false, 16):
    TEST_ASSERT_EQUAL(expected->value.as_u16, ((uint16_t(*)())function)());
    break;
  case frg_get_case_for_int_attributes(false, 32):
    TEST_ASSERT_EQUAL(expected->value.as_u32, ((uint32_t(*)())function)());
    break;
  case frg_get_case_for_int_attributes(false, 64):
    TEST_ASSERT_EQUAL(expected->value.as_u64, ((uint64_t(*)())function)());
    break;
  default:
    frg_die("Unexpected bit width");
  }
}

void frg_testing_assert_function_returns_value_float(
  void* function, const frg_ast_node_value_float_t* expected) {
  switch (expected->type.bit_width) {
  case 32:
    TEST_ASSERT_EQUAL(expected->value.as_f32, ((frg_f32_t(*)())function)());
    break;
  case 64:
    TEST_ASSERT_EQUAL(expected->value.as_f64, ((frg_f64_t(*)())function)());
    break;
  default:
    frg_die("Unexpected bit width");
  }
}
