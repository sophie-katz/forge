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

typedef struct {
    frg_indentation_t indentation;
    frg_recursion_depth_t max_depth;
    frg_ast_node_count_t max_list_length;
} frg_ast_print_debug_options_t;

extern const frg_ast_print_debug_options_t frg_ast_print_debug_options_default;

frg_ast_print_debug_options_t frg_ast_print_debug_options_get_next(
    const frg_ast_print_debug_options_t* options,
    frg_indentation_t indentation
);

void frg_ast_print_debug(
    frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    const frg_ast_print_debug_options_t* options
);
