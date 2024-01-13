// Copyright (c) 2024 Sophie Katz
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

#include <stdio.h>
#include <glib.h>
#include <stdbool.h>
#include <forge/common/enums.h>

typedef struct {
    frg_stream_output_flags_t flags;
    FILE* file;
    GString* text;
} frg_stream_output_t;

frg_stream_output_t* frg_stream_output_new_file(
    FILE* file,
    frg_stream_output_flags_t flags
);

frg_stream_output_t* frg_stream_output_new_buffer(
    frg_stream_output_flags_t flags
);

void frg_stream_output_destroy(frg_stream_output_t** output);

GString* frg_stream_output_destroy_take_buffer(frg_stream_output_t** output);

void frg_stream_output_write_char(
    frg_stream_output_t* output,
    char value
);

void frg_stream_output_write_string(
    frg_stream_output_t* output,
    const char* value
);

void frg_stream_output_write_string_with_length(
    frg_stream_output_t* output,
    const char* value,
    size_t length
);

void frg_stream_output_write_printf(
    frg_stream_output_t* output,
    const char* format,
    ...
);

void frg_stream_output_write_vprintf(
    frg_stream_output_t* output,
    const char* format,
    va_list args
);

void frg_stream_output_set_color(
    frg_stream_output_t* output,
    frg_stream_output_color_id_t id
);

const char* frg_stream_output_choose_unicode(
    const frg_stream_output_t* output,
    const char* ascii,
    const char* unicode
);

frg_stream_output_t* frg_stream_output_get_stdout();

frg_stream_output_t* frg_stream_output_get_stderr();

// TODO: Rename these
void frg_stream_output_set_console_color(
    bool enabled
);

void frg_stream_output_set_console_unicode(
    bool enabled
);
