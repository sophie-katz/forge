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

#include <forge/streams/output.h>
#include <stdio.h>

int main() {
  frg_stream_output_init();

  frg_stream_output_write_string(
    frg_stream_output_get_stdout(),
    frg_stream_output_choose_ascii_or_unicode(
      frg_stream_output_get_stdout(), "Unicode disabled\n", "Unicode enabled\n"));

  frg_stream_output_cleanup();

  return 0;
}
