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

#include <forge/ast/ast.h>

const char* frg_ast_decl_name_getter_decl_union(const frg_ast_t* ast);
const char* frg_ast_decl_name_getter_decl_struct(const frg_ast_t* ast);
const char* frg_ast_decl_name_getter_decl_prop(const frg_ast_t* ast);
const char* frg_ast_decl_name_getter_decl_iface(const frg_ast_t* ast);
const char* frg_ast_decl_name_getter_decl_fn_arg(const frg_ast_t* ast);
const char* frg_ast_decl_name_getter_decl_fn(const frg_ast_t* ast);
const char* frg_ast_decl_name_getter_decl_var(const frg_ast_t* ast);
