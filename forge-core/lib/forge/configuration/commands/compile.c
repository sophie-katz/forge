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
#include <forge/ast/node.h>
#include <forge/ast/print_debug.h>
#include <forge/codegen/codegen.h>
#include <forge/configuration/commands/compile.h>
#include <forge/configuration/commands/utilities.h>
#include <forge/configuration/options.h>
#include <forge/messages/codes.h>
#include <forge/parsing/parse.h>

int _frg_configuration_commands_callback_compile(
  frg_message_buffer_t* mut_message_buffer,
  void* mut_user_data,
  const frg_cli_program_t* program,
  const GList* positional_arguments) {
  // Get options from user data
  const frg_configuration_options_t* options
    = (const frg_configuration_options_t*)mut_user_data;

  // Validate options for compilation
  if (options->compilation.write_object_file
      && options->compilation.object_file_path == NULL) {
    frg_message_emit_fc_15_missing_required_argument(
      mut_message_buffer, "object-file-path", "o");

    return 1;
  }

  // Get source file path
  const char* path = frg_configuration_get_single_source_file_from_positional_arguments(
    mut_message_buffer, positional_arguments);

  if (path == NULL) {
    return 1;
  }

  // Open source file
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    frg_message_emit_ffs_2_open_for_reading(mut_message_buffer, path, strerror(errno));

    return 1;
  }

  frg_stream_input_t* stream
    = frg_stream_input_new_file(file, FRG_STREAM_INPUT_FLAG_OWNED);

  frg_parsing_source_t* source = frg_parsing_source_new(path, stream);

  // Parse source file
  frg_ast_node_t* node         = frg_parse(mut_message_buffer, source);

  // If there is an error, exit compilation early
  if (node == NULL) {
    frg_parsing_source_destroy(source);

    return 1;
  }

  // Print AST if enabled
  if (options->compilation.print_ast) {
    frg_stream_output_set_color(frg_stream_output_get_stdout(),
                                FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(
      frg_stream_output_get_stdout(),
      "Abstract syntax tree (AST):\n%s\n\n",
      frg_stream_output_choose_ascii_or_unicode(frg_stream_output_get_stdout(),
                                                "===========================",
                                                "━━━━━━━━━━━━━━━━━━━━━━━━━━━"));
    frg_stream_output_set_color(frg_stream_output_get_stdout(),
                                FRG_STREAM_OUTPUT_COLOR_RESET);

    frg_ast_print_debug(
      frg_stream_output_get_stdout(), node, &frg_ast_print_debug_options_default);

    frg_stream_output_write_character(frg_stream_output_get_stdout(), '\n');
  }

  if (options->compilation.codegen) {
    // If code generation is enabled, generate IR
    frg_codegen_module_t* codegen_module = frg_codegen(node);

    // Clean up memory early
    frg_ast_node_destroy(node);

    frg_parsing_source_destroy(source);

    // If there was an error during code generation, exit compilation early
    if (codegen_module == NULL) {
      return 1;
    }

    // Print IR if enabled
    if (options->compilation.print_ir) {
      if (options->compilation.print_ir) {
        frg_stream_output_write_character(frg_stream_output_get_stdout(), '\n');
      }

      frg_stream_output_set_color(frg_stream_output_get_stdout(),
                                  FRG_STREAM_OUTPUT_COLOR_BOLD);
      frg_stream_output_write_printf(
        frg_stream_output_get_stdout(),
        "Intermediate representation (IR):\n%s\n\n",
        frg_stream_output_choose_ascii_or_unicode(frg_stream_output_get_stdout(),
                                                  "=================================",
                                                  "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"));
      frg_stream_output_set_color(frg_stream_output_get_stdout(),
                                  FRG_STREAM_OUTPUT_COLOR_RESET);

      frg_codegen_module_print(frg_stream_output_get_stdout(), codegen_module);
    }

    bool write_object_file_result = true;
    if (options->compilation.write_object_file) {
      // If writing the object file is enabled, write it
      write_object_file_result = frg_codegen_module_write_object_file(
        mut_message_buffer, codegen_module, options->compilation.object_file_path);
    }

    // Clean up memory
    frg_codegen_module_destroy(codegen_module);

    // Return result
    if (!write_object_file_result) {
      return 1;
    }
  } else {
    // If code generation is disabled, exit compilation early
    frg_ast_node_destroy(node);

    frg_parsing_source_destroy(source);
  }

  return 0;
}

bool _frg_configuration_commands_option_callback_object_file_path(
  frg_message_buffer_t* mut_message_buffer, void* mut_user_data, const char* value) {
  frg_configuration_options_t* options  = (frg_configuration_options_t*)mut_user_data;

  options->compilation.object_file_path = value;

  return true;
}

bool _frg_configuration_commands_option_callback_print_ast(
  frg_message_buffer_t* message_buffer, void* mut_user_data, const char* value) {
  frg_configuration_options_t* options = (frg_configuration_options_t*)mut_user_data;

  options->compilation.print_ast       = true;

  return true;
}

bool _frg_configuration_commands_option_callback_only_parse(
  frg_message_buffer_t* message_buffer, void* mut_user_data, const char* value) {
  frg_configuration_options_t* options   = (frg_configuration_options_t*)mut_user_data;

  options->compilation.codegen           = false;
  options->compilation.print_ir          = false;
  options->compilation.write_object_file = false;
  options->compilation.object_file_path  = NULL;

  return true;
}

bool _frg_configuration_commands_option_callback_print_ir(
  frg_message_buffer_t* message_buffer, void* mut_user_data, const char* value) {
  frg_configuration_options_t* options = (frg_configuration_options_t*)mut_user_data;

  options->compilation.print_ir        = true;

  return true;
}

bool _frg_configuration_commands_option_callback_dry(
  frg_message_buffer_t* message_buffer, void* mut_user_data, const char* value) {
  frg_configuration_options_t* options   = (frg_configuration_options_t*)mut_user_data;

  options->compilation.write_object_file = false;
  options->compilation.object_file_path  = NULL;

  return true;
}

frg_cli_command_t* frg_configuration_new_command_compile() {
  frg_cli_command_t* command
    = frg_cli_command_new("compile",
                          "source file",
                          "Compiles one source file into an object file.",
                          _frg_configuration_commands_callback_compile);

  frg_cli_option_t* option = frg_cli_option_new_argument_short(
    'o',
    "output-path",
    "path",
    "The path to output the *.o file to.",
    _frg_configuration_commands_option_callback_object_file_path);

  frg_cli_option_set_add_option(command->option_set, option);

  option = frg_cli_option_new_flag(
    "print-ast",
    "Prints out abstract syntax tree (AST) during compilation.",
    _frg_configuration_commands_option_callback_print_ast);

  frg_cli_option_set_add_option(command->option_set, option);

  option = frg_cli_option_new_flag(
    "only-parse",
    "Only parses the source file, but does not generate machine code.",
    _frg_configuration_commands_option_callback_only_parse);

  frg_cli_option_set_add_option(command->option_set, option);

  option = frg_cli_option_new_flag(
    "print-ir",
    "Prints out intermediate representation (IR) during compilation.",
    _frg_configuration_commands_option_callback_print_ir);

  frg_cli_option_set_add_option(command->option_set, option);

  option
    = frg_cli_option_new_flag_short('n',
                                    "dry",
                                    "Dry run only (parses and generates machine "
                                    "code, but does not write object file).",
                                    _frg_configuration_commands_option_callback_dry);

  frg_cli_option_set_add_option(command->option_set, option);

  return command;
}
