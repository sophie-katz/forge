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

#include <forge-testing/compilation_test/options.h>
#include <forge/assert.h>
#include <forge/ast/utilities.h>
#include <forge/memory.h>

frg_testing_compilation_test_options_t*
  frg_testing_compilation_test_options_new_default() {
  frg_testing_compilation_test_options_t* options
    = (frg_testing_compilation_test_options_t*)frg_malloc(
      sizeof(frg_testing_compilation_test_options_t));

  options->kind                     = FRG_TESTING_COMPILATION_TEST_KIND_EXPECT_SUCCESS;
  options->name                     = NULL;
  options->source_text              = NULL;
  options->ast_debug                = NULL;
  options->llvm_ir                  = NULL;
  options->on_ast                   = NULL;
  options->on_messages              = NULL;
  options->on_shared_library_handle = NULL;
  options->mut_user_data            = NULL;
  options->string_substituter       = frg_testing_string_substituter_new();
  options->_string_substituter_keys = NULL;

  return options;
}

void frg_testing_compilation_test_options_destroy(
  frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);

  frg_testing_string_substituter_destroy(options->string_substituter);

  for (GList* string_substiter_keys_iter = options->_string_substituter_keys;
       string_substiter_keys_iter != NULL;
       string_substiter_keys_iter = string_substiter_keys_iter->next) {
    g_string_free((GString*)string_substiter_keys_iter->data, TRUE);
  }

  g_list_free(options->_string_substituter_keys);

  frg_free(options);
}

void frg_testing_compilation_test_options_validate(
  const frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);
  frg_assert_string_non_empty(options->name);

  // TODO: Have this run after substitution
  // for (const char* name_iter = options->name; *name_iter != 0; name_iter++) {
  //   frg_assert_with_message(
  //     *name_iter == '-' || (*name_iter >= 'a' && *name_iter <= 'z')
  //       || (*name_iter >= '0' && *name_iter <= '9'),
  //     "test names must be kebab case with only lowercase ASCII letters and numbers");
  // }

  frg_assert_pointer_non_null(options->source_text);
}

void _frg_testing_compilation_test_options_string_substituter_add_type_int_type(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_int_t* type) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(type);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-keyword", basename);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_type_int_print(stream, type);

  frg_testing_string_substituter_add_string(
    mut_options->string_substituter,
    key->str,
    frg_stream_output_destroy_take_buffer(stream));

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void _frg_testing_compilation_test_options_string_substituter_add_type_int_flags(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_int_t* type) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(type);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-flags", basename);

  frg_testing_string_substituter_add_str(
    mut_options->string_substituter,
    key->str,
    (type->flags & FRG_AST_NODE_TYPE_INT_FLAG_UNSIGNED) ? "unsigned" : "none");

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void _frg_testing_compilation_test_options_string_substituter_add_type_int_bit_width(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_int_t* type) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(type);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-bit-width", basename);

  frg_testing_string_substituter_add_printf(
    mut_options->string_substituter, key->str, "%u", type->bit_width);

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void frg_testing_compilation_test_options_string_substituter_add_type_int(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_int_t* type) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(type);

  _frg_testing_compilation_test_options_string_substituter_add_type_int_type(
    mut_options, basename, type);
  _frg_testing_compilation_test_options_string_substituter_add_type_int_flags(
    mut_options, basename, type);
  _frg_testing_compilation_test_options_string_substituter_add_type_int_bit_width(
    mut_options, basename, type);
}

