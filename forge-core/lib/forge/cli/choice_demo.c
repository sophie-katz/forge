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

#include <forge/cli/choice.h>

int main() {
  frg_stream_output_init();

  frg_cli_choice_t* choice
    = frg_cli_choice_new("option-name", "A description of the choice");

  frg_cli_choice_print_help(frg_stream_output_get_stdout(), choice);

  frg_cli_choice_destroy(choice);

  frg_stream_output_cleanup();

  return 0;
}
