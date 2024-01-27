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

#include <forge/ast/ast.h>
#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/common/debug.h>
#include <forge/ast/cloners.h>
#include <forge/ast/destructors.h>
#include <forge/ast/decl_name_getters.h>
#include <forge/ast/debug_printers.h>
#include <stddef.h>
#include <stdlib.h>

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

    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].name = "ty-fn";
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].flags = FRG_AST_KIND_FLAG_TY;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].destructor = frg_ast_destroy_ty_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].cloner = frg_ast_clone_ty_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_TY_FN].debug_printer = frg_ast_debug_printer_ty_fn;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].name = "decl-union";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].destructor = frg_ast_destroy_decl_union;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].cloner = frg_ast_clone_decl_union;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].decl_name_getter = frg_ast_decl_name_getter_decl_union;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_UNION].debug_printer = frg_ast_debug_printer_decl_union;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].name = "decl-struct";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].destructor = frg_ast_destroy_decl_struct;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].cloner = frg_ast_clone_decl_struct;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].decl_name_getter = frg_ast_decl_name_getter_decl_struct;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_STRUCT].debug_printer = frg_ast_debug_printer_decl_struct;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].name = "decl-prop";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].destructor = frg_ast_destroy_decl_prop;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].cloner = frg_ast_clone_decl_prop;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].decl_name_getter = frg_ast_decl_name_getter_decl_prop;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_PROP].debug_printer = frg_ast_debug_printer_decl_prop;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].name = "decl-iface";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].destructor = frg_ast_destroy_decl_iface;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].cloner = frg_ast_clone_decl_iface;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].decl_name_getter = frg_ast_decl_name_getter_decl_iface;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_IFACE].debug_printer = frg_ast_debug_printer_decl_iface;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].name = "decl-fn-arg";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].flags = FRG_AST_KIND_FLAG_NONE;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].destructor = frg_ast_destroy_decl_fn_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].cloner = frg_ast_clone_decl_fn_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].decl_name_getter = frg_ast_decl_name_getter_decl_fn_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN_ARG].debug_printer = frg_ast_debug_printer_decl_fn_arg;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].name = "decl-fn";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].destructor = frg_ast_destroy_decl_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].cloner = frg_ast_clone_decl_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].decl_name_getter = frg_ast_decl_name_getter_decl_fn;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_FN].debug_printer = frg_ast_debug_printer_decl_fn;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].name = "decl-var";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].destructor = frg_ast_destroy_decl_var;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].cloner = frg_ast_clone_decl_var;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].decl_name_getter = frg_ast_decl_name_getter_decl_var;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_VAR].debug_printer = frg_ast_debug_printer_decl_var;

    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].name = "decl-block";
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].flags = FRG_AST_KIND_FLAG_DECL;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].destructor = frg_ast_destroy_decl_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].cloner = frg_ast_clone_decl_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_DECL_BLOCK].debug_printer = frg_ast_debug_printer_decl_block;

    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].name = "stmt-return";
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].flags = FRG_AST_KIND_FLAG_STMT;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].destructor = frg_ast_destroy_stmt_return;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].cloner = frg_ast_clone_stmt_return;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_RETURN].debug_printer = frg_ast_debug_printer_stmt_return;

    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].name = "stmt-if";
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].flags = FRG_AST_KIND_FLAG_STMT;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].destructor = frg_ast_destroy_stmt_if;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].cloner = frg_ast_clone_stmt_if;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_IF].debug_printer = frg_ast_debug_printer_stmt_if;

    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].name = "stmt-while";
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].flags = FRG_AST_KIND_FLAG_STMT;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].destructor = frg_ast_destroy_stmt_while;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].cloner = frg_ast_clone_stmt_while;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_WHILE].debug_printer = frg_ast_debug_printer_stmt_while;

    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].name = "stmt-block";
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].flags = FRG_AST_KIND_FLAG_STMT;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].destructor = frg_ast_destroy_stmt_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].cloner = frg_ast_clone_stmt_block;
    _frg_ast_kind_info_table[FRG_AST_KIND_STMT_BLOCK].debug_printer = frg_ast_debug_printer_stmt_block;

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

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].name = "value-float";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].flags = FRG_AST_KIND_FLAG_VALUE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].destructor = frg_ast_destroy_value_float;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].cloner = frg_ast_clone_value_float;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_FLOAT].debug_printer = frg_ast_debug_printer_value_float;

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

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].name = "value-getaddr";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GETADDR].debug_printer = frg_ast_debug_printer_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].name = "value-call-kw-arg";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].flags = FRG_AST_KIND_FLAG_NONE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].destructor = frg_ast_destroy_value_call_kw_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].cloner = frg_ast_clone_value_call_kw_arg;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL_KW_ARG].debug_printer = frg_ast_debug_printer_value_call_kw_arg;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].name = "value-call";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].flags = FRG_AST_KIND_FLAG_VALUE;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].destructor = frg_ast_destroy_value_call;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].cloner = frg_ast_clone_value_call;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_CALL].debug_printer = frg_ast_debug_printer_value_call;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].name = "value-access";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ACCESS].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].name = "value-bit-not";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_NOT].debug_printer = frg_ast_debug_printer_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].name = "value-bit-and";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].name = "value-bit-or";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].name = "value-bit-xor";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].name = "value-bit-shl";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].name = "value-bit-shr";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].name = "value-neg";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NEG].debug_printer = frg_ast_debug_printer_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].name = "value-add";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].name = "value-sub";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].name = "value-mul";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].name = "value-div";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].name = "value-div-int";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].name = "value-mod";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].name = "value-exp";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].name = "value-eq";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EQ].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].name = "value-ne";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_NE].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].name = "value-lt";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LT].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].name = "value-le";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LE].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].name = "value-gt";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GT].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].name = "value-ge";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_GE].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].name = "value-log-not";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_NOT].debug_printer = frg_ast_debug_printer_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].name = "value-log-and";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].name = "value-log-or";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].name = "value-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].name = "value-bit-and-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_AND_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].name = "value-bit-or-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_OR_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].name = "value-bit-xor-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_XOR_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].name = "value-bit-shl-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHL_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].name = "value-bit-shr-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_BIT_SHR_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].name = "value-add-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_ADD_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].name = "value-inc";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_INC].debug_printer = frg_ast_debug_printer_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].name = "value-sub-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_SUB_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].name = "value-dec";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_UNARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].destructor = frg_ast_destroy_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].cloner = frg_ast_clone_value_unary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DEC].debug_printer = frg_ast_debug_printer_value_unary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].name = "value-mul-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MUL_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].name = "value-div-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].name = "value-div-int-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_DIV_INT_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].name = "value-mod-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_MOD_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].name = "value-exp-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_EXP_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].name = "value-log-and-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_AND_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].name = "value-log-or-assign";
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].flags = FRG_AST_KIND_FLAG_VALUE | FRG_AST_KIND_FLAG_VALUE_BINARY;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].destructor = frg_ast_destroy_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].cloner = frg_ast_clone_value_binary;
    _frg_ast_kind_info_table[FRG_AST_KIND_VALUE_LOG_OR_ASSIGN].debug_printer = frg_ast_debug_printer_value_binary;

    _frg_ast_kind_info_table_initialized = true;
}

