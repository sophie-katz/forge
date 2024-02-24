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
#include <forge/cli/program.h>
#include <forge/configuration/cli_program.h>
#include <forge/configuration/options.h>
#include <forge/enum_printing.h>
#include <forge/memory.h>
#include <forge/messages/codes.h>

frg_configuration_options_t* frg_configuration_options_new_default() {
  frg_configuration_options_t* options
    = frg_malloc(sizeof(frg_configuration_options_t));

  options->version_short            = false;
  options->compile_output_path      = NULL;
  options->minimum_message_severity = FRG_MESSAGE_SEVERITY_NOTE;

  return options;
}

void frg_configuration_options_destroy(frg_configuration_options_t* options) {
  frg_free(options);
}

int frg_configuration_options_parse_cli(frg_message_buffer_t* mut_message_buffer,
                                        frg_configuration_options_t* mut_options,
                                        int argc,
                                        const char** argv) {
  frg_assert_pointer_non_null(mut_options);
  frg_assert_int_greater_than(argc, 0);
  frg_assert_pointer_non_null(argv);

  frg_cli_program_t* program = frg_configuration_new_cli_program();

  int result
    = frg_cli_program_parse(mut_message_buffer, mut_options, program, argc, argv);

  frg_cli_program_destroy(program);

  return result;
}

bool _frg_configuration_options_parse_environment_bool(
  bool* out_result,
  frg_message_buffer_t* mut_message_buffer,
  const char* key,
  const char* text) {
  frg_assert_pointer_non_null(out_result);
  frg_assert_string_non_empty(key);
  frg_assert_pointer_non_null(text);

  if (strcmp(text, "true") == 0) {
    *out_result = true;
    return true;
  } else if (strcmp(text, "false") == 0) {
    *out_result = false;
    return true;
  } else {
    frg_message_emit_fc_12_invalid_boolean_env_variable(mut_message_buffer, key, text);

    return false;
  }
}

bool _frg_configuration_options_parse_environment_debug(
  frg_message_buffer_t* mut_message_buffer, frg_configuration_options_t* mut_options) {
  frg_assert_pointer_non_null(mut_options);

  const char* debug_text = g_getenv("FORGE_DEBUG");
  if (debug_text != NULL && *debug_text != 0) {
    bool debug_value = false;
    if (!_frg_configuration_options_parse_environment_bool(
          &debug_value, mut_message_buffer, "FORGE_DEBUG", debug_text)) {
      return false;
    }

    if (debug_value) {
      mut_options->minimum_message_severity = FRG_MESSAGE_SEVERITY_DEBUG;
    }
  }

  return true;
}

bool _frg_configuration_options_parse_environment_color_mode(
  frg_message_buffer_t* mut_message_buffer, frg_configuration_options_t* mut_options) {
  const char* color_mode_text = g_getenv("FORGE_COLOR_MODE");
  if (color_mode_text != NULL && *color_mode_text != 0) {
    if (strcmp(color_mode_text, "disabled") == 0) {
      frg_stream_output_set_console_color_enabled(false);
    } else if (strcmp(color_mode_text, "auto") == 0) {
      // Do nothing
    } else if (strcmp(color_mode_text, "enabled") == 0) {
      frg_stream_output_set_console_color_enabled(true);
    } else {
      frg_message_emit_fc_13_unknown_color_mode(mut_message_buffer, color_mode_text);

      return false;
    }
  }

  return true;
}

bool _frg_configuration_options_parse_environment_unicode_mode(
  frg_message_buffer_t* mut_message_buffer, frg_configuration_options_t* mut_options) {
  const char* unicode_mode_text = g_getenv("FORGE_UNICODE_MODE");
  if (unicode_mode_text != NULL && *unicode_mode_text != 0) {
    if (strcmp(unicode_mode_text, "disabled") == 0) {
      frg_stream_output_set_console_unicode_enabled(false);
    } else if (strcmp(unicode_mode_text, "auto") == 0) {
      // Do nothing
    } else if (strcmp(unicode_mode_text, "enabled") == 0) {
      frg_stream_output_set_console_unicode_enabled(true);
    } else {
      frg_message_emit_fc_14_unknown_unicode_mode(mut_message_buffer,
                                                  unicode_mode_text);

      return false;
    }
  }

  return true;
}

bool frg_configuration_options_parse_environment(
  frg_message_buffer_t* mut_message_buffer, frg_configuration_options_t* mut_options) {
  frg_assert_pointer_non_null(mut_options);

  if (!_frg_configuration_options_parse_environment_debug(mut_message_buffer,
                                                          mut_options)) {
    return false;
  }

  if (!_frg_configuration_options_parse_environment_color_mode(mut_message_buffer,
                                                               mut_options)) {
    return false;
  }

  if (!_frg_configuration_options_parse_environment_unicode_mode(mut_message_buffer,
                                                                 mut_options)) {
    return false;
  }

  return true;
}

void frg_configuration_options_emit_message_debug(
  frg_message_buffer_t* mut_message_buffer,
  const frg_configuration_options_t* options) {
  frg_assert_pointer_non_null(options);

  frg_stream_output_t* stream
    = frg_stream_output_new_buffer(FRG_STREAM_OUTPUT_FLAG_NONE);

  frg_stream_output_write_string(stream, "Configuration:");

  frg_stream_output_write_printf(stream,
                                 "\n  options.version_short == %s\n",
                                 options->version_short ? "true" : "false");

  frg_stream_output_write_printf(stream,
                                 "\n  options.compile_output_path == \"%s\"\n",
                                 options->compile_output_path);

  frg_stream_output_write_string(stream, "\n  options.minimum_message_severity == \"");

  frg_message_severity_print(stream, options->minimum_message_severity);

  frg_stream_output_write_string(stream, "\"\n");

  GString* buffer = frg_stream_output_destroy_take_buffer(stream);

  frg_message_emit(mut_message_buffer, FRG_MESSAGE_SEVERITY_DEBUG, NULL, buffer->str);

  g_string_free(buffer, TRUE);
}
