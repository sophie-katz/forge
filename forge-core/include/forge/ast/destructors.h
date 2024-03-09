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

void frg_ast_destructor_type_symbol(frg_ast_node_t* node);
void frg_ast_destructor_type_pointer(frg_ast_node_t* node);
void frg_ast_destructor_type_array(frg_ast_node_t* node);
void frg_ast_destructor_type_function(frg_ast_node_t* node);
void frg_ast_destructor_declaration_union(frg_ast_node_t* node);
void frg_ast_destructor_declaration_structure(frg_ast_node_t* node);
void frg_ast_destructor_declaration_property(frg_ast_node_t* node);
void frg_ast_destructor_declaration_interface(frg_ast_node_t* node);
void frg_ast_destructor_declaration_function_argument(frg_ast_node_t* node);
void frg_ast_destructor_declaration_function(frg_ast_node_t* node);
void frg_ast_destructor_declaration_assignment(frg_ast_node_t* node);
void frg_ast_destructor_declaration_block(frg_ast_node_t* node);
void frg_ast_destructor_statement_return(frg_ast_node_t* node);
void frg_ast_destructor_statement_if_conditional_clause(frg_ast_node_t* node);
void frg_ast_destructor_statement_if(frg_ast_node_t* node);
void frg_ast_destructor_statement_while(frg_ast_node_t* node);
void frg_ast_destructor_statement_block(frg_ast_node_t* node);
void frg_ast_destructor_value_string(frg_ast_node_t* node);
void frg_ast_destructor_value_array(frg_ast_node_t* node);
void frg_ast_destructor_value_array_repeated(frg_ast_node_t* node);
void frg_ast_destructor_value_structure(frg_ast_node_t* node);
void frg_ast_destructor_value_symbol(frg_ast_node_t* node);
void frg_ast_destructor_value_call_keyword_argument(frg_ast_node_t* node);
void frg_ast_destructor_value_call(frg_ast_node_t* node);
void frg_ast_destructor_value_cast(frg_ast_node_t* node);
void frg_ast_destructor_value_unary(frg_ast_node_t* node);
void frg_ast_destructor_value_binary(frg_ast_node_t* node);