const frg_ast_kind_info_t* frg_ast_kind_info_get(frg_ast_kind_t kind) {
    if (!_frg_ast_kind_info_table_initialized) {
        _frg_ast_kind_info_table_init();
    }

    frg_assert(kind >= 0 && kind < FRG_AST_KIND_COUNT);

    return &_frg_ast_kind_info_table[kind];
}

frg_ast_t* frg_ast_new_ty_primary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind
) {
    frg_assert(frg_ast_kind_info_get(kind)->flags & FRG_AST_KIND_FLAG_TY_PRIMARY);

    frg_ast_t* ast = frg_safe_malloc(sizeof(frg_ast_t));
    ast->source_range = *source_range;
    ast->kind = kind;

    return ast;
}

frg_ast_ty_symbol_t* frg_ast_new_ty_symbol(
    const frg_parsing_range_t *source_range,
    GString* name
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_ty_symbol_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_symbol_t));

    ast->base.kind = FRG_AST_KIND_TY_SYMBOL;
    ast->base.source_range = *source_range;
    ast->name = name;

    return ast;
}

frg_ast_ty_pointer_t* frg_ast_new_ty_pointer(
    const frg_parsing_range_t *source_range,
    frg_ast_t* value
) {
    frg_assert_pointer_non_null(value);

    frg_ast_ty_pointer_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_pointer_t));

    ast->base.kind = FRG_AST_KIND_TY_POINTER;
    ast->base.source_range = *source_range;
    ast->value = value;

    return ast;
}

