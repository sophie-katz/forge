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
#include <forge/ast/visitor.h>

const char* frg_ast_get_decl_name(const frg_ast_t* ast);

const char* frg_ast_get_decl_fn_arg_name(const frg_ast_t* ast);

void frg_ast_print_debug(
    frg_stream_output_t* stream,
    const frg_ast_t* ast,
    frg_indent_t indent
);

frg_ast_visitor_status_t frg_ast_accept(
    frg_ast_visitor_t* visitor,
    frg_ast_t** ast
);
