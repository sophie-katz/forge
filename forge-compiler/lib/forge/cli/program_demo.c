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

#include <forge/cli/program.h>

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

int no_commands() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_program_t* program
    = frg_cli_program_new("Forge",
                          "forge",
                          "source files",
                          0,
                          1,
                          0,
                          "alpha",
                          g_string_new("Some more version\ninformation"),
                          NULL);

  frg_cli_option_t* option
    = frg_cli_option_new_flag("debug", "Enable debug mode", callback_option);

  frg_cli_option_set_add_option(program->global_options, option);

  frg_cli_program_try_print_help(
    frg_stream_output_get_stdout(), message_buffer, program, NULL);

  frg_cli_program_destroy(program);
  frg_message_buffer_destroy(message_buffer);

  return 0;
}

int one_command_global() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_program_t* program
    = frg_cli_program_new("Forge",
                          "forge",
                          NULL,
                          0,
                          1,
                          0,
                          "alpha",
                          g_string_new("Some more version\ninformation"),
                          NULL);

  frg_cli_option_t* option
    = frg_cli_option_new_flag("debug", "Enable debug mode", callback_option);

  frg_cli_option_set_add_option(program->global_options, option);

  frg_cli_command_t* command
    = frg_cli_command_new("command", NULL, "A command", callback_command);

  option = frg_cli_option_new_flag("option", "An option", callback_option);

  frg_cli_option_set_add_option(command->option_set, option);

  frg_cli_program_add_command(program, command);

  frg_cli_program_try_print_help(
    frg_stream_output_get_stdout(), message_buffer, program, NULL);

  frg_cli_program_destroy(program);
  frg_message_buffer_destroy(message_buffer);

  return 0;
}

int one_command_command() {
  frg_message_buffer_t* message_buffer = frg_message_buffer_new();

  frg_cli_program_t* program
    = frg_cli_program_new("Forge",
                          "forge",
                          NULL,
                          0,
                          1,
                          0,
                          "alpha",
                          g_string_new("Some more version\ninformation"),
                          NULL);

  frg_cli_option_t* option
    = frg_cli_option_new_flag("debug", "Enable debug mode", callback_option);

  frg_cli_option_set_add_option(program->global_options, option);

  frg_cli_command_t* command = frg_cli_command_new(
    "command", "positional argument", "A command", callback_command);

  option = frg_cli_option_new_flag("option", "An option", callback_option);

  frg_cli_option_set_add_option(command->option_set, option);

  frg_cli_program_add_command(program, command);

  frg_cli_program_try_print_help(
    frg_stream_output_get_stdout(), message_buffer, program, "command");

  frg_cli_program_destroy(program);
  frg_message_buffer_destroy(message_buffer);

  return 0;
}

int main() {
  frg_stream_output_init();

  printf("=== NO COMMANDS ===\n\n");

  int result = no_commands();
  if (result != 0) {
    return result;
  }

  printf("\n\n=== ONE COMMAND (GLOBAL) ===\n\n");

  result = one_command_global();
  if (result != 0) {
    return result;
  }

  printf("\n\n=== ONE COMMAND (COMMAND) ===\n\n");

  result = one_command_command();
  if (result != 0) {
    return result;
  }

  frg_stream_output_cleanup();

  return 0;
}
