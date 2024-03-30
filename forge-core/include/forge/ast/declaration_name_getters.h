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

const char* frg_ast_declaration_name_getter_union(const frg_ast_node_t* node);
const char* frg_ast_declaration_name_getter_structure(const frg_ast_node_t* node);
const char* frg_ast_declaration_name_getter_property(const frg_ast_node_t* node);
const char* frg_ast_declaration_name_getter_interface(const frg_ast_node_t* node);
const char* frg_ast_declaration_name_getter_function(const frg_ast_node_t* node);
const char* frg_ast_declaration_name_getter_assignment(const frg_ast_node_t* node);
