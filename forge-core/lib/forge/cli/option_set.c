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
#include <forge/cli/option_set.h>
#include <forge/memory.h>
#include <forge/messages/codes.h>

int _frg_cli_get_short_name_index(char short_name) {
  if (short_name >= 'a' && short_name <= 'z') {
    return short_name - 'a';
  } else if (short_name >= 'A' && short_name <= 'Z') {
    return short_name - 'A' + 26;
  } else {
    frg_die("invalid short name '%c' (%i)", short_name, short_name);
  }
}

frg_cli_option_set_t* frg_cli_option_set_new() {
  frg_cli_option_set_t* option_set = frg_malloc(sizeof(frg_cli_option_set_t));

  option_set->_options             = NULL;
  memset(option_set->_options_index_by_short_name,
         0,
         sizeof(option_set->_options_index_by_short_name));
  option_set->_options_index_by_long_name
    = g_hash_table_new((GHashFunc)g_str_hash, (GEqualFunc)g_str_equal);

  return option_set;
}

void frg_cli_option_set_destroy(frg_cli_option_set_t* option_set) {
  frg_assert_pointer_non_null(option_set);

  for (GList* option = option_set->_options; option != NULL; option = option->next) {
    frg_cli_option_destroy(option->data);
  }

  g_list_free(option_set->_options);

  g_hash_table_destroy(option_set->_options_index_by_long_name);

  frg_free(option_set);
}

bool frg_cli_option_set_is_empty(const frg_cli_option_set_t* option_set) {
  frg_assert_pointer_non_null(option_set);

  return option_set->_options == NULL;
}

void frg_cli_option_set_add_option(frg_cli_option_set_t* mut_option_set,
                                   frg_cli_option_t* option) {
  frg_assert_pointer_non_null(mut_option_set);
  frg_assert_pointer_non_null(option);

  mut_option_set->_options = g_list_append(mut_option_set->_options, option);

  if (option->short_name != FRG_CLI_OPTION_SHORT_NAME_NULL) {
    int index = _frg_cli_get_short_name_index(option->short_name);

    if (mut_option_set->_options_index_by_short_name[index] != NULL) {
      frg_die("short option already added for '%c'", option->short_name);
    }

    mut_option_set->_options_index_by_short_name[index] = option;
  }

  if (g_hash_table_contains(mut_option_set->_options_index_by_long_name,
                            option->long_name)) {
    frg_die("long option already added for '%s'", option->long_name);
  } else {
    g_hash_table_insert(
      mut_option_set->_options_index_by_long_name, (void*)option->long_name, option);
  }
}

frg_cli_option_t* frg_cli_option_set_get_option_by_long_name(
  frg_cli_option_set_t* mut_option_set, const char* name) {
  frg_assert_pointer_non_null(mut_option_set);
  frg_assert_string_non_empty(name);
  frg_assert_int_not_equal_to(name[0], '-');

  return (frg_cli_option_t*)g_hash_table_lookup(
    mut_option_set->_options_index_by_long_name, name);
}

frg_cli_option_t* frg_cli_option_set_get_option_by_short_name(
  frg_cli_option_set_t* mut_option_set, char name) {
  frg_assert_pointer_non_null(mut_option_set);
  frg_assert(frg_cli_is_valid_short_name(name));

  return mut_option_set
    ->_options_index_by_short_name[_frg_cli_get_short_name_index(name)];
}

void frg_cli_option_set_print_help(frg_stream_output_t* mut_stream,
                                   const frg_cli_option_set_t* option_set) {
  frg_assert_pointer_non_null(option_set);

  bool first = true;

  for (GList* option = option_set->_options; option != NULL; option = option->next) {
    if (first) {
      first = false;
    } else {
      frg_stream_output_write_character(mut_stream, '\n');
    }

    frg_cli_option_print_help(mut_stream, (const frg_cli_option_t*)option->data);
  }
}

