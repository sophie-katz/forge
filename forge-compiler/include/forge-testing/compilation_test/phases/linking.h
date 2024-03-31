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

#pragma once

#include <forge-testing/compilation_test/phases/codegen.h>
#include <forge-testing/compilation_test/phases/initialization.h>
#include <forge-testing/compilation_test/phases/parsing.h>
#include <forge/linking/configuration.h>

typedef struct {
  void* shared_library_handle;
} frg_testing_compilation_test_context_linked_t;

frg_testing_compilation_test_context_linked_t*
  frg_testing_compilation_test_context_linked_new(void* shared_library_handle);

void frg_testing_compilation_test_context_linked_destroy(
  frg_testing_compilation_test_context_linked_t* context_linked);

frg_testing_compilation_test_context_linked_t*
  frg_testing_test_compilation_phase_run_linking(
    GString** out_message,
    const frg_testing_compilation_test_options_t* options,
    const frg_testing_compilation_test_context_initialized_t* context_initialized,
    const frg_testing_compilation_test_context_codegen_t* context_codegen);
