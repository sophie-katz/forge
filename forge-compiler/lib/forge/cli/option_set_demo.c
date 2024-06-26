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

#include <forge/cli/option_set.h>

bool callback(frg_message_buffer_t* mut_message_buffer,
              void* mut_user_data,
              const char* value) {
  return true;
}

int main() {
  frg_stream_output_init();

  frg_cli_option_set_t* option_set = frg_cli_option_set_new();

  frg_cli_option_set_add_option(
    option_set, frg_cli_option_new_flag("long-flag", "A long-only flag.", callback));

  frg_cli_option_set_add_option(
    option_set,
    frg_cli_option_new_flag_short(
      'f', "short-flag", "A long and short flag.", callback));

  frg_cli_option_set_add_option(
    option_set,
    frg_cli_option_new_argument(
      "long-argument", "value", "A long-only argument.", callback));

  frg_cli_option_set_add_option(
    option_set,
    frg_cli_option_new_argument_short(
      'a', "short-argument", "value", "A long and short argument.", callback));

  frg_cli_option_t* option = frg_cli_option_new_choice(
    "long-choice", "value", "A long-only choice.", callback);

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-1", "A choice."));

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-2", "Another choice."));

  frg_cli_option_set_add_option(option_set, option);

  option = frg_cli_option_new_choice_short(
    'g', "short-choice", "value", "A long and short choice.", callback);

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-1", "A choice."));

  frg_cli_option_add_choice(option, frg_cli_choice_new("option-2", "Another choice."));

  frg_cli_option_set_add_option(option_set, option);

  frg_cli_option_set_print_help(frg_stream_output_get_stdout(), option_set);

  frg_cli_option_set_destroy(option_set);

  frg_stream_output_cleanup();

  return 0;
}
