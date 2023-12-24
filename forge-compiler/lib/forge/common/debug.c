// Copyright (c) 2023 Sophie Katz
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

#include <forge/common/color.h>
#include <forge/common/debug.h>
#include <forge/common/stream.h>
#include <stdarg.h>
#include <stdio.h>

void frg_debug_print_newline(frg_indent_t indent) {
    fputc('\n', FRG_STREAM_DEFAULT);

    for (size_t i = 0; i < indent; i++) {
        fputc(' ', FRG_STREAM_DEFAULT);
    }
}

void frg_debug_print_node(const char* format, ...) {
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "[");

    va_list args;
    va_start(args, format);

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_GREEN);
    vfprintf(FRG_STREAM_DEFAULT, format, args);

    va_end(args);

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "]");

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
}

void frg_debug_print_property(const char* label, const char* format, ...) {
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_CYAN);
    fprintf(FRG_STREAM_DEFAULT, "%s = ", label);
    
    if (format != NULL) {
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

        va_list args;
        va_start(args, format);

        vfprintf(FRG_STREAM_DEFAULT, format, args);

        va_end(args);
    }
}

void frg_debug_print_property_with_index(
    const char* label,
    size_t index,
    const char* format,
    ...
) {
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_CYAN);
    fprintf(FRG_STREAM_DEFAULT, "%s", label);

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "[%lu] = ", index);
    
    if (format != NULL) {
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

        va_list args;
        va_start(args, format);

        vfprintf(FRG_STREAM_DEFAULT, format, args);

        va_end(args);
    }
}
