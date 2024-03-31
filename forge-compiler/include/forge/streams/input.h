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

#pragma once

#include <forge/enums.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  frg_stream_input_flags_t _flags;
  FILE* _file;
  char* _text;
  size_t _length;
  char* _text_offset;
} frg_stream_input_t;

frg_stream_input_t* frg_stream_input_new_file(FILE* file,
                                              frg_stream_input_flags_t flags);

frg_stream_input_t* frg_stream_input_new_buffer(char* text,
                                                frg_stream_input_flags_t flags);

frg_stream_input_t* frg_stream_input_new_buffer_with_length(
  char* text, size_t length, frg_stream_input_flags_t flags);

void frg_stream_input_destroy(frg_stream_input_t* input);

bool frg_stream_input_is_buffer(const frg_stream_input_t* input);

bool frg_stream_input_is_file(const frg_stream_input_t* input);

bool frg_stream_input_is_at_end(const frg_stream_input_t* input);

bool frg_stream_input_has_error(const frg_stream_input_t* input);

char frg_stream_input_peek_character(frg_stream_input_t* mut_input);

char frg_stream_input_read_character(frg_stream_input_t* mut_input);

size_t frg_stream_input_read_into_buffer(frg_stream_input_t* mut_input,
                                         char* mut_buffer,
                                         size_t length);

size_t frg_stream_input_get_offset(const frg_stream_input_t* input);

void frg_stream_input_seek_to_offset(frg_stream_input_t* mut_input, size_t offset);

size_t frg_stream_get_length(const frg_stream_input_t* input);

char* frg_stream_get_buffer(frg_stream_input_t* mut_input);

FILE* frg_stream_get_file(frg_stream_input_t* mut_input);
