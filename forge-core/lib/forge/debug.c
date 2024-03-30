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

#include <forge/debug.h>
#include <stdarg.h>

void frg_debug_print_newline(frg_stream_output_t* mut_stream,
                             frg_indentation_t indentation) {
  frg_stream_output_write_character(mut_stream, '\n');

  for (size_t i = 0; i < indentation; i++) {
    frg_stream_output_write_character(mut_stream, ' ');
  }
}

void frg_debug_print_node(frg_stream_output_t* mut_stream, const char* format, ...) {
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
  frg_stream_output_write_printf(mut_stream, "[");

  va_list args;
  va_start(args, format);

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_GREEN);
  frg_stream_output_write_vprintf(mut_stream, format, args);

  va_end(args);

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
  frg_stream_output_write_printf(mut_stream, "]");

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
}

void frg_debug_print_property(frg_stream_output_t* mut_stream,
                              const char* label,
                              const char* format,
                              ...) {
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_CYAN);
  frg_stream_output_write_printf(mut_stream, "%s = ", label);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);

  if (format != NULL) {
    va_list args;
    va_start(args, format);

    frg_stream_output_write_vprintf(mut_stream, format, args);

    va_end(args);
  }
}

void frg_debug_print_property_with_index(frg_stream_output_t* mut_stream,
                                         const char* label,
                                         size_t index,
                                         const char* format,
                                         ...) {
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_CYAN);
  frg_stream_output_write_printf(mut_stream, "%s", label);

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
  frg_stream_output_write_printf(mut_stream, "[%lu] = ", index);

  if (format != NULL) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);

    va_list args;
    va_start(args, format);

    frg_stream_output_write_vprintf(mut_stream, format, args);

    va_end(args);
  }
}
