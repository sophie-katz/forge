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

#include <forge/assert.h>
#include <forge/cli/option.h>
#include <forge/memory.h>
#include <forge/messages/codes.h>

bool frg_cli_is_valid_short_name(char short_name) {
  if (short_name >= 'a' && short_name <= 'z') {
    return true;
  } else if (short_name >= 'A' && short_name <= 'Z') {
    return true;
  } else {
    return false;
  }
}

frg_cli_option_t* frg_cli_option_new_flag(const char* long_name,
                                          const char* help,
                                          frg_cli_option_callback_t callback) {
  frg_assert_string_non_empty(long_name);
  frg_assert_string_non_empty(help);
  frg_assert_pointer_non_null(callback);

  frg_cli_option_t* option = frg_malloc(sizeof(frg_cli_option_t));

  option->short_name       = FRG_CLI_OPTION_SHORT_NAME_NULL;
  option->long_name        = long_name;
  option->value_name       = NULL;
  option->help             = help;
  option->choices          = NULL;
  option->_callback        = callback;

  return option;
}

frg_cli_option_t* frg_cli_option_new_flag_short(char short_name,
                                                const char* long_name,
                                                const char* help,
                                                frg_cli_option_callback_t callback) {
  frg_assert(frg_cli_is_valid_short_name(short_name));
  frg_assert_string_non_empty(long_name);
  frg_assert_string_non_empty(help);
  frg_assert_pointer_non_null(callback);

  frg_cli_option_t* option = frg_malloc(sizeof(frg_cli_option_t));

  option->short_name       = short_name;
  option->long_name        = long_name;
  option->value_name       = NULL;
  option->help             = help;
  option->choices          = NULL;
  option->_callback        = callback;

  return option;
}

frg_cli_option_t* frg_cli_option_new_argument(const char* long_name,
                                              const char* value_name,
                                              const char* help,
                                              frg_cli_option_callback_t callback) {
  frg_assert_string_non_empty(long_name);
  frg_assert_string_non_empty(value_name);
  frg_assert_string_non_empty(help);
  frg_assert_pointer_non_null(callback);

  frg_cli_option_t* option = frg_malloc(sizeof(frg_cli_option_t));

  option->short_name       = FRG_CLI_OPTION_SHORT_NAME_NULL;
  option->long_name        = long_name;
  option->value_name       = value_name;
  option->help             = help;
  option->choices          = NULL;
  option->_callback        = callback;

  return option;
}

frg_cli_option_t* frg_cli_option_new_argument_short(
  char short_name,
  const char* long_name,
  const char* value_name,
  const char* help,
  frg_cli_option_callback_t callback) {
  frg_assert(frg_cli_is_valid_short_name(short_name));
  frg_assert_string_non_empty(long_name);
  frg_assert_string_non_empty(value_name);
  frg_assert_string_non_empty(help);
  frg_assert_pointer_non_null(callback);

  frg_cli_option_t* option = frg_malloc(sizeof(frg_cli_option_t));

  option->short_name       = short_name;
  option->long_name        = long_name;
  option->value_name       = value_name;
  option->help             = help;
  option->choices          = NULL;
  option->_callback        = callback;

  return option;
}

frg_cli_option_t* frg_cli_option_new_choice(const char* long_name,
                                            const char* value_name,
                                            const char* help,
                                            frg_cli_option_callback_t callback) {
  frg_assert_string_non_empty(long_name);
  frg_assert_string_non_empty(value_name);
  frg_assert_string_non_empty(help);
  frg_assert_pointer_non_null(callback);

  frg_cli_option_t* option = frg_malloc(sizeof(frg_cli_option_t));

  option->short_name       = FRG_CLI_OPTION_SHORT_NAME_NULL;
  option->long_name        = long_name;
  option->value_name       = value_name;
  option->help             = help;
  option->choices          = NULL;
  option->_callback        = callback;

  return option;
}

frg_cli_option_t* frg_cli_option_new_choice_short(char short_name,
                                                  const char* long_name,
                                                  const char* value_name,
                                                  const char* help,
                                                  frg_cli_option_callback_t callback) {
  frg_assert(frg_cli_is_valid_short_name(short_name));
  frg_assert_string_non_empty(long_name);
  frg_assert_string_non_empty(value_name);
  frg_assert_string_non_empty(help);
  frg_assert_pointer_non_null(callback);

  frg_cli_option_t* option = frg_malloc(sizeof(frg_cli_option_t));

  option->short_name       = short_name;
  option->long_name        = long_name;
  option->value_name       = value_name;
  option->help             = help;
  option->choices          = NULL;
  option->_callback        = callback;

  return option;
}

