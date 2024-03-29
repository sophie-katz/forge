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

#pragma once

#include <forge-testing/enums.h>
#include <forge-testing/string_substitution.h>
#include <forge/ast/node.h>
#include <forge/messages/message_buffer.h>

typedef void (*frg_testing_compilation_test_callback_on_ast)(
  const frg_ast_node_t* ast_node, void* mut_user_data);

typedef void (*frg_testing_compilation_test_callback_on_messages)(
  const frg_message_buffer_t* message_buffer, void* mut_user_data);

typedef void (*frg_testing_compilation_test_callback_on_shared_library_handle)(
  void* mut_shared_library_handle, void* mut_user_data);

typedef struct {
  frg_testing_compilation_test_kind_t kind;
  const char* name;
  const char* source_text;
  const char* ast_debug;
  const char* llvm_ir;
  frg_testing_compilation_test_callback_on_ast on_ast;
  frg_testing_compilation_test_callback_on_messages on_messages;
  frg_testing_compilation_test_callback_on_shared_library_handle
    on_shared_library_handle;
  void* mut_user_data;
  frg_testing_string_substituter_t* string_substituter;
  GList* _string_substituter_keys;
} frg_testing_compilation_test_options_t;

frg_testing_compilation_test_options_t*
  frg_testing_compilation_test_options_new_default();

void frg_testing_compilation_test_options_destroy(
  frg_testing_compilation_test_options_t* options);

void frg_testing_compilation_test_options_validate(
  const frg_testing_compilation_test_options_t* options);

void frg_testing_compilation_test_options_string_substituter_add_type_int(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_int_t* type);

void frg_testing_compilation_test_options_string_substituter_add_value_int(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const char* text,
  const frg_ast_node_value_int_t* type);

void frg_testing_compilation_test_options_string_substituter_add_type_float(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const frg_ast_node_type_float_t* type);

void frg_testing_compilation_test_options_string_substituter_add_value_float(
  frg_testing_compilation_test_options_t* mut_options,
  const char* basename,
  const char* text,
  const frg_ast_node_value_float_t* type);