frg_ast_ty_fn_t* frg_ast_new_ty_fn(
    const frg_parsing_range_t *source_range,
    GList* args,
    frg_ast_t* var_pos_args,
    frg_ast_t* var_kw_args,
    frg_ast_t* return_ty
) {
    frg_ast_ty_fn_t* ast = frg_safe_malloc(sizeof(frg_ast_ty_fn_t));

    ast->base.kind = FRG_AST_KIND_TY_FN;
    ast->base.source_range = *source_range;
    ast->args = args;
    ast->var_pos_args = var_pos_args;
    ast->var_kw_args = var_kw_args;
    ast->return_ty = return_ty;

    return ast;
}

frg_ast_decl_union_t* frg_ast_new_decl_union(
    const frg_parsing_range_t *source_range,
    GString* name,
    GList* props
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(props);

    frg_ast_decl_union_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_union_t));

    ast->base.kind = FRG_AST_KIND_DECL_UNION;
    ast->base.source_range = *source_range;
    ast->name = name;
    ast->props = props;

    return ast;
}

frg_ast_decl_struct_t* frg_ast_new_decl_struct(
    const frg_parsing_range_t *source_range,
    GString* name,
    GList* decls
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_struct_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_struct_t));

    ast->base.kind = FRG_AST_KIND_DECL_STRUCT;
    ast->base.source_range = *source_range;
    ast->name = name;
    ast->decls = decls;

    return ast;
}

frg_ast_decl_prop_t* frg_ast_new_decl_prop(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_prop_flags_t flags,
    GString* name,
    frg_ast_t* ty
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_prop_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_prop_t));

    ast->base.kind = FRG_AST_KIND_DECL_PROP;
    ast->base.source_range = *source_range;
    ast->flags = flags;
    ast->name = name;
    ast->ty = ty;

    return ast;
}

frg_ast_decl_iface_t* frg_ast_new_decl_iface(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_iface_flags_t flags,
    GString* name,
    GList* extends,
    GList* decls
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_decl_iface_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_iface_t));

    ast->base.kind = FRG_AST_KIND_DECL_IFACE;
    ast->base.source_range = *source_range;
    ast->flags = flags;
    ast->name = name;
    ast->extends = extends;
    ast->decls = decls;

    return ast;
}

frg_ast_decl_fn_arg_t* frg_ast_new_decl_fn_arg(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_fn_arg_flags_t flags,
    frg_ast_decl_prop_t* prop,
    frg_ast_t* default_value
) {
    frg_assert_pointer_non_null(prop);

    frg_ast_decl_fn_arg_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_fn_arg_t));

    ast->base.kind = FRG_AST_KIND_DECL_FN_ARG;
    ast->base.source_range = *source_range;
    ast->flags = flags;
    ast->prop = prop;
    ast->default_value = default_value;

    return ast;
}

