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

void frg_ast_debug_printer_ty_symbol(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_ty_pointer(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_ty_fn(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_decl_union(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_decl_struct(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_decl_prop(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_decl_iface(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_decl_fn_arg(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_decl_fn(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_decl_var(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_decl_block(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_stmt_return(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_stmt_if(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_stmt_while(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_stmt_block(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_int(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_float(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_char(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_str(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_symbol(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_call_kw_arg(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_call(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_unary(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
void frg_ast_debug_printer_value_binary(frg_stream_output_t* stream, const frg_ast_t* ast, frg_indent_t indent);
