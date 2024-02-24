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

#include <forge/enums.h>
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>

void frg_stream_output_init();

void frg_stream_output_cleanup();

void frg_stream_output_set_console_color_enabled(bool enabled);

void frg_stream_output_set_console_unicode_enabled(bool enabled);

typedef struct {
  frg_stream_output_flags_t _flags;
  FILE* _file;
  GString* _text;
} frg_stream_output_t;

frg_stream_output_t* frg_stream_output_new_file(FILE* mut_file,
                                                frg_stream_output_flags_t flags);

frg_stream_output_t* frg_stream_output_new_buffer(frg_stream_output_flags_t flags);

void frg_stream_output_destroy(frg_stream_output_t* output);

GString* frg_stream_output_destroy_take_buffer(frg_stream_output_t* output);

void frg_stream_output_write_character(frg_stream_output_t* mut_output, char value);

void frg_stream_output_write_string(frg_stream_output_t* mut_output, const char* value);

void frg_stream_output_write_string_with_length(frg_stream_output_t* mut_output,
                                                const char* value,
                                                size_t length);

void frg_stream_output_write_printf(frg_stream_output_t* mut_output,
                                    const char* format,
                                    ...);

void frg_stream_output_write_vprintf(frg_stream_output_t* mut_output,
                                     const char* format,
                                     va_list args);

void frg_stream_output_set_color(frg_stream_output_t* mut_output,
                                 frg_stream_output_color_t color);

const char* frg_stream_output_choose_ascii_or_unicode(
  const frg_stream_output_t* mut_output, const char* ascii, const char* unicode);

frg_stream_output_t* frg_stream_output_get_stdout();

frg_stream_output_t* frg_stream_output_get_stderr();