void frg_cli_option_destroy(frg_cli_option_t* option) {
  frg_assert_pointer_non_null(option);

  for (GList* choice = option->choices; choice != NULL; choice = choice->next) {
    frg_cli_choice_destroy(choice->data);
  }

  g_list_free(option->choices);

  frg_free(option);
}

void frg_cli_option_add_choice(frg_cli_option_t* mut_option, frg_cli_choice_t* choice) {
  frg_assert_pointer_non_null(mut_option);
  frg_assert_pointer_non_null(choice);

  mut_option->choices = g_list_append(mut_option->choices, choice);
}

void frg_cli_option_print_help(frg_stream_output_t* mut_stream,
                               const frg_cli_option_t* option) {
  frg_assert_pointer_non_null(option);

  frg_stream_output_write_string(mut_stream, "  ");

  if (option->short_name != FRG_CLI_OPTION_SHORT_NAME_NULL) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(mut_stream, "-%c", option->short_name);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_write_string(mut_stream, ", ");
  }

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
  frg_stream_output_write_printf(mut_stream, "--%s", option->long_name);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);

  if (option->value_name != NULL) {
    frg_stream_output_write_printf(mut_stream, " <%s>", option->value_name);
  }

  frg_stream_output_write_printf(mut_stream, "\n    %s\n", option->help);

  if (option->choices != NULL) {
    frg_stream_output_write_string(mut_stream, "    ");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
    frg_stream_output_write_string(mut_stream, "Choices:");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_write_string(mut_stream, "\n");

    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    for (GList* choice = option->choices; choice != NULL; choice = choice->next) {
      frg_cli_choice_print_help(mut_stream, (const frg_cli_choice_t*)choice->data);
    }
  }
}

bool _frg_cli_option_check_matches_argument(const frg_cli_option_t* option,
                                            const char* arg) {
  frg_assert_pointer_non_null(option);
  frg_assert_string_non_empty(arg);
  frg_assert_int_equal_to(arg[0], '-');

  if (arg[1] == '-') {
    return strcmp(arg + 2, option->long_name) == 0;
  } else {
    return arg[1] == option->short_name;
  }
}

bool frg_cli_option_parse_next(frg_message_buffer_t* mut_message_buffer,
                               int* mut_argi,
                               void* mut_user_data,
                               const frg_cli_option_t* option,
                               int argc,
                               const char** argv) {
  // Error checking
  frg_assert_pointer_non_null(option);
  frg_assert_pointer_non_null(mut_argi);
  frg_assert_pointer_non_null(argv);
  frg_assert_int_greater_than_or_equal_to(*mut_argi, 0);
  frg_assert_int_greater_than(argc, 0);
  frg_assert_int_less_than_or_equal_toss_than(*mut_argi, argc);
  frg_assert_string_non_empty(argv[*mut_argi]);
  frg_assert_int_equal_to(argv[*mut_argi][0], '-');
  frg_assert(_frg_cli_option_check_matches_argument(option, argv[*mut_argi]));

  // Increment argument index
  (*mut_argi)++;

  if (option->value_name == NULL) {
    // If the argument does not take a value, call the callback with NULL
    return option->_callback(mut_message_buffer, mut_user_data, NULL);
  } else {
    // If the argument does take a value, make sure there is a value to parse
    if (*mut_argi >= argc) {
      frg_message_emit_fc_1_argument_expects_value(
        mut_message_buffer, argv[*mut_argi - 1], option->value_name);
      return false;
    }

    frg_assert_string_non_empty(argv[*mut_argi]);

    if (argv[*mut_argi][0] == '-') {
      frg_message_emit_fc_2_argument_expects_value_not_argument(
        mut_message_buffer, argv[*mut_argi - 1], option->value_name, argv[*mut_argi]);
      return false;
    }

    // If the argument has a set of choices, make sure the value matches one of them
    if (option->choices != NULL) {
      bool found = false;

      for (GList* choice = option->choices; choice != NULL; choice = choice->next) {
        if (strcmp(argv[*mut_argi], ((frg_cli_choice_t*)choice->data)->name) == 0) {
          found = true;
          break;
        }
      }

      if (!found) {
        frg_message_emit_fc_3_invalid_choice(
          mut_message_buffer, argv[*mut_argi - 1], option);

        return false;
      }
    }

    // Call the callback with the value
    bool result = option->_callback(mut_message_buffer, mut_user_data, argv[*mut_argi]);

    // Increment argument index
    (*mut_argi)++;

    return result;
  }
}
