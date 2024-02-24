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

#include <forge/ast/node.h>

frg_ast_node_t* frg_ast_cloner_type_bool(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_type_int(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_type_float(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_type_symbol(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_type_pointer(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_type_function(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_declaration_union(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_declaration_structure(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_declaration_property(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_declaration_interface(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_declaration_function_argument(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_declaration_function(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_declaration_variable(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_declaration_block(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_statement_return(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_statement_if(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_statement_while(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_statement_block(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_bool(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_int(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_float(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_character(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_string(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_symbol(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_call_keyword_argument(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_call(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_unary(const frg_ast_node_t* node);
frg_ast_node_t* frg_ast_cloner_value_binary(const frg_ast_node_t* node);
