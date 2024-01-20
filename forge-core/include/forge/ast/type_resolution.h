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
#include <forge/ast/scope.h>
#include <forge/messages/message_buffer.h>

frg_ast_t* frg_ast_resolve_decl_type(
    frg_ast_scope_t* scope,
    frg_ast_t* decl
);

frg_ast_t* frg_ast_resolve_value_type(
    frg_ast_scope_t* scope,
    frg_ast_t* value
);