frg_ast_decl_fn_t* frg_ast_new_decl_fn(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_fn_flags_t flags,
    GString* name,
    frg_ast_ty_fn_t* ty,
    frg_ast_t* body
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(ty);

    frg_ast_decl_fn_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_fn_t));

    ast->base.kind = FRG_AST_KIND_DECL_FN;
    ast->base.source_range = *source_range;
    ast->flags = flags;
    ast->name = name;
    ast->ty = ty;
    ast->body = body;

    return ast;
}

frg_ast_decl_var_t* frg_ast_new_decl_var(
    const frg_parsing_range_t *source_range,
    frg_ast_decl_prop_t* prop,
    frg_ast_t* initial_value
) {
    frg_assert_pointer_non_null(prop);

    frg_ast_decl_var_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_var_t));

    ast->base.kind = FRG_AST_KIND_DECL_VAR;
    ast->base.source_range = *source_range;
    ast->prop = prop;
    ast->initial_value = initial_value;

    return ast;
}

frg_ast_decl_block_t* frg_ast_new_decl_block(
    const frg_parsing_range_t *source_range,
    GList* decls
) {
    frg_ast_decl_block_t* ast = frg_safe_malloc(sizeof(frg_ast_decl_block_t));

    ast->base.kind = FRG_AST_KIND_DECL_BLOCK;
    ast->base.source_range = *source_range;
    ast->decls = decls;

    return ast;
}

frg_ast_stmt_return_t* frg_ast_new_stmt_return(
    const frg_parsing_range_t *source_range,
    frg_ast_t* value
) {
    frg_ast_stmt_return_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_return_t));

    ast->base.kind = FRG_AST_KIND_STMT_RETURN;
    ast->base.source_range = *source_range;
    ast->value = value;

    return ast;
}

frg_ast_stmt_if_t* frg_ast_new_stmt_if(
    const frg_parsing_range_t *source_range,
    frg_ast_t* condition,
    frg_ast_t* then_clause,
    frg_ast_t* else_clause
) {
    frg_assert_pointer_non_null(condition);
    frg_assert_pointer_non_null(then_clause);

    frg_ast_stmt_if_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_if_t));

    ast->base.kind = FRG_AST_KIND_STMT_IF;
    ast->base.source_range = *source_range;
    ast->condition = condition;
    ast->then_clause = then_clause;
    ast->else_clause = else_clause;

    return ast;
}

frg_ast_stmt_while_t* frg_ast_new_stmt_while(
    const frg_parsing_range_t *source_range,
    frg_ast_t* condition,
    frg_ast_t* body
) {
    frg_assert_pointer_non_null(condition);

    frg_ast_stmt_while_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_while_t));

    ast->base.kind = FRG_AST_KIND_STMT_WHILE;
    ast->base.source_range = *source_range;
    ast->condition = condition;
    ast->body = body;

    return ast;
}

frg_ast_stmt_block_t* frg_ast_new_stmt_block(
    const frg_parsing_range_t *source_range,
    GList* stmts
) {
    frg_ast_stmt_block_t* ast = frg_safe_malloc(sizeof(frg_ast_stmt_block_t));

    ast->base.kind = FRG_AST_KIND_STMT_BLOCK;
    ast->base.source_range = *source_range;
    ast->stmts = stmts;

    return ast;
}

frg_ast_t* frg_ast_new_value_primary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind
) {
    frg_assert(frg_ast_kind_info_get(kind)->flags & FRG_AST_KIND_FLAG_VALUE_PRIMARY);

    frg_ast_t* ast = frg_safe_malloc(sizeof(frg_ast_t));

    ast->kind = kind;
    ast->source_range = *source_range;

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_i8(
    const frg_parsing_range_t *source_range,
    int8_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.i8 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_I8);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_i16(
    const frg_parsing_range_t *source_range,
    int16_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.i16 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_I16);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_i32(
    const frg_parsing_range_t *source_range,
    int32_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.i32 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_I32);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_i64(
    const frg_parsing_range_t *source_range,
    int64_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.i64 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_I64);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_u8(
    const frg_parsing_range_t *source_range,
    uint8_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.u8 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_U8);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_u16(
    const frg_parsing_range_t *source_range,
    uint16_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.u16 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_U16);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_u32(
    const frg_parsing_range_t *source_range,
    uint32_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.u32 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_U32);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_u64(
    const frg_parsing_range_t *source_range,
    uint64_t value
) {
    frg_ast_value_int_t* ast = frg_safe_malloc(sizeof(frg_ast_value_int_t));

    ast->base.kind = FRG_AST_KIND_VALUE_INT;
    ast->base.source_range = *source_range;
    ast->value.u64 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_U64);

    return ast;
}

