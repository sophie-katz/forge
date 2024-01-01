// Copyright (c) 2023 Sophie Katz
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

#ifdef __cplusplus
extern "C" {
#endif

#include <forge/ast/ast.h>

typedef struct frg_llvm_module_t frg_llvm_module_t;

frg_status_t frg_codegen(frg_llvm_module_t** module, frg_ast_t* ast);

frg_status_t frg_codegen_call_function(
    void* returned_value,
    frg_llvm_module_t* module,
    const char* name,
    GList* pos_args
);

frg_status_t frg_codegen_write_object_file(
    frg_llvm_module_t* module,
    const char* path
);

frg_status_t frg_codegen_destroy_module(frg_llvm_module_t** module);

frg_status_t frg_codegen_print_module(frg_llvm_module_t* module);

#ifdef __cplusplus
}
#endif
