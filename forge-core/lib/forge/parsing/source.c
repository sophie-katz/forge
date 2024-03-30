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
#include <forge/parsing/source.h>

frg_parsing_source_t* frg_parsing_source_new(const char* path,
                                             frg_stream_input_t* stream) {
  frg_assert_string_non_empty(path);
  frg_assert_pointer_non_null(stream);

  frg_parsing_source_t* source = frg_malloc(sizeof(frg_parsing_source_t));

  source->path                 = path;
  source->stream               = stream;

  return source;
}

void frg_parsing_source_destroy(frg_parsing_source_t* source) {
  frg_assert_pointer_non_null(source);

  frg_stream_input_destroy(source->stream);

  frg_free(source);
}

void _frg_parsing_source_seek_to_line_start(frg_parsing_source_t* mut_source,
                                            const frg_parsing_location_t* start) {
  frg_assert_pointer_non_null(mut_source);
  frg_assert_int_greater_than_or_equal_to(start->line_number, 1);
  frg_assert_int_greater_than_or_equal_to(start->column_number, 1);
  frg_assert(strcmp(start->path, mut_source->path) == 0);
  frg_assert_int_less_than(start->offset, frg_stream_get_length(mut_source->stream));

  frg_stream_input_seek_to_offset(
    mut_source->stream,
    (start->column_number > start->offset) ? 0 : start->offset - start->column_number);

  while (!frg_stream_input_is_at_end(mut_source->stream)) {
    int c = frg_stream_input_peek_character(mut_source->stream);
    if (c != '\n' && c != '\r') {
      break;
    }

    frg_stream_input_read_character(mut_source->stream);
  }
}

GString* frg_parsing_source_load_range(frg_parsing_source_t* mut_source,
                                       const frg_parsing_range_t* range) {
  frg_assert_pointer_non_null(mut_source);
  frg_assert_int_greater_than_or_equal_to(range->start.line_number, 1);
  frg_assert_int_greater_than_or_equal_to(range->start.column_number, 1);
  frg_assert(strcmp(range->start.path, mut_source->path) == 0);
  frg_assert_int_less_than(range->start.offset,
                           frg_stream_get_length(mut_source->stream));
  frg_assert_int_less_than_or_equal_to(range->start.offset + range->length,
                                       frg_stream_get_length(mut_source->stream));

  _frg_parsing_source_seek_to_line_start(mut_source, &range->start);

  size_t string_length = range->start.column_number + range->length - 1;

  GString* result      = g_string_new(NULL);

  g_string_set_size(result, string_length);

  size_t read_bytes
    = frg_stream_input_read_into_buffer(mut_source->stream, result->str, string_length);

  frg_assert_int_equal_to(read_bytes, string_length);

  while (!frg_stream_input_is_at_end(mut_source->stream)) {
    int c = frg_stream_input_read_character(mut_source->stream);
    if (c == '\n' || c == '\r') {
      break;
    } else {
      g_string_append_c(result, c);
    }
  }

  return result;
}
