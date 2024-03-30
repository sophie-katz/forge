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
#include <forge/cli/command.h>
#include <forge/memory.h>

frg_cli_command_t* frg_cli_command_new(const char* name,
                                       const char* positional_arguments_name,
                                       const char* help,
                                       frg_cli_command_callback_t callback) {
  frg_assert_string_non_empty(name);

  if (positional_arguments_name != NULL) {
    frg_assert_string_non_empty(positional_arguments_name);
  }

  frg_assert_string_non_empty(help);
  frg_assert_pointer_non_null(callback);

  frg_cli_command_t* command         = frg_malloc(sizeof(frg_cli_command_t));

  command->name                      = name;
  command->positional_arguments_name = positional_arguments_name;
  command->help                      = help;
  command->option_set                = NULL;
  command->callback                  = callback;
  command->option_set                = frg_cli_option_set_new();

  return command;
}

void frg_cli_command_destroy(frg_cli_command_t* command) {
  frg_assert_pointer_non_null(command);

  frg_cli_option_set_destroy(command->option_set);

  frg_free(command);
}

void frg_cli_command_print_help(frg_stream_output_t* mut_stream,
                                const frg_cli_command_t* command) {
  frg_stream_output_write_printf(mut_stream, "%s\n", command->help);
}