bool _frg_cli_option_set_parse_next_long(frg_message_buffer_t* mut_message_buffer,
                                         int* mut_argi,
                                         void* mut_user_data,
                                         const frg_cli_option_set_t* option_set,
                                         int argc,
                                         const char** argv,
                                         const char* name) {
  // Error checking
  frg_assert_pointer_non_null(option_set);
  frg_assert_pointer_non_null(mut_argi);
  frg_assert_pointer_non_null(argv);
  frg_assert_int_greater_than_or_equal_to(*mut_argi, 0);
  frg_assert_int_greater_than(argc, 0);
  frg_assert_int_less_than(*mut_argi, argc);
  frg_assert_string_non_empty(argv[*mut_argi]);
  frg_assert_int_equal_to(argv[*mut_argi][0], '-');
  frg_assert_int_equal_to(argv[*mut_argi][1], '-');
  frg_assert_pointer_non_null(name);

  if (name[0] == 0) {
    frg_message_emit_fc_7_invalid_argument_dash_dash(mut_message_buffer);
    return false;
  }

  frg_cli_option_t* option = frg_cli_option_set_get_option_by_long_name(
    (frg_cli_option_set_t*)option_set, name);

  if (option == NULL) {
    frg_message_emit_fc_9_unknown_long_argument(mut_message_buffer, name);
    return false;
  }

  return frg_cli_option_parse_next(
    mut_message_buffer, mut_argi, mut_user_data, option, argc, argv);
}

bool _frg_cli_option_set_parse_next_short(frg_message_buffer_t* mut_message_buffer,
                                          int* mut_argi,
                                          void* mut_user_data,
                                          const frg_cli_option_set_t* option_set,
                                          int argc,
                                          const char** argv,
                                          char name) {
  // Error checking
  frg_assert_pointer_non_null(option_set);
  frg_assert_pointer_non_null(mut_argi);
  frg_assert_pointer_non_null(argv);
  frg_assert_int_greater_than_or_equal_to(*mut_argi, 0);
  frg_assert_int_greater_than(argc, 0);
  frg_assert_int_less_than(*mut_argi, argc);
  frg_assert_string_non_empty(argv[*mut_argi]);
  frg_assert_int_equal_to(argv[*mut_argi][0], '-');

  frg_cli_option_t* option = frg_cli_option_set_get_option_by_short_name(
    (frg_cli_option_set_t*)option_set, name);

  if (option == NULL) {
    frg_message_emit_fc_9_unknown_long_argument(mut_message_buffer, name);

    return false;
  }

  return frg_cli_option_parse_next(
    mut_message_buffer, mut_argi, mut_user_data, option, argc, argv);
}

bool frg_cli_option_set_parse_next(frg_message_buffer_t* mut_message_buffer,
                                   int* mut_argi,
                                   void* mut_user_data,
                                   const frg_cli_option_set_t* option_set,
                                   int argc,
                                   const char** argv) {
  // Error checking
  frg_assert_pointer_non_null(option_set);
  frg_assert_pointer_non_null(mut_argi);
  frg_assert_pointer_non_null(argv);
  frg_assert_int_greater_than_or_equal_to(*mut_argi, 0);
  frg_assert_int_greater_than(argc, 0);
  frg_assert_int_less_than(*mut_argi, argc);
  frg_assert_string_non_empty(argv[*mut_argi]);
  frg_assert_int_equal_to(argv[*mut_argi][0], '-');

  // Try to parse the next argument
  if (argv[*mut_argi][1] == '-') {
    // It is a long argument like --argument
    return _frg_cli_option_set_parse_next_long(mut_message_buffer,
                                               mut_argi,
                                               mut_user_data,
                                               option_set,
                                               argc,
                                               argv,
                                               argv[*mut_argi] + 2);
  } else {
    if (argv[*mut_argi][2] != 0) {
      // It is a short argument like -abc
      frg_message_emit_fc_10_long_argument_with_single_dash(mut_message_buffer,
                                                            argv[*mut_argi]);
      return false;
    }

    // It is a short argument like -a
    return _frg_cli_option_set_parse_next_short(mut_message_buffer,
                                                mut_argi,
                                                mut_user_data,
                                                option_set,
                                                argc,
                                                argv,
                                                argv[*mut_argi][1]);
  }
}
