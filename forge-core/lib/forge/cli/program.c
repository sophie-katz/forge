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
#include <forge/cli/program.h>
#include <forge/memory.h>
#include <forge/messages/codes.h>

frg_cli_program_t* frg_cli_program_new(const char* name,
                                       const char* binary_name,
                                       const char* positional_arguments_name,
                                       int version_major,
                                       int version_minor,
                                       int version_patch,
                                       const char* version_label,
                                       GString* version_details,
                                       frg_cli_program_callback_t callback) {
  frg_assert_string_non_empty(name);
  frg_assert_string_non_empty(binary_name);

  if (positional_arguments_name != NULL) {
    frg_assert_string_non_empty(positional_arguments_name);
  }

  frg_assert_int_greater_than_or_equal_to(version_major, 0);
  frg_assert_int_greater_than_or_equal_to(version_minor, 0);
  frg_assert_int_greater_than_or_equal_to(version_patch, 0);

  if (version_label != NULL) {
    frg_assert_string_non_empty(version_label);
  }

  if (version_details != NULL) {
    frg_assert_gstring_non_empty(version_details);
  }

  frg_cli_program_t* program         = frg_malloc(sizeof(frg_cli_program_t));

  program->name                      = name;
  program->binary_name               = binary_name;
  program->positional_arguments_name = positional_arguments_name;
  program->version_major             = version_major;
  program->version_minor             = version_minor;
  program->version_patch             = version_patch;
  program->version_label             = version_label;
  program->version_details           = version_details;
  program->_callback                 = callback;
  program->_commands                 = NULL;
  program->_commands_index_by_name
    = g_hash_table_new((GHashFunc)g_str_hash, (GEqualFunc)g_str_equal);
  program->global_options = frg_cli_option_set_new();

  return program;
}

void frg_cli_program_destroy(frg_cli_program_t* program) {
  frg_assert_pointer_non_null(program);

  for (GList* command = program->_commands; command != NULL; command = command->next) {
    frg_cli_command_destroy(command->data);
  }

  g_list_free(program->_commands);

  g_hash_table_destroy(program->_commands_index_by_name);

  frg_cli_option_set_destroy(program->global_options);

  if (program->version_details != NULL) {
    g_string_free(program->version_details, true);
  }

  frg_free(program);
}

void frg_cli_program_add_command(frg_cli_program_t* program,
                                 frg_cli_command_t* command) {
  frg_assert_pointer_non_null(program);
  frg_assert_pointer_non_null(command);

  program->_commands = g_list_append(program->_commands, command);

  if (g_hash_table_contains(program->_commands_index_by_name, command->name)) {
    frg_die("command '%s' already added", command->name);
  } else {
    g_hash_table_insert(
      program->_commands_index_by_name, (void*)command->name, command);
  }
}

frg_cli_command_t* frg_cli_program_get_command_by_name(frg_cli_program_t* program,
                                                       const char* name) {
  frg_assert_pointer_non_null(program);
  frg_assert_string_non_empty(name);

  return (frg_cli_command_t*)g_hash_table_lookup(program->_commands_index_by_name,
                                                 name);
}

bool frg_cli_program_try_print_help(frg_stream_output_t* mut_stream,
                                    frg_message_buffer_t* mut_message_buffer,
                                    const frg_cli_program_t* program,
                                    const char* command_name) {
  frg_assert_pointer_non_null(program);

  if (command_name != NULL) {
    frg_assert_string_non_empty(command_name);
  }

  frg_cli_program_print_version_long(mut_stream, program);

  frg_stream_output_write_string(mut_stream, "\n");

  if (command_name == NULL) {
    if (program->_commands == NULL) {
      frg_stream_output_write_string(mut_stream, "Usage: ");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
      frg_stream_output_write_printf(
        mut_stream,
        "%s [options]%s%s%s\n",
        program->binary_name,
        program->positional_arguments_name == NULL ? "" : " <",
        program->positional_arguments_name,
        program->positional_arguments_name == NULL ? "" : ">");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
      frg_stream_output_write_string(mut_stream, "\n");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
      frg_stream_output_write_string(mut_stream, "Options:");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
      frg_stream_output_write_string(mut_stream, "\n");
    } else {
      frg_stream_output_write_string(mut_stream, "Usage: ");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
      frg_stream_output_write_printf(
        mut_stream,
        "%s [global options] <command> [command options]\n",
        program->binary_name);
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
      frg_stream_output_write_string(mut_stream, "\n");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
      frg_stream_output_write_string(mut_stream, "Global options:");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
      frg_stream_output_write_string(mut_stream, "\n");
    }

    frg_cli_option_set_print_help(mut_stream, program->global_options);

    if (program->_commands != NULL) {
      frg_stream_output_write_string(mut_stream, "\n");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
      frg_stream_output_write_string(mut_stream, "Commands:");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
      frg_stream_output_write_string(mut_stream, "\n");

      for (GList* command = program->_commands; command != NULL;
           command        = command->next) {
        frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
        frg_stream_output_write_printf(
          mut_stream, "  %s\n", ((frg_cli_command_t*)command->data)->name);
        frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
      }

      frg_stream_output_write_string(mut_stream, "\n");
      frg_stream_output_write_string(mut_stream,
                                     "To see more info about a given command, run:\n");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
      frg_stream_output_write_string(mut_stream, "$ ");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
      frg_stream_output_write_printf(
        mut_stream, "%s help <command>\n", program->binary_name);
    }
  } else {
    frg_cli_command_t* command = (frg_cli_command_t*)g_hash_table_lookup(
      program->_commands_index_by_name, command_name);

    if (command == NULL) {
      frg_message_emit_fc_11_unknown_command(
        mut_message_buffer, command_name, program->binary_name);

      return false;
    }

    frg_stream_output_write_string(mut_stream, "Usage: ");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(
      mut_stream,
      "%s [global options] %s [command options]%s%s%s\n\n",
      program->binary_name,
      command_name,
      command->positional_arguments_name == NULL ? "" : " <",
      command->positional_arguments_name,
      command->positional_arguments_name == NULL ? "" : ">");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);

    frg_cli_command_print_help(mut_stream, command);

    frg_stream_output_write_string(mut_stream, "\n");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
    frg_stream_output_write_string(mut_stream, "Global options:");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_write_string(mut_stream, "\n");

    frg_cli_option_set_print_help(mut_stream, program->global_options);

    if (!frg_cli_option_set_is_empty(command->option_set)) {
      frg_stream_output_write_string(mut_stream, "\n");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
      frg_stream_output_write_string(mut_stream, "Command options:");
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
      frg_stream_output_write_string(mut_stream, "\n");

      frg_cli_option_set_print_help(mut_stream, command->option_set);
    }
  }

  return true;
}

