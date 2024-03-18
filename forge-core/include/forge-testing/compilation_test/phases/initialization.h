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

#include <forge-testing/compilation_test/options.h>
#include <forge/messages/message_buffer.h>

typedef struct {
  frg_message_buffer_t* message_buffer;
  GString* source_path;
  frg_parsing_source_context_t* source_context;
  frg_parsing_source_t* mut_source;
} frg_testing_compilation_test_context_initialized_t;

frg_testing_compilation_test_context_initialized_t*
  frg_testing_compilation_test_context_initialized_new(
    GString* source_path,
    frg_parsing_source_context_t* source_context,
    frg_parsing_source_t* mut_source,
    frg_message_buffer_t* message_buffer);

void frg_testing_compilation_test_context_initialized_destroy(
  frg_testing_compilation_test_context_initialized_t* context_initialized);

frg_testing_compilation_test_context_initialized_t*
  frg_testing_test_compilation_phase_run_initialization(
    const frg_testing_compilation_test_options_t* options);

GString* frg_testing_test_compilation_phase_cleanup_initialization(
  const frg_testing_compilation_test_options_t* options,
  frg_testing_compilation_test_context_initialized_t* context_initialized);
