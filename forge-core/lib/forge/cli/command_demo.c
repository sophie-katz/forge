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

#include <forge/cli/command.h>

int callback_command(frg_message_buffer_t* mut_message_buffer,
                     void* mut_user_data,
                     const frg_cli_program_t* program,
                     const GList* positional_arguments) {
  return 0;
}

bool callback_option(frg_message_buffer_t* mut_message_buffer,
                     void* mut_user_data,
                     const char* value) {
  return true;
}

int main() {
  frg_stream_output_init();

  frg_cli_command_t* command = frg_cli_command_new(
    "command", NULL, "A description of the command", callback_command);

  frg_cli_command_print_help(frg_stream_output_get_stdout(), command);

  frg_cli_command_destroy(command);

  frg_stream_output_cleanup();

  return 0;
}
