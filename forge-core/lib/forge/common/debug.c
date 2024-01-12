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

#include <forge/common/color.h>
#include <forge/common/debug.h>
#include <forge/common/stream.h>
#include <stdarg.h>
#include <stdio.h>

void frg_debug_print_newline(FILE* file, frg_indent_t indent) {
    fputc('\n', file);

    for (size_t i = 0; i < indent; i++) {
        fputc(' ', file);
    }
}

void frg_debug_print_node(FILE* file, const char* format, ...) {
    frg_color_set(file, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(file, "[");

    va_list args;
    va_start(args, format);

    frg_color_set(file, FRG_COLOR_ID_BOLD);
    frg_color_set(file, FRG_COLOR_ID_GREEN);
    vfprintf(file, format, args);

    va_end(args);

    frg_color_set(file, FRG_COLOR_ID_RESET);
    frg_color_set(file, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(file, "]");

    frg_color_set(file, FRG_COLOR_ID_RESET);
}

void frg_debug_print_property(FILE* file, const char* label, const char* format, ...) {
    frg_color_set(file, FRG_COLOR_ID_BOLD);
    frg_color_set(file, FRG_COLOR_ID_CYAN);
    fprintf(file, "%s = ", label);
    
    if (format != NULL) {
        frg_color_set(file, FRG_COLOR_ID_RESET);

        va_list args;
        va_start(args, format);

        vfprintf(file, format, args);

        va_end(args);
    }
}

void frg_debug_print_property_with_index(
    FILE* file,
    const char* label,
    size_t index,
    const char* format,
    ...
) {
    frg_color_set(file, FRG_COLOR_ID_BOLD);
    frg_color_set(file, FRG_COLOR_ID_CYAN);
    fprintf(file, "%s", label);

    frg_color_set(file, FRG_COLOR_ID_RESET);
    frg_color_set(file, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(file, "[%lu] = ", index);
    
    if (format != NULL) {
        frg_color_set(file, FRG_COLOR_ID_RESET);

        va_list args;
        va_start(args, format);

        vfprintf(file, format, args);

        va_end(args);
    }
}
