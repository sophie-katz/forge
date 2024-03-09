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

#include <forge/assert.h>
#include <forge/ast/cloners.h>
#include <forge/ast/debug_printers.h>
#include <forge/ast/declaration_name_getters.h>
#include <forge/ast/destructors.h>
#include <forge/ast/node_kind_info.h>
#include <forge/ast/visitor_acceptors.h>
#include <forge/formatting/formatted_printers.h>
#include <forge/verification/type_resolvers.h>

bool _frg_ast_node_kind_info_table_initialized = false;
frg_ast_node_kind_info_t _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_COUNT];

void _frg_ast_node_kind_info_table_init() {
  frg_assert(!_frg_ast_node_kind_info_table_initialized);

  memset(_frg_ast_node_kind_info_table, 0, sizeof(_frg_ast_node_kind_info_table));

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_VOID].name = "type-void";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_VOID].flags
    = FRG_AST_NODE_KIND_FLAG_TYPE | FRG_AST_NODE_KIND_FLAG_TYPE_PRIMARY;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_VOID]._cloner
    = frg_ast_cloner_type_primary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_VOID]._formatted_printer
    = frg_formatting_formatted_printer_type_void;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_BOOL].name = "type-bool";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_BOOL].flags
    = FRG_AST_NODE_KIND_FLAG_TYPE | FRG_AST_NODE_KIND_FLAG_TYPE_PRIMARY;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_BOOL]._cloner
    = frg_ast_cloner_type_primary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_BOOL]._formatted_printer
    = frg_formatting_formatted_printer_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_INT].name = "type-int";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_INT].flags
    = FRG_AST_NODE_KIND_FLAG_TYPE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_INT]._cloner
    = frg_ast_cloner_type_int;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_INT]._debug_printer
    = frg_ast_debug_printer_type_int;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_INT]._formatted_printer
    = frg_formatting_formatted_printer_type_int;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FLOAT].name = "type-float";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FLOAT].flags
    = FRG_AST_NODE_KIND_FLAG_TYPE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FLOAT]._cloner
    = frg_ast_cloner_type_float;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FLOAT]._debug_printer
    = frg_ast_debug_printer_type_float;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FLOAT]._formatted_printer
    = frg_formatting_formatted_printer_type_float;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_SYMBOL].name = "type-symbol";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_SYMBOL].flags
    = FRG_AST_NODE_KIND_FLAG_TYPE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_SYMBOL]._destructor
    = frg_ast_destructor_type_symbol;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_SYMBOL]._cloner
    = frg_ast_cloner_type_symbol;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_SYMBOL]._debug_printer
    = frg_ast_debug_printer_type_symbol;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_SYMBOL]._formatted_printer
    = frg_formatting_formatted_printer_type_symbol;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_POINTER].name = "type-pointer";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_POINTER].flags
    = FRG_AST_NODE_KIND_FLAG_TYPE | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_POINTER]._destructor
    = frg_ast_destructor_type_pointer;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_POINTER]._cloner
    = frg_ast_cloner_type_pointer;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_POINTER]._debug_printer
    = frg_ast_debug_printer_type_pointer;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_POINTER]._visitor_acceptor
    = frg_ast_visitor_acceptor_type_pointer;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_POINTER]._formatted_printer
    = frg_formatting_formatted_printer_type_pointer;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_ARRAY].name = "type-array";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_ARRAY].flags
    = FRG_AST_NODE_KIND_FLAG_TYPE | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_ARRAY]._destructor
    = frg_ast_destructor_type_array;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_ARRAY]._cloner
    = frg_ast_cloner_type_array;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_ARRAY]._debug_printer
    = frg_ast_debug_printer_type_array;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_ARRAY]._visitor_acceptor
    = frg_ast_visitor_acceptor_type_array;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_ARRAY]._formatted_printer
    = frg_formatting_formatted_printer_type_array;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FUNCTION].name = "type-function";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FUNCTION].flags
    = FRG_AST_NODE_KIND_FLAG_TYPE | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FUNCTION]._destructor
    = frg_ast_destructor_type_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FUNCTION]._cloner
    = frg_ast_cloner_type_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FUNCTION]._debug_printer
    = frg_ast_debug_printer_type_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FUNCTION]._visitor_acceptor
    = frg_ast_visitor_acceptor_type_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_TYPE_FUNCTION]._formatted_printer
    = frg_formatting_formatted_printer_type_function;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_UNION].name
    = "declaration-union";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_UNION].flags
    = FRG_AST_NODE_KIND_FLAG_DECLARATION | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_UNION]._destructor
    = frg_ast_destructor_declaration_union;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_UNION]._cloner
    = frg_ast_cloner_declaration_union;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_UNION]
    ._declaration_name_getter
    = frg_ast_declaration_name_getter_union;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_UNION]._debug_printer
    = frg_ast_debug_printer_declaration_union;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_UNION]._visitor_acceptor
    = frg_ast_visitor_acceptor_declaration_union;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_UNION]._type_resolver
    = frg_verification_type_resolver_declaration_union;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_STRUCTURE].name
    = "declaration-structure";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_STRUCTURE].flags
    = FRG_AST_NODE_KIND_FLAG_DECLARATION | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_STRUCTURE]._destructor
    = frg_ast_destructor_declaration_structure;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_STRUCTURE]._cloner
    = frg_ast_cloner_declaration_structure;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_STRUCTURE]
    ._declaration_name_getter
    = frg_ast_declaration_name_getter_structure;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_STRUCTURE]._debug_printer
    = frg_ast_debug_printer_declaration_structure;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_STRUCTURE]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_declaration_structure;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_STRUCTURE]._type_resolver
    = frg_verification_type_resolver_declaration_structure;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY].name
    = "declaration-property";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY].flags
    = FRG_AST_NODE_KIND_FLAG_DECLARATION | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY]._destructor
    = frg_ast_destructor_declaration_property;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY]._cloner
    = frg_ast_cloner_declaration_property;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY]
    ._declaration_name_getter
    = frg_ast_declaration_name_getter_property;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY]._debug_printer
    = frg_ast_debug_printer_declaration_property;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_declaration_property;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY]
    ._formatted_printer
    = frg_formatting_formatted_printer_declaration_property;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_PROPERTY]._type_resolver
    = frg_verification_type_resolver_declaration_property;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_INTERFACE].name
    = "declaration-interface";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_INTERFACE].flags
    = FRG_AST_NODE_KIND_FLAG_DECLARATION | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_INTERFACE]._destructor
    = frg_ast_destructor_declaration_interface;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_INTERFACE]._cloner
    = frg_ast_cloner_declaration_interface;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_INTERFACE]
    ._declaration_name_getter
    = frg_ast_declaration_name_getter_interface;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_INTERFACE]._debug_printer
    = frg_ast_debug_printer_declaration_interface;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_INTERFACE]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_declaration_interface;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_INTERFACE]._type_resolver
    = frg_verification_type_resolver_declaration_interface;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT].name
    = "declaration-function-argument";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT].flags
    = FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT]
    ._destructor
    = frg_ast_destructor_declaration_function_argument;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT]._cloner
    = frg_ast_cloner_declaration_function_argument;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT]
    ._debug_printer
    = frg_ast_debug_printer_declaration_function_argument;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_declaration_function_argument;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT]
    ._formatted_printer
    = frg_formatting_formatted_printer_declaration_function_argument;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION_ARGUMENT]
    ._type_resolver
    = frg_verification_type_resolver_declaration_function_argument;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION].name
    = "declaration-function";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION].flags
    = FRG_AST_NODE_KIND_FLAG_DECLARATION | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION]._destructor
    = frg_ast_destructor_declaration_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION]._cloner
    = frg_ast_cloner_declaration_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION]
    ._declaration_name_getter
    = frg_ast_declaration_name_getter_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION]._debug_printer
    = frg_ast_debug_printer_declaration_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_declaration_function;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_FUNCTION]._type_resolver
    = frg_verification_type_resolver_declaration_function;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT].name
    = "declaration-assignment";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT].flags
    = FRG_AST_NODE_KIND_FLAG_DECLARATION | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT]._destructor
    = frg_ast_destructor_declaration_assignment;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT]._cloner
    = frg_ast_cloner_declaration_assignment;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT]
    ._declaration_name_getter
    = frg_ast_declaration_name_getter_assignment;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT]._debug_printer
    = frg_ast_debug_printer_declaration_assignment;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_declaration_assignment;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_ASSIGNMENT]._type_resolver
    = frg_verification_type_resolver_declaration_assignment;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_BLOCK].name
    = "declaration-block";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_BLOCK].flags
    = FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_BLOCK]._destructor
    = frg_ast_destructor_declaration_block;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_BLOCK]._cloner
    = frg_ast_cloner_declaration_block;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_BLOCK]._debug_printer
    = frg_ast_debug_printer_declaration_block;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_DECLARATION_BLOCK]._visitor_acceptor
    = frg_ast_visitor_acceptor_declaration_block;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_RETURN].name
    = "statement-return";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_RETURN].flags
    = FRG_AST_NODE_KIND_FLAG_STATEMENT | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_RETURN]._destructor
    = frg_ast_destructor_statement_return;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_RETURN]._cloner
    = frg_ast_cloner_statement_return;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_RETURN]._debug_printer
    = frg_ast_debug_printer_statement_return;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_RETURN]._visitor_acceptor
    = frg_ast_visitor_acceptor_statement_return;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE].name
    = "statement-if-conditional-clause";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE].flags
    = FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE]
    ._destructor
    = frg_ast_destructor_statement_if_conditional_clause;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE]
    ._cloner
    = frg_ast_cloner_statement_if_conditional_clause;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE]
    ._debug_printer
    = frg_ast_debug_printer_statement_if_conditional_clause;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF_CONDITIONAL_CLAUSE]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_statement_if_conditional_clause;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF].name = "statement-if";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF].flags
    = FRG_AST_NODE_KIND_FLAG_STATEMENT | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF]._destructor
    = frg_ast_destructor_statement_if;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF]._cloner
    = frg_ast_cloner_statement_if;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF]._debug_printer
    = frg_ast_debug_printer_statement_if;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_IF]._visitor_acceptor
    = frg_ast_visitor_acceptor_statement_if;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_WHILE].name
    = "statement-while";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_WHILE].flags
    = FRG_AST_NODE_KIND_FLAG_STATEMENT | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_WHILE]._destructor
    = frg_ast_destructor_statement_while;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_WHILE]._cloner
    = frg_ast_cloner_statement_while;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_WHILE]._debug_printer
    = frg_ast_debug_printer_statement_while;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_WHILE]._visitor_acceptor
    = frg_ast_visitor_acceptor_statement_while;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_BLOCK].name
    = "statement-block";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_BLOCK].flags
    = FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_BLOCK]._destructor
    = frg_ast_destructor_statement_block;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_BLOCK]._cloner
    = frg_ast_cloner_statement_block;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_BLOCK]._debug_printer
    = frg_ast_debug_printer_statement_block;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_STATEMENT_BLOCK]._visitor_acceptor
    = frg_ast_visitor_acceptor_statement_block;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BOOL].name = "value-bool";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BOOL].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BOOL]._cloner
    = frg_ast_cloner_value_bool;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BOOL]._debug_printer
    = frg_ast_debug_printer_value_bool;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BOOL]._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INT].name = "value-int";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INT]._cloner
    = frg_ast_cloner_value_int;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INT]._debug_printer
    = frg_ast_debug_printer_value_int;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INT]._type_resolver
    = frg_verification_type_resolver_value_int;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_FLOAT].name = "value-float";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_FLOAT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_FLOAT]._cloner
    = frg_ast_cloner_value_float;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_FLOAT]._debug_printer
    = frg_ast_debug_printer_value_float;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_FLOAT]._type_resolver
    = frg_verification_type_resolver_value_float;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CHARACTER].name
    = "value-character";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CHARACTER].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CHARACTER]._cloner
    = frg_ast_cloner_value_character;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CHARACTER]._debug_printer
    = frg_ast_debug_printer_value_character;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CHARACTER]._type_resolver
    = frg_verification_type_resolver_value_character;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRING].name = "value-string";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRING].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRING]._destructor
    = frg_ast_destructor_value_string;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRING]._cloner
    = frg_ast_cloner_value_string;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRING]._debug_printer
    = frg_ast_debug_printer_value_string;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRING]._type_resolver
    = frg_verification_type_resolver_value_string;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY].name = "value-array";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY]._destructor
    = frg_ast_destructor_value_array;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY]._cloner
    = frg_ast_cloner_value_array;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY]._debug_printer
    = frg_ast_debug_printer_value_array;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_array;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY]._type_resolver
    = frg_verification_type_resolver_value_array;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED].name
    = "value-array-repeated";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED]._destructor
    = frg_ast_destructor_value_array_repeated;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED]._cloner
    = frg_ast_cloner_value_array_repeated;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED]._debug_printer
    = frg_ast_debug_printer_value_array_repeated;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_array_repeated;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ARRAY_REPEATED]._type_resolver
    = frg_verification_type_resolver_value_array_repeated;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRUCTURE].name
    = "value-structure";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRUCTURE].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRUCTURE]._destructor
    = frg_ast_destructor_value_structure;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRUCTURE]._cloner
    = frg_ast_cloner_value_structure;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRUCTURE]._debug_printer
    = frg_ast_debug_printer_value_structure;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_STRUCTURE]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_structure;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SYMBOL].name = "value-symbol";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SYMBOL].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SYMBOL]._destructor
    = frg_ast_destructor_value_symbol;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SYMBOL]._cloner
    = frg_ast_cloner_value_symbol;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SYMBOL]._debug_printer
    = frg_ast_debug_printer_value_symbol;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SYMBOL]._type_resolver
    = frg_verification_type_resolver_value_symbol;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DEREFERENCE].name
    = "value-dereference";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DEREFERENCE].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_UNARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DEREFERENCE]._destructor
    = frg_ast_destructor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DEREFERENCE]._cloner
    = frg_ast_cloner_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DEREFERENCE]._debug_printer
    = frg_ast_debug_printer_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DEREFERENCE]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DEREFERENCE]._type_resolver
    = frg_verification_type_resolver_value_dereference;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_GET_ADDRESS].name
    = "value-get-address";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_GET_ADDRESS].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_UNARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_GET_ADDRESS]._destructor
    = frg_ast_destructor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_GET_ADDRESS]._cloner
    = frg_ast_cloner_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_GET_ADDRESS]._debug_printer
    = frg_ast_debug_printer_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_GET_ADDRESS]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_GET_ADDRESS]._type_resolver
    = frg_verification_type_resolver_value_get_address;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT].name
    = "value-call-keyword-argument";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT].flags
    = FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT]
    ._destructor
    = frg_ast_destructor_value_call_keyword_argument;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT]._cloner
    = frg_ast_cloner_value_call_keyword_argument;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT]
    ._debug_printer
    = frg_ast_debug_printer_value_call_keyword_argument;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL_KEYWORD_ARGUMENT]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_call_keyword_argument;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL].name = "value-call";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL]._destructor
    = frg_ast_destructor_value_call;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL]._cloner
    = frg_ast_cloner_value_call;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL]._debug_printer
    = frg_ast_debug_printer_value_call;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_call;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CALL]._type_resolver
    = frg_verification_type_resolver_value_call;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CAST].name = "value-cast";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CAST].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CAST]._destructor
    = frg_ast_destructor_value_cast;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CAST]._cloner
    = frg_ast_cloner_value_cast;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CAST]._debug_printer
    = frg_ast_debug_printer_value_cast;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CAST]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_cast;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_CAST]._type_resolver
    = frg_verification_type_resolver_value_cast;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ACCESS].name = "value-access";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ACCESS].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ACCESS]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ACCESS]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ACCESS]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ACCESS]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ACCESS]._type_resolver
    = frg_verification_type_resolver_value_access;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_NOT].name = "value-bit-not";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_NOT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_UNARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_NOT]._destructor
    = frg_ast_destructor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_NOT]._cloner
    = frg_ast_cloner_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_NOT]._debug_printer
    = frg_ast_debug_printer_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_NOT]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_NOT]._type_resolver
    = frg_verification_type_resolver_value_unary_ident;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND].name = "value-bit-and";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR].name = "value-bit-or";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR].name = "value-bit-xor";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT].name
    = "value-bit-shift-left";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT].name
    = "value-bit-shift-right";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NEGATE].name = "value-negate";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NEGATE].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_UNARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NEGATE]._destructor
    = frg_ast_destructor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NEGATE]._cloner
    = frg_ast_cloner_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NEGATE]._debug_printer
    = frg_ast_debug_printer_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NEGATE]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NEGATE]._type_resolver
    = frg_verification_type_resolver_value_unary_ident;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD].name = "value-add";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT].name
    = "value-subtract";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY].name
    = "value-multiply";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE].name = "value-divide";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT].name
    = "value-divide-int";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO].name = "value-modulo";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE].name
    = "value-exponentiate";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE]._type_resolver
    = frg_verification_type_resolver_value_binary_containing;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EQUALS].name = "value-equals";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EQUALS].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EQUALS]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EQUALS]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EQUALS]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EQUALS]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EQUALS]._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NOT_EQUALS].name
    = "value-not-equals";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NOT_EQUALS].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NOT_EQUALS]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NOT_EQUALS]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NOT_EQUALS]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NOT_EQUALS]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_NOT_EQUALS]._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN].name
    = "value-is-less-than";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN]._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO].name
    = "value-is-less-than-or-equal-to";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO]
    ._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO]
    ._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO]
    ._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_LESS_THAN_OR_EQUAL_TO]
    ._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN].name
    = "value-is-greater-than";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN]._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO]
    .name
    = "value-is-greater-than-or-equal-to";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO]
    .flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO]
    ._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO]
    ._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO]
    ._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_IS_GREATER_THAN_OR_EQUAL_TO]
    ._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT].name
    = "value-logical-not";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_UNARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT]._destructor
    = frg_ast_destructor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT]._cloner
    = frg_ast_cloner_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT]._debug_printer
    = frg_ast_debug_printer_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_NOT]._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND].name
    = "value-logical-and";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND]._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR].name
    = "value-logical-or";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR]._type_resolver
    = frg_verification_type_resolver_as_type_bool;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ASSIGN].name = "value-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ASSIGN]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN].name
    = "value-bit-and-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_AND_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN].name
    = "value-bit-or-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_OR_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN].name
    = "value-bit-xor-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_XOR_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN].name
    = "value-bit-shift-left-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN]
    ._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN]
    ._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_LEFT_ASSIGN]
    ._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN].name
    = "value-bit-shift-right-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN]
    ._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN]
    ._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_BIT_SHIFT_RIGHT_ASSIGN]
    ._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN].name
    = "value-add-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_ADD_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INCREMENT].name
    = "value-increment";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INCREMENT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_UNARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INCREMENT]._destructor
    = frg_ast_destructor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INCREMENT]._cloner
    = frg_ast_cloner_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INCREMENT]._debug_printer
    = frg_ast_debug_printer_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INCREMENT]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_INCREMENT]._type_resolver
    = frg_verification_type_resolver_value_unary_ident;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN].name
    = "value-subtract-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_SUBTRACT_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DECREMENT].name
    = "value-decrement";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DECREMENT].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_UNARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DECREMENT]._destructor
    = frg_ast_destructor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DECREMENT]._cloner
    = frg_ast_cloner_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DECREMENT]._debug_printer
    = frg_ast_debug_printer_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DECREMENT]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_unary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DECREMENT]._type_resolver
    = frg_verification_type_resolver_value_unary_ident;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN].name
    = "value-multiply-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MULTIPLY_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN].name
    = "value-divide-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN].name
    = "value-divide-int-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN]
    ._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_DIVIDE_INT_ASSIGN]
    ._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN].name
    = "value-modulo-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN]._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN]._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_MODULO_ASSIGN]._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN].name
    = "value-exponentiate-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN]
    ._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_EXPONENTIATE_ASSIGN]
    ._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN].name
    = "value-logical-and-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN]
    ._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_AND_ASSIGN]
    ._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN].name
    = "value-logical-or-assign";
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN].flags
    = FRG_AST_NODE_KIND_FLAG_VALUE | FRG_AST_NODE_KIND_FLAG_VALUE_BINARY
    | FRG_AST_NODE_KIND_FLAG_HAS_CHILDREN;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN]._destructor
    = frg_ast_destructor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN]._cloner
    = frg_ast_cloner_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN]
    ._debug_printer
    = frg_ast_debug_printer_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN]
    ._visitor_acceptor
    = frg_ast_visitor_acceptor_value_binary;
  _frg_ast_node_kind_info_table[FRG_AST_NODE_KIND_VALUE_LOGICAL_OR_ASSIGN]
    ._type_resolver
    = frg_verification_type_resolver_value_binary_left;

  _frg_ast_node_kind_info_table_initialized = true;
}

const frg_ast_node_kind_info_t* frg_ast_node_kind_info_get(frg_ast_node_kind_t kind) {
  if (!_frg_ast_node_kind_info_table_initialized) {
    _frg_ast_node_kind_info_table_init();
  }

  frg_assert(kind >= 0 && kind < FRG_AST_NODE_KIND_COUNT);

  return &_frg_ast_node_kind_info_table[kind];
}
