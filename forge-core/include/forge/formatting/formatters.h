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

#include <forge/streams/output.h>
#include <forge/ast/ast.h>

void frg_format_ty_bool(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_u8(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_u16(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_u32(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_u64(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_i8(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_i16(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_i32(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_i64(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_f32(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_f64(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_symbol(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_pointer(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_ty_fn(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_decl_prop(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);

void frg_format_decl_fn_arg(
    frg_stream_output_t *output,
    const frg_ast_t* ast,
    frg_indent_t indent
);