void frg_cli_program_print_version_long(frg_stream_output_t* mut_stream,
                                        const frg_cli_program_t* program) {
  frg_assert_pointer_non_null(program);

  frg_stream_output_write_printf(mut_stream,
                                 "%s %d.%d.%d%s%s\n",
                                 program->name,
                                 program->version_major,
                                 program->version_minor,
                                 program->version_patch,
                                 program->version_label == NULL ? "" : "-",
                                 program->version_label);

  if (program->version_details != NULL) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
    frg_stream_output_write_printf(mut_stream, "%s\n", program->version_details->str);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  }
}

void frg_cli_program_print_version_short(frg_stream_output_t* mut_stream,
                                         const frg_cli_program_t* program) {
  frg_assert_pointer_non_null(program);

  frg_stream_output_write_printf(mut_stream,
                                 "%d.%d.%d%s%s\n",
                                 program->version_major,
                                 program->version_minor,
                                 program->version_patch,
                                 program->version_label == NULL ? "" : "-",
                                 program->version_label);
}

int frg_cli_program_parse(frg_message_buffer_t* mut_message_buffer,
                          void* mut_user_data,
                          const frg_cli_program_t* program,
                          int argc,
                          const char** argv) {
  frg_assert_pointer_non_null(program);
  frg_assert_int_greater_than(argc, 0);
  frg_assert_pointer_non_null(argv);

  GList* positional_arguments = NULL;

  int argi                    = 1;

  while (argi < argc) {
    frg_assert_string_non_empty(argv[argi]);

    if (argv[argi][0] == '-') {
      if (!frg_cli_option_set_parse_next(mut_message_buffer,
                                         &argi,
                                         mut_user_data,
                                         program->global_options,
                                         argc,
                                         argv)) {
        return 1;
      }
    } else {
      if (program->_commands == NULL) {
        positional_arguments = g_list_append(positional_arguments, (void*)argv[argi]);
        argi++;
      } else {
        break;
      }
    }
  }

  if (program->_commands == NULL) {
    frg_assert_pointer_non_null(program->_callback);

    return ((frg_cli_program_callback_t)(program->_callback))(
      mut_message_buffer, mut_user_data, program, positional_arguments);
  } else if (argi >= argc) {
    frg_message_emit_fc_6_expected_command(mut_message_buffer, program->binary_name);

    return 1;
  }
  {
    frg_cli_command_t* command
      = frg_cli_program_get_command_by_name((frg_cli_program_t*)program, argv[argi]);

    if (command == NULL) {
      frg_message_emit_fc_11_unknown_command(
        mut_message_buffer, argv[argi], program->binary_name);

      return 1;
    }

    argi++;

    while (argi < argc) {
      frg_assert_string_non_empty(argv[argi]);
      if (argv[argi][0] == '-') {
        if (!frg_cli_option_set_parse_next(mut_message_buffer,
                                           &argi,
                                           mut_user_data,
                                           command->option_set,
                                           argc,
                                           argv)) {
          return 1;
        }
      } else {
        positional_arguments = g_list_append(positional_arguments, (void*)argv[argi]);
        argi++;
      }
    }

    frg_assert_pointer_non_null(command->callback);

    return ((frg_cli_command_callback_t)(command->callback))(
      mut_message_buffer, mut_user_data, program, positional_arguments);
  }

  return 0;
}