frg_ast_value_int_t* frg_ast_new_value_int_from_lexical_cast_result(
    const frg_parsing_range_t *source_range,
    const frg_parse_uint_result_t* result
) {
    frg_assert_pointer_non_null(result);

    if (result->bit_width == 8) {
        if (result->is_signed) {
            return frg_ast_new_value_i8(source_range, (int8_t)result->value);
        } else {
            return frg_ast_new_value_u8(source_range, (uint8_t)result->value);
        }
    } else if (result->bit_width == 16) {
        if (result->is_signed) {
            return frg_ast_new_value_i16(source_range, (int16_t)result->value);
        } else {
            return frg_ast_new_value_u16(source_range, (uint16_t)result->value);
        }
    } else if (result->bit_width == 32) {
        if (result->is_signed) {
            return frg_ast_new_value_i32(source_range, (int32_t)result->value);
        } else {
            return frg_ast_new_value_u32(source_range, (uint32_t)result->value);
        }
    } else if (result->bit_width == 64) {
        if (result->is_signed) {
            return frg_ast_new_value_i64(source_range, (int64_t)result->value);
        } else {
            return frg_ast_new_value_u64(source_range, (uint64_t)result->value);
        }
    } else {
        frg_die("unexpected bit width %i", result->bit_width);
    }
}

frg_ast_value_float_t* frg_ast_new_value_f32(
    const frg_parsing_range_t *source_range,
    frg_f32_t value
) {
    frg_ast_value_float_t* ast = frg_safe_malloc(sizeof(frg_ast_value_float_t));

    ast->base.kind = FRG_AST_KIND_VALUE_FLOAT;
    ast->base.source_range = *source_range;
    ast->value.f32 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_F32);

    return ast;
}

frg_ast_value_float_t* frg_ast_new_value_f64(
    const frg_parsing_range_t *source_range,
    frg_f64_t value
) {
    frg_ast_value_float_t* ast = frg_safe_malloc(sizeof(frg_ast_value_float_t));

    ast->base.kind = FRG_AST_KIND_VALUE_FLOAT;
    ast->base.source_range = *source_range;
    ast->value.f64 = value;
    ast->ty = frg_ast_new_ty_primary(source_range, FRG_AST_KIND_TY_F64);

    return ast;
}

frg_ast_value_float_t* frg_ast_new_value_float_from_lexical_cast_result(
    const frg_parsing_range_t *source_range,
    const frg_parse_float_result_t* result
) {
    frg_assert_pointer_non_null(result);

    if (result->bit_width == 32) {
        return frg_ast_new_value_f32(source_range, (frg_f32_t)result->value);
    } else if (result->bit_width == 64) {
        return frg_ast_new_value_f64(source_range, (frg_f64_t)result->value);
    } else {
        frg_die("unexpected bit width %i", result->bit_width);
    }
}

frg_ast_value_char_t* frg_ast_new_value_char(
    const frg_parsing_range_t *source_range,
    frg_char_t value
) {
    frg_ast_value_char_t* ast = frg_safe_malloc(sizeof(frg_ast_value_char_t));

    ast->base.kind = FRG_AST_KIND_VALUE_CHAR;
    ast->base.source_range = *source_range;
    ast->value = value;

    return ast;
}

