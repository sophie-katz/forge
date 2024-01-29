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

#include <forge/ast/kind_info.h>
#include <forge/ast/cloners.h>
#include <forge/ast/destructors.h>
#include <forge/ast/decl_name_getters.h>
#include <forge/ast/visitor_acceptors.h>
#include <forge/ast/debug_printers.h>
#include <forge/common/error.h>

bool _frg_ast_kind_info_table_initialized = false;
frg_ast_kind_info_t _frg_ast_kind_info_table[FRG_AST_KIND_COUNT];

void _frg_ast_kind_info_table_init(void) {
    frg_assert(!_frg_ast_kind_info_table_initialized);

    memset(_frg_ast_kind_info_table, 0, sizeof(_frg_ast_kind_info_table));

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_BOOL].name = "ty-bool";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_BOOL].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_BOOL].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U8].name = "ty-u8";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U8].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_INT | FRG_AST_KIND_FLAG_TY_INT_UNSIGNED;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U8].ty_bit_width = 8;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U8].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U16].name = "ty-u16";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U16].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_INT | FRG_AST_KIND_FLAG_TY_INT_UNSIGNED;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U16].ty_bit_width = 16;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U16].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U32].name = "ty-u32";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U32].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_INT | FRG_AST_KIND_FLAG_TY_INT_UNSIGNED;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U32].ty_bit_width = 32;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U32].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U64].name = "ty-u64";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U64].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_INT | FRG_AST_KIND_FLAG_TY_INT_UNSIGNED;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U64].ty_bit_width = 64;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_U64].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I8].name = "ty-i8";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I8].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_INT | FRG_AST_KIND_FLAG_TY_INT_SIGNED;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I8].ty_bit_width = 8;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I8].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I16].name = "ty-i16";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I16].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_INT | FRG_AST_KIND_FLAG_TY_INT_SIGNED;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I16].ty_bit_width = 16;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I16].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I32].name = "ty-i32";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I32].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_INT | FRG_AST_KIND_FLAG_TY_INT_SIGNED;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I32].ty_bit_width = 32;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I32].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I64].name = "ty-i64";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I64].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_INT | FRG_AST_KIND_FLAG_TY_INT_SIGNED;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I64].ty_bit_width = 64;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_I64].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_F32].name = "ty-f32";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_F32].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_FLOAT;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_F32].ty_bit_width = 32;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_F32].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_F64].name = "ty-f64";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_F64].flags = FRG_AST_KIND_FLAG_TY | FRG_AST_KIND_FLAG_TY_PRIMARY | FRG_AST_KIND_FLAG_TY_FLOAT;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_F64].ty_bit_width = 64;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_F64].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_SYMBOL].name = "ty-symbol";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_SYMBOL].flags = FRG_AST_KIND_FLAG_TY;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_SYMBOL].destructor = frg_ast_destroy_ty_symbol;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_SYMBOL].cloner = frg_ast_clone_ty_symbol;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_SYMBOL].debug_printer = frg_ast_debug_printer_ty_symbol;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_POINTER].name = "ty-pointer";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_POINTER].flags = FRG_AST_KIND_FLAG_TY;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_POINTER].destructor = frg_ast_destroy_ty_pointer;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_POINTER].cloner = frg_ast_clone_ty_pointer;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_POINTER].debug_printer = frg_ast_debug_printer_ty_pointer;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_POINTER].visitor_acceptor = frg_ast_visitor_acceptor_ty_pointer;

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].name = "ty-fn";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].flags = FRG_AST_KIND_FLAG_TY;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].destructor = frg_ast_destroy_ty_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].cloner = frg_ast_clone_ty_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].debug_printer = frg_ast_debug_printer_ty_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].visitor_acceptor = frg_ast_visitor_acceptor_ty_fn;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].name = "decl-union";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].destructor = frg_ast_destroy_decl_union;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].cloner = frg_ast_clone_decl_union;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].decl_name_getter = frg_ast_decl_name_getter_decl_union;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].debug_printer = frg_ast_debug_printer_decl_union;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].visitor_acceptor = frg_ast_visitor_acceptor_decl_union;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].name = "decl-struct";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].destructor = frg_ast_destroy_decl_struct;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].cloner = frg_ast_clone_decl_struct;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].decl_name_getter = frg_ast_decl_name_getter_decl_struct;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].debug_printer = frg_ast_debug_printer_decl_struct;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].visitor_acceptor = frg_ast_visitor_acceptor_decl_struct;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].name = "decl-prop";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].destructor = frg_ast_destroy_decl_prop;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].cloner = frg_ast_clone_decl_prop;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].decl_name_getter = frg_ast_decl_name_getter_decl_prop;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].debug_printer = frg_ast_debug_printer_decl_prop;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].visitor_acceptor = frg_ast_visitor_acceptor_decl_prop;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].name = "decl-iface";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].destructor = frg_ast_destroy_decl_iface;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].cloner = frg_ast_clone_decl_iface;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].decl_name_getter = frg_ast_decl_name_getter_decl_iface;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].debug_printer = frg_ast_debug_printer_decl_iface;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].visitor_acceptor = frg_ast_visitor_acceptor_decl_iface;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].name = "decl-fn-arg";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].flags = FRG_AST_KIND_FLAG_NONE;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].destructor = frg_ast_destroy_decl_fn_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].cloner = frg_ast_clone_decl_fn_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].decl_name_getter = frg_ast_decl_name_getter_decl_fn_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].debug_printer = frg_ast_debug_printer_decl_fn_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].visitor_acceptor = frg_ast_visitor_acceptor_decl_fn_arg;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].name = "decl-fn";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].destructor = frg_ast_destroy_decl_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].cloner = frg_ast_clone_decl_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].decl_name_getter = frg_ast_decl_name_getter_decl_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].debug_printer = frg_ast_debug_printer_decl_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].visitor_acceptor = frg_ast_visitor_acceptor_decl_fn;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].name = "decl-var";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].destructor = frg_ast_destroy_decl_var;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].cloner = frg_ast_clone_decl_var;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].decl_name_getter = frg_ast_decl_name_getter_decl_var;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].debug_printer = frg_ast_debug_printer_decl_var;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].visitor_acceptor = frg_ast_visitor_acceptor_decl_var;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].name = "decl-block";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].destructor = frg_ast_destroy_decl_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].cloner = frg_ast_clone_decl_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].debug_printer = frg_ast_debug_printer_decl_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].visitor_acceptor = frg_ast_visitor_acceptor_decl_block;

    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].name = "stmt-return";
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].flags = FRG_AST_KIND_FLAG_STMT;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].destructor = frg_ast_destroy_stmt_return;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].cloner = frg_ast_clone_stmt_return;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].debug_printer = frg_ast_debug_printer_stmt_return;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].visitor_acceptor = frg_ast_visitor_acceptor_stmt_return;

    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].name = "stmt-if";
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].flags = FRG_AST_KIND_FLAG_STMT;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].destructor = frg_ast_destroy_stmt_if;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].cloner = frg_ast_clone_stmt_if;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].debug_printer = frg_ast_debug_printer_stmt_if;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].visitor_acceptor = frg_ast_visitor_acceptor_stmt_if;

    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].name = "stmt-while";
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].flags = FRG_AST_KIND_FLAG_STMT;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].destructor = frg_ast_destroy_stmt_while;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].cloner = frg_ast_clone_stmt_while;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].debug_printer = frg_ast_debug_printer_stmt_while;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].visitor_acceptor = frg_ast_visitor_acceptor_stmt_while;

    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].name = "stmt-block";
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].flags = FRG_AST_KIND_FLAG_STMT;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].destructor = frg_ast_destroy_stmt_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].cloner = frg_ast_clone_stmt_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].debug_printer = frg_ast_debug_printer_stmt_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].visitor_acceptor = frg_ast_visitor_acceptor_stmt_block;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_TRUE].name = "value-true";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_TRUE].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_PRIMARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_TRUE].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FALSE].name = "value-false";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FALSE].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_PRIMARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FALSE].cloner = frg_ast_clone_base;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INT].name = "value-int";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INT].flags = FRG_AST_KIND_FLAG_VALUE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INT].destructor = frg_ast_destroy_value_int;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INT].cloner = frg_ast_clone_value_int;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INT].debug_printer = frg_ast_debug_printer_value_int;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INT].visitor_acceptor = frg_ast_visitor_acceptor_value_int;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].name = "value-float";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].flags = FRG_AST_KIND_FLAG_VALUE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].destructor = frg_ast_destroy_value_float;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].cloner = frg_ast_clone_value_float;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].debug_printer = frg_ast_debug_printer_value_float;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].visitor_acceptor = frg_ast_visitor_acceptor_value_float;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CHAR].name = "value-char";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CHAR].flags = FRG_AST_KIND_FLAG_VALUE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CHAR].cloner = frg_ast_clone_value_char;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CHAR].debug_printer = frg_ast_debug_printer_value_char;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_STR].name = "value-str";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_STR].flags = FRG_AST_KIND_FLAG_VALUE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_STR].destructor = frg_ast_destroy_value_str;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_STR].cloner = frg_ast_clone_value_str;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_STR].debug_printer = frg_ast_debug_printer_value_str;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SYMBOL].name = "value-symbol";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SYMBOL].flags = FRG_AST_KIND_FLAG_VALUE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SYMBOL].destructor = frg_ast_destroy_value_symbol;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SYMBOL].cloner = frg_ast_clone_value_symbol;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SYMBOL].debug_printer = frg_ast_debug_printer_value_symbol;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEREF].name = "value-deref";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEREF].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEREF].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEREF].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEREF].debug_printer = frg_ast_debug_printer_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEREF].visitor_acceptor = frg_ast_visitor_acceptor_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].name = "value-getaddr";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].debug_printer = frg_ast_debug_printer_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].visitor_acceptor = frg_ast_visitor_acceptor_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].name = "value-call-kw-arg";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].flags = FRG_AST_KIND_FLAG_NONE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].destructor = frg_ast_destroy_value_call_kw_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].cloner = frg_ast_clone_value_call_kw_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].debug_printer = frg_ast_debug_printer_value_call_kw_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].visitor_acceptor = frg_ast_visitor_acceptor_value_call_kw_arg;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].name = "value-call";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].flags = FRG_AST_KIND_FLAG_VALUE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].destructor = frg_ast_destroy_value_call;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].cloner = frg_ast_clone_value_call;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].debug_printer = frg_ast_debug_printer_value_call;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].visitor_acceptor = frg_ast_visitor_acceptor_value_call;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].name = "value-access";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].name = "value-bit-not";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].debug_printer = frg_ast_debug_printer_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].visitor_acceptor = frg_ast_visitor_acceptor_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].name = "value-bit-and";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].name = "value-bit-or";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].name = "value-bit-xor";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].name = "value-bit-shl";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].name = "value-bit-shr";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].name = "value-neg";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].debug_printer = frg_ast_debug_printer_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].visitor_acceptor = frg_ast_visitor_acceptor_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].name = "value-add";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].name = "value-sub";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].name = "value-mul";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].name = "value-div";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].name = "value-div-int";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].name = "value-mod";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].name = "value-exp";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].name = "value-eq";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].name = "value-ne";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].name = "value-lt";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].name = "value-le";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].name = "value-gt";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].name = "value-ge";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].name = "value-log-not";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].debug_printer = frg_ast_debug_printer_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].visitor_acceptor = frg_ast_visitor_acceptor_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].name = "value-log-and";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].name = "value-log-or";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].name = "value-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].name = "value-bit-and-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].name = "value-bit-or-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].name = "value-bit-xor-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].name = "value-bit-shl-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].name = "value-bit-shr-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].name = "value-add-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].name = "value-inc";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].debug_printer = frg_ast_debug_printer_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].visitor_acceptor = frg_ast_visitor_acceptor_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].name = "value-sub-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].name = "value-dec";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].debug_printer = frg_ast_debug_printer_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].visitor_acceptor = frg_ast_visitor_acceptor_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].name = "value-mul-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].name = "value-div-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].name = "value-div-int-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].name = "value-mod-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].name = "value-exp-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].name = "value-log-and-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].name = "value-log-or-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].visitor_acceptor = frg_ast_visitor_acceptor_value_binary;

    _frg_ast_kind_info_table_initialized = true;
}

const frg_ast_kind_info_t* frg_ast_kind_info_get(frg_ast_kind_t kind) {
    if (!_frg_ast_kind_info_table_initialized) {
        _frg_ast_kind_info_table_init();
    }

    frg_assert(kind >= 0 && kind < FRG_AST_KIND_COUNT);

    return &_frg_ast_kind_info_table[kind];
}
