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
#include <forge/ast/node_kind_info.h>
#include <forge/ast/print_debug.h>
#include <forge/debug.h>

const frg_ast_print_debug_options_t frg_ast_print_debug_options_default
  = { .indentation = 0, .max_depth = INT32_MAX, .max_list_length = INT32_MAX };

frg_ast_print_debug_options_t frg_ast_print_debug_options_get_next(
  const frg_ast_print_debug_options_t* options, frg_indentation_t indentation) {
  frg_ast_print_debug_options_t result
    = { .indentation     = options->indentation + indentation,
        .max_depth       = options->max_depth - 1,
        .max_list_length = options->max_list_length };

  return result;
}

void frg_ast_print_debug(frg_stream_output_t* mut_stream,
                         const frg_ast_node_t* node,
                         const frg_ast_print_debug_options_t* options) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(options);

  if (node == NULL) {
    frg_debug_print_node(mut_stream, "null");
    return;
  }

  frg_debug_print_node(mut_stream, frg_ast_node_kind_info_get(node->kind)->name);

  if (options->max_depth < 0) {
    frg_debug_print_newline(mut_stream, options->indentation);
    frg_stream_output_write_string(mut_stream, "...");
    return;
  }

  frg_ast_debug_printer_t debug_printer
    = frg_ast_node_kind_info_get(node->kind)->_debug_printer;

  if (debug_printer != NULL) {
    debug_printer(mut_stream, node, options);
  }
}
