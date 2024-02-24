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

#include <forge/debug.h>
#include <stdio.h>

int main() {
  frg_stream_output_init();

  frg_debug_print_node(frg_stream_output_get_stdout(), "node %d", 0);
  frg_debug_print_newline(frg_stream_output_get_stdout(), 2);
  frg_debug_print_property(frg_stream_output_get_stdout(), "a", "hi");
  frg_debug_print_newline(frg_stream_output_get_stdout(), 2);
  frg_debug_print_property(frg_stream_output_get_stdout(), "b", "%d", 5);
  frg_debug_print_newline(frg_stream_output_get_stdout(), 2);
  frg_debug_print_property(frg_stream_output_get_stdout(), "c", NULL);
  frg_debug_print_node(frg_stream_output_get_stdout(), "node %d", 1);
  frg_debug_print_newline(frg_stream_output_get_stdout(), 4);
  frg_debug_print_property_with_index(frg_stream_output_get_stdout(), "d", 0, "%d", 1);
  frg_debug_print_newline(frg_stream_output_get_stdout(), 4);
  frg_debug_print_property_with_index(frg_stream_output_get_stdout(), "d", 1, "%d", 2);
  frg_debug_print_newline(frg_stream_output_get_stdout(), 4);
  frg_debug_print_property_with_index(frg_stream_output_get_stdout(), "d", 2, "%d", 3);
  printf("\n");

  frg_stream_output_cleanup();

  return 0;
}