frg_ast_value_str_t* frg_ast_new_value_str(
    const frg_parsing_range_t *source_range,
    GString* value
) {
    frg_assert_pointer_non_null(value);

    frg_ast_value_str_t* ast = frg_safe_malloc(sizeof(frg_ast_value_str_t));

    ast->base.kind = FRG_AST_KIND_VALUE_STR;
    ast->base.source_range = *source_range;
    ast->value = value;

    return ast;
}

frg_ast_value_symbol_t* frg_ast_new_value_symbol(
    const frg_parsing_range_t *source_range,
    GString* name
) {
    frg_assert_gstring_non_empty(name);

    frg_ast_value_symbol_t* ast = frg_safe_malloc(sizeof(frg_ast_value_symbol_t));

    ast->base.kind = FRG_AST_KIND_VALUE_SYMBOL;
    ast->base.source_range = *source_range;
    ast->name = name;

    return ast;
}

frg_ast_value_call_kw_arg_t* frg_ast_new_value_call_kw_arg(
    const frg_parsing_range_t *source_range,
    GString* name,
    frg_ast_t* value
) {
    frg_assert_gstring_non_empty(name);
    frg_assert_pointer_non_null(value);

    frg_ast_value_call_kw_arg_t* ast = frg_safe_malloc(sizeof(frg_ast_value_call_kw_arg_t));

    ast->base.kind = FRG_AST_KIND_VALUE_CALL_KW_ARG;
    ast->base.source_range = *source_range;
    ast->name = name;
    ast->value = value;

    return ast;
}

frg_ast_value_call_t* frg_ast_new_value_call(
    const frg_parsing_range_t *source_range,
    frg_ast_t* callee,
    GList* args,
    GList* kw_args
) {
    frg_assert_pointer_non_null(callee);

    frg_ast_value_call_t* ast = frg_safe_malloc(sizeof(frg_ast_value_call_t));

    ast->base.kind = FRG_AST_KIND_VALUE_CALL;
    ast->base.source_range = *source_range;
    ast->callee = callee;
    ast->args = args;
    ast->kw_args = kw_args;

    return ast;
}

