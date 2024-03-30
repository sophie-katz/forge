// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#pragma once

#include <forge/ast/scope.h>

frg_ast_node_t* frg_verification_get_numeric_containing_type(const frg_ast_node_t* a,
                                                             const frg_ast_node_t* b);

frg_ast_node_t* frg_verification_resolve_type(frg_message_buffer_t* mut_message_buffer,
                                              const frg_ast_scope_t* scope,
                                              const frg_ast_node_t* node);
