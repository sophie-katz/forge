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

#include <build_configuration.h>
#include <forge/assert.h>
#include <forge/configuration/cli_banner.h>
#include <forge/configuration/cli_program.h>
#include <forge/configuration/commands/compile.h>
#include <forge/configuration/commands/dump_ast.h>
#include <forge/configuration/commands/dump_ir.h>
#include <forge/configuration/commands/help.h>
#include <forge/configuration/commands/link.h>
#include <forge/configuration/commands/version.h>
#include <forge/configuration/options.h>

int _frg_config_cli_program_callback(frg_message_buffer_t* mut_message_buffer,
                                     void* mut_user_data,
                                     const frg_cli_program_t* program,
                                     const GList* positional_arguments) {
  frg_configuration_print_cli_banner();

  if (!frg_cli_program_try_print_help(
        frg_stream_output_get_stdout(), mut_message_buffer, program, NULL)) {
    return 1;
  }

  return 0;
}

bool _frg_config_cli_option_callback_debug(frg_message_buffer_t* mut_message_buffer,
                                           void* mut_user_data,
                                           const char* value) {
  frg_configuration_options_t* options = (frg_configuration_options_t*)mut_user_data;

  options->minimum_message_severity    = FRG_MESSAGE_SEVERITY_DEBUG;

  return true;
}

bool _frg_config_cli_option_callback_color_mode(
  frg_message_buffer_t* mut_message_buffer, void* mut_user_data, const char* value) {
  if (strcmp(value, "disabled") == 0) {
    frg_stream_output_set_console_color_enabled(false);
  } else if (strcmp(value, "auto") == 0) {
    // Do nothing
  } else if (strcmp(value, "enabled") == 0) {
    frg_stream_output_set_console_color_enabled(true);
  } else {
    frg_die("unknown color mode");

    return false;
  }

  return true;
}

bool _frg_config_cli_option_callback_unicode_mode(
  frg_message_buffer_t* mut_message_buffer, void* mut_user_data, const char* value) {
  if (strcmp(value, "disabled") == 0) {
    frg_stream_output_set_console_unicode_enabled(false);
  } else if (strcmp(value, "auto") == 0) {
    // Do nothing
  } else if (strcmp(value, "enabled") == 0) {
    frg_stream_output_set_console_unicode_enabled(true);
  } else {
    frg_die("unknown color mode");

    return false;
  }

  return true;
}

frg_cli_program_t* frg_config_cli_program_new() {
  GString* version_details = g_string_new(NULL);

  g_string_append_printf(version_details, "Git commit: %s\n", FRG_GIT_COMMIT_SHA);
  g_string_append_printf(
    version_details, "Compiled with: %s %s\n", FRG_COMPILER_ID, FRG_COMPILER_VERSION);
  g_string_append_printf(version_details, "Linked with: %s\n", FRG_LINKER_ID);
  g_string_append_printf(version_details,
                         "Built for: %s (%s)\n",
                         FRG_HOST_MACHINE_SYSTEM,
                         FRG_HOST_MACHINE_CPU);
  g_string_append_printf(version_details,
                         "Using LLVM %i.%i.%i",
                         FRG_LLVM_VERSION_MAJOR,
                         FRG_LLVM_VERSION_MINOR,
                         FRG_LLVM_VERSION_PATCH);

  frg_cli_program_t* program = frg_cli_program_new("Forge",
                                                   "forge",
                                                   NULL,
                                                   FRG_VERSION_MAJOR,
                                                   FRG_VERSION_MINOR,
                                                   FRG_VERSION_PATCH,
                                                   FRG_VERSION_LABEL,
                                                   version_details,
                                                   _frg_config_cli_program_callback);

  frg_cli_option_t* option
    = frg_cli_option_new_choice("color-mode",
                                "mode",
                                "Set the color mode",
                                _frg_config_cli_option_callback_color_mode);

  frg_cli_option_add_choice(option,
                            frg_cli_choice_new("disabled", "Disable color output"));

  frg_cli_option_add_choice(
    option,
    frg_cli_choice_new("auto",
                       "Automatically detect whether or not to use color output"));

  frg_cli_option_add_choice(
    option, frg_cli_choice_new("enabled", "Force color output to be enabled"));

  frg_cli_option_set_add_option(program->global_options, option);

  frg_cli_option_set_add_option(
    program->global_options,
    frg_cli_option_new_flag(
      "debug", "Enable debug logging", _frg_config_cli_option_callback_debug));

  option = frg_cli_option_new_choice("unicode-mode",
                                     "mode",
                                     "Set the unicode mode",
                                     _frg_config_cli_option_callback_unicode_mode);

  frg_cli_option_add_choice(
    option,
    frg_cli_choice_new("disabled", "Disable unicode characters and use ASCII instead"));

  frg_cli_option_add_choice(
    option,
    frg_cli_choice_new(
      "auto", "Automatically detect whether or not to use unicode characters"));

  frg_cli_option_add_choice(
    option,
    frg_cli_choice_new("enabled",
                       "Force unicode characters to be used when available"));

  frg_cli_option_set_add_option(program->global_options, option);

  frg_cli_program_add_command(program, frg_configuration_new_command_compile());

  frg_cli_program_add_command(program, frg_configuration_new_command_dump_ast());

  frg_cli_program_add_command(program, frg_configuration_new_command_dump_ir());

  frg_cli_program_add_command(program, frg_configuration_new_command_help());

  frg_cli_program_add_command(program, frg_configuration_new_command_link());

  frg_cli_program_add_command(program, frg_configuration_new_command_version());

  return program;
}
