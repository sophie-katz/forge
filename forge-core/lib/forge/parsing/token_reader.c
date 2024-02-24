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
#include <forge/memory.h>
#include <forge/parsing/token_reader.h>
#include <string.h>

frg_parsing_token_reader_t* frg_parsing_token_reader_new(
  const frg_parsing_location_t* start, const char* text) {
  frg_assert_string_non_empty(start->path);
  frg_assert_int_greater_than_or_equal_to(start->line_number, 1);
  frg_assert_int_greater_than_or_equal_to(start->column_number, 1);
  frg_assert_string_non_empty(text);

  frg_parsing_token_reader_t* reader = frg_malloc(sizeof(frg_parsing_token_reader_t));

  reader->_start                     = *start;
  reader->_text                      = text;
  reader->_current_location          = *start;
  reader->_current_character         = text;

  return reader;
}

void frg_parsing_token_reader_destroy(frg_parsing_token_reader_t* reader) {
  frg_assert(reader != NULL);

  frg_free(reader);
}

const frg_parsing_location_t* frg_parsing_token_reader_get_current_location(
  const frg_parsing_token_reader_t* reader) {
  frg_assert(reader != NULL);

  return &reader->_current_location;
}

char frg_parsing_token_reader_get_current_character(
  const frg_parsing_token_reader_t* reader) {
  frg_assert(reader != NULL);

  return *reader->_current_character;
}

void frg_parsing_token_reader_step(frg_parsing_token_reader_t* mut_reader) {
  frg_assert(mut_reader != NULL);

  if (*mut_reader->_current_character == 0) {
    return;
  } else if (*mut_reader->_current_character == '\n') {
    mut_reader->_current_location.line_number++;
    mut_reader->_current_location.column_number = 1;
  } else {
    mut_reader->_current_location.column_number++;
  }

  mut_reader->_current_location.offset++;
  mut_reader->_current_character++;
}

frg_parsing_token_reader_t* frg_parsing_token_reader_clone(
  const frg_parsing_token_reader_t* reader) {
  frg_assert_pointer_non_null(reader);

  frg_parsing_token_reader_t* clone
    = frg_parsing_token_reader_new(&reader->_start, reader->_text);
  clone->_current_location  = reader->_current_location;
  clone->_current_character = reader->_current_character;

  return clone;
}

void frg_parsing_token_reader_restore(frg_parsing_token_reader_t* mut_reader,
                                      const frg_parsing_token_reader_t* state) {
  frg_assert_pointer_non_null(mut_reader);
  frg_assert_pointer_non_null(state);
  frg_assert_int_equal_to(mut_reader->_start.offset, state->_start.offset);
  frg_assert_int_equal_to(mut_reader->_start.line_number, state->_start.line_number);
  frg_assert_int_equal_to(mut_reader->_start.column_number,
                          state->_start.column_number);
  frg_assert_int_equal_to(mut_reader->_start.path, state->_start.path);
  frg_assert_int_equal_to(mut_reader->_text, state->_text);

  mut_reader->_current_location  = state->_current_location;
  mut_reader->_current_character = state->_current_character;
}
