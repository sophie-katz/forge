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
#include <forge/log.h>

bool frg_global_log_enabled = false;

frg_stream_output_t* frg_log_stream_get() {
  return frg_stream_output_get_stdout();
}

bool _frg_log_event(
  const char* file, int line_number, const char* function, const char* format, ...) {
  frg_assert_string_non_empty(file);
  frg_assert_int_positive(line_number);
  frg_assert_string_non_empty(function);
  frg_assert_string_non_empty(format);

  if (frg_global_log_enabled) {
    frg_stream_output_set_color(frg_log_stream_get(),
                                FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
    frg_stream_output_write_printf(
      frg_log_stream_get(), "[%s:%i:%s] ", file, line_number, function);
    frg_stream_output_set_color(frg_log_stream_get(), FRG_STREAM_OUTPUT_COLOR_RESET);

    frg_stream_output_set_color(frg_log_stream_get(), FRG_STREAM_OUTPUT_COLOR_BOLD);
    va_list args;
    va_start(args, format);
    frg_stream_output_write_vprintf(frg_log_stream_get(), format, args);
    va_end(args);
    frg_stream_output_set_color(frg_log_stream_get(), FRG_STREAM_OUTPUT_COLOR_RESET);

    frg_stream_output_write_character(frg_log_stream_get(), '\n');

    return true;
  } else {
    return false;
  }
}

void frg_log_property(const char* name, const char* format, ...) {
  frg_assert_string_non_empty(name);

  frg_stream_output_set_color(frg_log_stream_get(),
                              FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
  frg_stream_output_write_string(frg_log_stream_get(), "  [");

  frg_stream_output_set_color(frg_log_stream_get(), FRG_STREAM_OUTPUT_COLOR_CYAN);
  frg_stream_output_write_string(frg_log_stream_get(), name);

  frg_stream_output_set_color(frg_log_stream_get(),
                              FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
  frg_stream_output_write_string(frg_log_stream_get(), "] ");

  frg_stream_output_set_color(frg_log_stream_get(), FRG_STREAM_OUTPUT_COLOR_RESET);

  if (format != NULL) {
    frg_assert_string_non_empty(format);

    va_list args;
    va_start(args, format);
    frg_stream_output_write_vprintf(frg_log_stream_get(), format, args);
    va_end(args);

    frg_stream_output_write_character(frg_log_stream_get(), '\n');
  }
}
