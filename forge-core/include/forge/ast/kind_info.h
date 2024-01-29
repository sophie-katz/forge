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
#include <forge/ast/scope.h>

typedef void (*frg_ast_destructor_t)(frg_ast_t* ast);
typedef frg_ast_t* (*frg_ast_cloner_t)(const frg_ast_t* ast);
typedef const char* (*frg_ast_decl_name_getter_t)(const frg_ast_t* ast);
typedef void (*frg_ast_debug_printer_t)(
    frg_stream_output_t* stream,
    const frg_ast_t* ast,
    frg_indent_t indent
);
typedef frg_ast_visitor_status_t (*frg_ast_visitor_acceptor_t)(
    frg_ast_visitor_t* visitor,
    GList* parents,
    frg_ast_t** ast
);
typedef frg_ast_t* (*frg_ast_type_resolver_t)(
    frg_message_buffer_t* message_buffer,
    const frg_ast_scope_t* scope,
    const frg_ast_t* ast
);

typedef struct {
    const char* name;
    frg_ast_kind_flags_t flags;
    frg_bit_width_t ty_bit_width;
    frg_ast_destructor_t destructor;
    frg_ast_cloner_t cloner;
    frg_ast_decl_name_getter_t decl_name_getter;
    frg_ast_debug_printer_t debug_printer;
    frg_ast_visitor_acceptor_t visitor_acceptor;
    frg_ast_type_resolver_t type_resolver;
} frg_ast_kind_info_t;

const frg_ast_kind_info_t* frg_ast_kind_info_get(frg_ast_kind_t kind);
