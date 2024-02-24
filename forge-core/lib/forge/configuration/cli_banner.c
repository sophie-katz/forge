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

#include <forge/configuration/cli_banner.h>
#include <forge/streams/output.h>

void frg_configuration_print_cli_banner() {
  frg_stream_output_set_color(frg_stream_output_get_stdout(),
                              FRG_STREAM_OUTPUT_COLOR_BOLD);
  frg_stream_output_set_color(frg_stream_output_get_stdout(),
                              FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW);
  frg_stream_output_write_printf(frg_stream_output_get_stdout(), "\n       \\ | / ,\n");
  frg_stream_output_write_printf(frg_stream_output_get_stdout(), "     `\n");
  frg_stream_output_set_color(frg_stream_output_get_stdout(),
                              FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED);
  frg_stream_output_write_printf(frg_stream_output_get_stdout(), "---------------\n");
  frg_stream_output_write_printf(frg_stream_output_get_stdout(), "  ----.      /\n");
  frg_stream_output_write_printf(frg_stream_output_get_stdout(), "      /___^__\\\n\n");
  frg_stream_output_set_color(frg_stream_output_get_stdout(),
                              FRG_STREAM_OUTPUT_COLOR_RESET);
}