void _frg_testing_compilation_test_options_string_substituter_add_value_int_type(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_value_int_t* value) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(value);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-type", basename);

  frg_testing_compilation_test_options_string_substituter_add_type_int(
    mut_options, key->str, &value->type);

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void _frg_testing_compilation_test_options_string_substituter_add_value_int_text(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const char* text) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_string_non_empty(text);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-text", basename);

  frg_testing_string_substituter_add_str(
    mut_options->string_substituter, key->str, text);

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void _frg_testing_compilation_test_options_string_substituter_add_value_int_value_unsigned(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_value_int_t* value) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(value);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-unsigned", basename);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_value_int_print_unsigned(stream, value, 10, 0);

  frg_testing_string_substituter_add_string(
    mut_options->string_substituter,
    key->str,
    frg_stream_output_destroy_take_buffer(stream));

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void _frg_testing_compilation_test_options_string_substituter_add_value_int_value_signed(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_value_int_t* value) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(value);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-signed", basename);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_value_int_print_signed(stream, value, 10, 0);

  frg_testing_string_substituter_add_string(
    mut_options->string_substituter,
    key->str,
    frg_stream_output_destroy_take_buffer(stream));

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void _frg_testing_compilation_test_options_string_substituter_add_value_int_value(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_value_int_t* value) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(value);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_value_int_print(stream, value, 10, 0);

  frg_testing_string_substituter_add_string(
    mut_options->string_substituter,
    basename,
    frg_stream_output_destroy_take_buffer(stream));
}

void frg_testing_compilation_test_options_string_substituter_add_value_int(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const char* text,
  const frg_ast_node_value_int_t* value) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);

  if (text != NULL) {
    _frg_testing_compilation_test_options_string_substituter_add_value_int_text(
      mut_options, basename, text);
  }

  if (value != NULL) {
    _frg_testing_compilation_test_options_string_substituter_add_value_int_type(
      mut_options, basename, value);
    _frg_testing_compilation_test_options_string_substituter_add_value_int_value_unsigned(
      mut_options, basename, value);
    _frg_testing_compilation_test_options_string_substituter_add_value_int_value_signed(
      mut_options, basename, value);
    _frg_testing_compilation_test_options_string_substituter_add_value_int_value(
      mut_options, basename, value);
  }
}

void _frg_testing_compilation_test_options_string_substituter_add_type_float_type(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_float_t* type) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(type);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-keyword", basename);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_type_float_print(stream, type);

  frg_testing_string_substituter_add_string(
    mut_options->string_substituter,
    key->str,
    frg_stream_output_destroy_take_buffer(stream));

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void _frg_testing_compilation_test_options_string_substituter_add_type_float_bit_width(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_float_t* type) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(type);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-bit-width", basename);

  frg_testing_string_substituter_add_printf(
    mut_options->string_substituter, key->str, "%u", type->bit_width);

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void frg_testing_compilation_test_options_string_substituter_add_type_float(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_float_t* type) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(type);

  _frg_testing_compilation_test_options_string_substituter_add_type_float_type(
    mut_options, basename, type);
  _frg_testing_compilation_test_options_string_substituter_add_type_float_bit_width(
    mut_options, basename, type);
}

void _frg_testing_compilation_test_options_string_substituter_add_value_float_value(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_value_float_t* value) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_pointer_non_null(value);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_ast_value_float_print(stream, value, 10, 0);

  frg_testing_string_substituter_add_string(
    mut_options->string_substituter,
    basename,
    frg_stream_output_destroy_take_buffer(stream));
}

void _frg_testing_compilation_test_options_string_substituter_add_value_float_text(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const char* text) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);
  frg_assert_string_non_empty(text);

  GString* key = g_string_new(NULL);
  g_string_printf(key, "%s-text", basename);

  frg_testing_string_substituter_add_str(
    mut_options->string_substituter, key->str, text);

  mut_options->_string_substituter_keys
    = g_list_append(mut_options->_string_substituter_keys, key);
}

void frg_testing_compilation_test_options_string_substituter_add_value_float(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const char* text,
  const frg_ast_node_value_float_t* value) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_string_non_empty(basename);

  if (text != NULL) {
    _frg_testing_compilation_test_options_string_substituter_add_value_float_text(
      mut_options, basename, text);
  }

  if (value != NULL) {
    _frg_testing_compilation_test_options_string_substituter_add_value_float_value(
      mut_options, basename, value);
  }
}
