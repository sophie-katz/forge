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

#include <forge/codegen/codegen.h>

typedef void (*frg_test_compilation_callback)(
    const frg_message_buffer_t* message_buffer,
    void* mut_shared_library
);

void frg_test_compilation(
    const char* name,
    const char* source_text,
    const char* ast_debug,
    const char* llvm_ir,
    frg_test_compilation_callback callback
);

void* frg_test_compilation_get_function(
    void* mut_shared_library,
    const char* name
);
