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
#include <forge/ast/node.h>
#include <forge/configuration/commands/link.h>

int _frg_configuration_command_callback_link(frg_message_buffer_t* mut_message_buffer,
                                             void* mut_user_data,
                                             const frg_cli_program_t* program,
                                             const GList* positional_arguments) {
  frg_assert_pointer_non_null(program);

  frg_die("not yet implemented");
}

frg_cli_command_t* frg_configuration_new_command_link() {
  return frg_cli_command_new("link",
                             "object files",
                             "Links object files into executables or libraries.",
                             _frg_configuration_command_callback_link);
}
