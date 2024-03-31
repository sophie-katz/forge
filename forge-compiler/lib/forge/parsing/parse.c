// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#include <forge/assert.h>
#include <forge/memory.h>
#include <forge/parsing/parse.h>

extern FILE* yyin;
extern int yyparse(frg_ast_node_t** node);
extern int yy_scan_buffer(char* base, size_t size);
extern void yypop_buffer_state();
extern void yyrestart(FILE* file);
extern int yylex_destroy();

const char* _frg_parsing_current_path                     = NULL;
size_t _frg_parsing_current_offset                        = 0;
frg_message_buffer_t* _frg_parsing_current_message_buffer = NULL;
frg_parsing_location_t _frg_parsing_last_location
  = { .path = NULL, .offset = 0, .line_number = 0, .column_number = 0 };

frg_ast_node_t* frg_parse(frg_message_buffer_t* mut_message_buffer,
                          frg_parsing_source_t* mut_source) {
  // Error checking
  frg_assert_pointer_non_null(mut_source);
  frg_assert_string_non_empty(mut_source->path);

  // Set globals
  _frg_parsing_current_path                = mut_source->path;
  _frg_parsing_current_offset              = 0;
  _frg_parsing_current_message_buffer      = mut_message_buffer;
  _frg_parsing_last_location.path          = mut_source->path;
  _frg_parsing_last_location.offset        = 0;
  _frg_parsing_last_location.line_number   = 1;
  _frg_parsing_last_location.column_number = 1;

  if (frg_stream_input_is_buffer(mut_source->stream)) {
    frg_assert_int_greater_than_or_equal_to(frg_stream_get_length(mut_source->stream),
                                            2);
    frg_assert_int_equal_to(
      frg_stream_get_buffer(
        mut_source->stream)[frg_stream_get_length(mut_source->stream) - 2],
      0);
    frg_assert_int_equal_to(
      frg_stream_get_buffer(
        mut_source->stream)[frg_stream_get_length(mut_source->stream) - 1],
      0);

    // Set the scan buffer
    yy_scan_buffer(frg_stream_get_buffer(mut_source->stream),
                   frg_stream_get_length(mut_source->stream));
  } else {
    frg_assert(frg_stream_input_is_file(mut_source->stream));
    frg_assert(!frg_stream_input_has_error(mut_source->stream));

    // Set the input file handle
    yyrestart(frg_stream_get_file(mut_source->stream));
  }

  // Actually parse the text
  frg_ast_node_t* node = NULL;
  if (yyparse(&node) != 0 && node != NULL) {
    frg_ast_node_destroy(node);
    node = NULL;
  }

  // Clean up after parsing
  if (frg_stream_input_is_buffer(mut_source->stream)) {
    yypop_buffer_state();
  }

  yylex_destroy();

  // Unset globals
  _frg_parsing_current_path           = NULL;
  _frg_parsing_current_offset         = 0;
  _frg_parsing_current_message_buffer = NULL;

  // Return AST
  return node;
}
