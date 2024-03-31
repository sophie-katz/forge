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

#include <forge/ast/node.h>

bool frg_ast_comparer_always_true(const frg_ast_node_t* a, const frg_ast_node_t* b);
bool frg_ast_comparer_type_int(const frg_ast_node_t* a, const frg_ast_node_t* b);
bool frg_ast_comparer_type_float(const frg_ast_node_t* a, const frg_ast_node_t* b);
bool frg_ast_comparer_type_symbol(const frg_ast_node_t* a, const frg_ast_node_t* b);
bool frg_ast_comparer_type_pointer(const frg_ast_node_t* a, const frg_ast_node_t* b);
bool frg_ast_comparer_type_array(const frg_ast_node_t* a, const frg_ast_node_t* b);
bool frg_ast_comparer_type_function(const frg_ast_node_t* a, const frg_ast_node_t* b);
