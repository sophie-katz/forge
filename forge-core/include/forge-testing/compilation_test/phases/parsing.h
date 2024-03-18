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
#include <forge-testing/compilation_test/phases/initialization.h>
#include <forge/ast/node.h>
#include <glib.h>

typedef struct {
  frg_ast_node_t* ast_node;
} frg_testing_compilation_test_context_parsed_t;

frg_testing_compilation_test_context_parsed_t*
  frg_testing_compilation_test_context_parsed_new(frg_ast_node_t* ast_node);

void frg_testing_compilation_test_context_parsed_destroy(
  frg_testing_compilation_test_context_parsed_t* context_parsed);

frg_testing_compilation_test_context_parsed_t*
  frg_testing_test_compilation_phase_run_parsing(
    const frg_testing_compilation_test_context_initialized_t* context_initialized);
