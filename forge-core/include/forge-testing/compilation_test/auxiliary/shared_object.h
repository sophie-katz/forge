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
#include <forge-testing/compilation_test/phases/linking.h>

GString* frg_testing_test_compilation_auxiliary_shared_object(
  const frg_testing_compilation_test_options_t* options,
  const frg_testing_compilation_test_context_linked_t* context_linked);
