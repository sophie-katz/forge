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

#include <forge/formatting/print.h>
#include <forge/ast/node_kind_info.h>
#include <forge/assert.h>

void frg_formatting_print_formatted(
    frg_stream_output_t* mut_stream,
    const frg_ast_node_t* node,
    frg_indentation_t indentation
) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(node);

    frg_ast_formatting_formatted_printer_t formatted_printer = frg_ast_node_kind_info_get(node->kind)->_formatted_printer;

    if (formatted_printer != NULL) {
        formatted_printer(mut_stream, node, indentation);
    } else {
        frg_die_ast_kind_not_yet_supported(node->kind);
    }
}
