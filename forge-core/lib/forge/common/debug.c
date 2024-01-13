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

#include <forge/common/debug.h>
#include <stdarg.h>
#include <stdio.h>

void frg_debug_print_newline(frg_stream_output_t* stream, frg_indent_t indent) {
    frg_stream_output_write_char(stream, '\n');

    for (size_t i = 0; i < indent; i++) {
        frg_stream_output_write_char(stream, ' ');
    }
}

void frg_debug_print_node(frg_stream_output_t* stream, const char* format, ...) {
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_BLACK);
    frg_stream_output_write_printf(stream, "[");

    va_list args;
    va_start(args, format);

    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_GREEN);
    frg_stream_output_write_vprintf(stream, format, args);

    va_end(args);

    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_BLACK);
    frg_stream_output_write_printf(stream, "]");

    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
}

void frg_debug_print_property(frg_stream_output_t* stream, const char* label, const char* format, ...) {
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_CYAN);
    frg_stream_output_write_printf(stream, "%s = ", label);
    
    if (format != NULL) {
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);

        va_list args;
        va_start(args, format);

        frg_stream_output_write_vprintf(stream, format, args);

        va_end(args);
    }
}

void frg_debug_print_property_with_index(
    frg_stream_output_t* stream,
    const char* label,
    size_t index,
    const char* format,
    ...
) {
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_CYAN);
    frg_stream_output_write_printf(stream, "%s", label);

    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_BLACK);
    frg_stream_output_write_printf(stream, "[%lu] = ", index);
    
    if (format != NULL) {
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);

        va_list args;
        va_start(args, format);

        frg_stream_output_write_vprintf(stream, format, args);

        va_end(args);
    }
}
