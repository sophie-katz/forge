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

void frg_ast_destroy_ty_symbol(frg_ast_t* ast);
void frg_ast_destroy_ty_pointer(frg_ast_t* ast);
void frg_ast_destroy_ty_fn(frg_ast_t* ast);
void frg_ast_destroy_decl_union(frg_ast_t* ast);
void frg_ast_destroy_decl_struct(frg_ast_t* ast);
void frg_ast_destroy_decl_prop(frg_ast_t* ast);
void frg_ast_destroy_decl_iface(frg_ast_t* ast);
void frg_ast_destroy_decl_fn_arg(frg_ast_t* ast);
void frg_ast_destroy_decl_fn(frg_ast_t* ast);
void frg_ast_destroy_decl_var(frg_ast_t* ast);
void frg_ast_destroy_decl_block(frg_ast_t* ast);
void frg_ast_destroy_stmt_return(frg_ast_t* ast);
void frg_ast_destroy_stmt_if(frg_ast_t* ast);
void frg_ast_destroy_stmt_while(frg_ast_t* ast);
void frg_ast_destroy_stmt_block(frg_ast_t* ast);
void frg_ast_destroy_value_int(frg_ast_t* ast);
void frg_ast_destroy_value_float(frg_ast_t* ast);
void frg_ast_destroy_value_str(frg_ast_t* ast);
void frg_ast_destroy_value_symbol(frg_ast_t* ast);
void frg_ast_destroy_value_call_kw_arg(frg_ast_t* ast);
void frg_ast_destroy_value_call(frg_ast_t* ast);
void frg_ast_destroy_value_unary(frg_ast_t* ast);
void frg_ast_destroy_value_binary(frg_ast_t* ast);
