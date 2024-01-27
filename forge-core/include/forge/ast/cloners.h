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

frg_ast_t* frg_ast_clone_base(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_ty_symbol(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_ty_pointer(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_ty_fn(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_decl_union(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_decl_struct(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_decl_prop(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_decl_iface(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_decl_fn_arg(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_decl_fn(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_decl_var(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_decl_block(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_stmt_return(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_stmt_if(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_stmt_while(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_stmt_block(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_int(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_float(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_char(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_str(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_symbol(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_call_kw_arg(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_call(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_unary(const frg_ast_t* ast);
frg_ast_t* frg_ast_clone_value_binary(const frg_ast_t* ast);