frg_ast_value_unary_t* frg_ast_new_value_unary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind,
    frg_ast_t* operand
) {
    frg_assert(frg_ast_kind_info_get(kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY);
    frg_assert_pointer_non_null(operand);

    frg_ast_value_unary_t* ast = frg_safe_malloc(sizeof(frg_ast_value_unary_t));

    ast->base.kind = kind;
    ast->base.source_range = *source_range;
    ast->operand = operand;

    return ast;
}

frg_ast_value_binary_t* frg_ast_new_value_binary(
    const frg_parsing_range_t *source_range,
    frg_ast_kind_t kind,
    frg_ast_t* left,
    frg_ast_t* right
) {
    frg_assert(frg_ast_kind_info_get(kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY);
    frg_assert_pointer_non_null(left);
    frg_assert_pointer_non_null(right);

    frg_ast_value_binary_t* ast = frg_safe_malloc(sizeof(frg_ast_value_binary_t));

    ast->base.kind = kind;
    ast->base.source_range = *source_range;
    ast->left = left;
    ast->right = right;

    return ast;
}

void frg_ast_destroy(frg_ast_t** ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_pointer_non_null(*ast);

    frg_ast_destructor destructor = frg_ast_kind_info_get((*ast)->kind)->destructor;
    
    if (destructor != NULL) {
        destructor(*ast);
    }

    frg_safe_free((void**)ast);
}

frg_ast_t* frg_ast_clone(const frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    }

    return frg_ast_kind_info_get(ast->kind)->cloner(ast);
}

frg_ast_ty_symbol_t* frg_ast_try_cast_ty_symbol(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_TY_SYMBOL) {
        return (frg_ast_ty_symbol_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_union_t* frg_ast_try_cast_decl_union(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_UNION) {
        return (frg_ast_decl_union_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_struct_t* frg_ast_try_cast_decl_struct(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_STRUCT) {
        return (frg_ast_decl_struct_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_prop_t* frg_ast_try_cast_decl_prop(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_PROP) {
        return (frg_ast_decl_prop_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_iface_t* frg_ast_try_cast_decl_iface(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_IFACE) {
        return (frg_ast_decl_iface_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_fn_arg_t* frg_ast_try_cast_decl_fn_arg(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_FN_ARG) {
        return (frg_ast_decl_fn_arg_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_fn_t* frg_ast_try_cast_decl_fn(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_FN) {
        return (frg_ast_decl_fn_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_var_t* frg_ast_try_cast_decl_var(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_VAR) {
        return (frg_ast_decl_var_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_decl_block_t* frg_ast_try_cast_decl_block(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_DECL_BLOCK) {
        return (frg_ast_decl_block_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_return_t* frg_ast_try_cast_stmt_return(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_STMT_RETURN) {
        return (frg_ast_stmt_return_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_if_t* frg_ast_try_cast_stmt_if(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_STMT_IF) {
        return (frg_ast_stmt_if_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_while_t* frg_ast_try_cast_stmt_while(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_STMT_WHILE) {
        return (frg_ast_stmt_while_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_stmt_block_t* frg_ast_try_cast_stmt_block(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_STMT_BLOCK) {
        return (frg_ast_stmt_block_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_int_t* frg_ast_try_cast_value_int(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_INT) {
        return (frg_ast_value_int_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_float_t* frg_ast_try_cast_value_float(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_FLOAT) {
        return (frg_ast_value_float_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_char_t* frg_ast_try_cast_value_char(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_CHAR) {
        return (frg_ast_value_char_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_str_t* frg_ast_try_cast_value_str(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_STR) {
        return (frg_ast_value_str_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_symbol_t* frg_ast_try_cast_value_symbol(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_SYMBOL) {
        return (frg_ast_value_symbol_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_call_kw_arg_t* frg_ast_try_cast_value_call_kw_arg(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_CALL_KW_ARG) {
        return (frg_ast_value_call_kw_arg_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_call_t* frg_ast_try_cast_value_call(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (ast->kind == FRG_AST_KIND_VALUE_CALL) {
        return (frg_ast_value_call_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_unary_t* frg_ast_try_cast_value_unary(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_UNARY) {
        return (frg_ast_value_unary_t*)ast;
    } else {
        return NULL;
    }
}

frg_ast_value_binary_t* frg_ast_try_cast_value_binary(frg_ast_t* ast) {
    if (ast == NULL) {
        return NULL;
    } else if (frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_VALUE_BINARY) {
        return (frg_ast_value_binary_t*)ast;
    } else {
        return NULL;
    }
}

const char* frg_ast_get_decl_name(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert(frg_ast_kind_info_get(ast->kind)->flags & FRG_AST_KIND_FLAG_DECL);

    frg_ast_decl_name_getter decl_name_getter = frg_ast_kind_info_get(ast->kind)->decl_name_getter;

    frg_assert_pointer_non_null(decl_name_getter);

    return decl_name_getter(ast);
}

const char* frg_ast_get_decl_fn_arg_name(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);
    frg_assert_int_eq(ast->kind, FRG_AST_KIND_DECL_FN_ARG);

    frg_ast_decl_name_getter decl_name_getter = frg_ast_kind_info_get(ast->kind)->decl_name_getter;

    frg_assert_pointer_non_null(decl_name_getter);

    return decl_name_getter(ast);
}

void frg_ast_print_debug(
    frg_stream_output_t* stream,
    const frg_ast_t* ast,
    frg_indent_t indent
) {
    if (ast == NULL) {
        frg_debug_print_node(stream, "null");
        return;
    }

    frg_debug_print_node(stream, frg_ast_kind_info_get(ast->kind)->name);

    frg_ast_debug_printer debug_printer = frg_ast_kind_info_get(ast->kind)->debug_printer;

    if (debug_printer != NULL) {
        debug_printer(stream, ast, indent);
    }
}
