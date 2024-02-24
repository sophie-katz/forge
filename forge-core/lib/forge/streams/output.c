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
#include <forge/memory.h>
#include <forge/streams/output.h>
#include <unistd.h>

frg_stream_output_t* _frg_global_stream_output_stdout = NULL;

frg_stream_output_t* _frg_global_stream_output_stderr = NULL;

void frg_stream_output_init() {
  frg_assert_with_message(_frg_global_stream_output_stdout == NULL
                            && _frg_global_stream_output_stderr == NULL,
                          "frg_stream_output_init() called twice");

  _frg_global_stream_output_stdout = frg_stream_output_new_file(
    stdout,
    FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR | FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE);

  _frg_global_stream_output_stderr = frg_stream_output_new_file(
    stderr,
    FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR | FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE);
}

void frg_stream_output_cleanup() {
  frg_assert_with_message(_frg_global_stream_output_stdout != NULL
                            || _frg_global_stream_output_stderr != NULL,
                          "frg_stream_output_init() was not called");

  frg_stream_output_destroy(_frg_global_stream_output_stdout);
  _frg_global_stream_output_stdout = NULL;

  frg_stream_output_destroy(_frg_global_stream_output_stderr);
  _frg_global_stream_output_stderr = NULL;
}

void frg_stream_output_set_console_color_enabled(bool enabled) {
  if (enabled) {
    frg_stream_output_get_stdout()->_flags |= FRG_STREAM_OUTPUT_FLAG_COLOR;
    frg_stream_output_get_stderr()->_flags |= FRG_STREAM_OUTPUT_FLAG_COLOR;
  } else {
    frg_stream_output_get_stdout()->_flags &= ~FRG_STREAM_OUTPUT_FLAG_COLOR;
    frg_stream_output_get_stderr()->_flags &= ~FRG_STREAM_OUTPUT_FLAG_COLOR;
  }
}

void frg_stream_output_set_console_unicode_enabled(bool enabled) {
  if (enabled) {
    frg_stream_output_get_stdout()->_flags |= FRG_STREAM_OUTPUT_FLAG_UNICODE;
    frg_stream_output_get_stderr()->_flags |= FRG_STREAM_OUTPUT_FLAG_UNICODE;
  } else {
    frg_stream_output_get_stdout()->_flags &= ~FRG_STREAM_OUTPUT_FLAG_UNICODE;
    frg_stream_output_get_stderr()->_flags &= ~FRG_STREAM_OUTPUT_FLAG_UNICODE;
  }
}

bool _frg_global_stream_output_has_detected_color  = false;
bool _frg_global_stream_output_detect_color_result = false;

bool _frg_stream_output_detect_color(FILE* file) {
  frg_assert(file == stdout || file == stderr);

  if (!_frg_global_stream_output_has_detected_color) {
    _frg_global_stream_output_detect_color_result = isatty(fileno(file));
    _frg_global_stream_output_has_detected_color  = true;
  }

  return _frg_global_stream_output_detect_color_result;
}

bool _frg_global_stream_output_has_detected_unicode  = false;
bool _frg_global_stream_output_detect_unicode_result = false;

bool _frg_stream_output_detect_unicode() {
  // This is stolen from:
  // https://github.com/sindresorhus/is-unicode-supported/blob/main/index.js

  if (!_frg_global_stream_output_has_detected_unicode) {
    const char* value;

#ifdef G_OS_WIN32
    value = g_getenv("WT_SESSION");
    if (value != NULL && *value != 0) {
      _frg_global_stream_output_detect_unicode_result = true;
      _frg_global_stream_output_has_detected_unicode  = true;
      return true;
    }

    value = g_getenv("TERMINUS_SUBLIME");
    if (value != NULL && *value != 0) {
      _frg_global_stream_output_detect_unicode_result = true;
      _frg_global_stream_output_has_detected_unicode  = true;
      return true;
    }

    value = g_getenv("ConEmuTask");
    if (value != NULL && strcmp(value, "{cmd::Cmder}") == 0) {
      _frg_global_stream_output_detect_unicode_result = true;
      _frg_global_stream_output_has_detected_unicode  = true;
      return true;
    }

    value = g_getenv("TERM_PROGRAM");
    if (value != NULL && strcmp(value, "Terminus-Sublime") == 0) {
      _frg_global_stream_output_detect_unicode_result = true;
      _frg_global_stream_output_has_detected_unicode  = true;
      return true;
    } else if (value != NULL && strcmp(value, "vscode") == 0) {
      _frg_global_stream_output_detect_unicode_result = true;
      _frg_global_stream_output_has_detected_unicode  = true;
      return true;
    }

    value = g_getenv("TERM");
    if (value != NULL && strcmp(value, "xterm-256color") == 0) {
      _frg_global_stream_output_detect_unicode_result = true;
      _frg_global_stream_output_has_detected_unicode  = true;
      return true;
    } else if (value != NULL && strcmp(value, "alacritty") == 0) {
      _frg_global_stream_output_detect_unicode_result = true;
      _frg_global_stream_output_has_detected_unicode  = true;
      return true;
    }

    value = g_getenv("TERMINAL_EMULATOR");
    if (value != NULL && strcmp(value, "JetBrains-JediTerm") == 0) {
      _frg_global_stream_output_detect_unicode_result = true;
      _frg_global_stream_output_has_detected_unicode  = true;
      return true;
    }
#else
    value = g_getenv("TERM");
    if (value != NULL && strcmp(value, "linux") == 0) {
      // Linux console (kernel)
      _frg_global_stream_output_detect_unicode_result = false;
      _frg_global_stream_output_has_detected_unicode  = true;
      return false;
    }
#endif
  }

  return _frg_global_stream_output_detect_unicode_result;
}

frg_stream_output_t* frg_stream_output_new_file(FILE* mut_file,
                                                frg_stream_output_flags_t flags) {
  frg_assert_pointer_non_null(mut_file);

  if (mut_file == stdout || mut_file == stderr) {
    frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_OWNED) == 0);

    if ((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR) != 0) {
      if (_frg_stream_output_detect_color(mut_file)) {
        flags |= FRG_STREAM_OUTPUT_FLAG_COLOR;
      } else {
        flags &= ~FRG_STREAM_OUTPUT_FLAG_COLOR;
      }

      flags &= ~FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR;
    }

    if ((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE) != 0) {
      if (_frg_stream_output_detect_unicode()) {
        flags |= FRG_STREAM_OUTPUT_FLAG_UNICODE;
      } else {
        flags &= ~FRG_STREAM_OUTPUT_FLAG_UNICODE;
      }

      flags &= ~FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE;
    }
  } else {
    frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR) == 0);
    frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE) == 0);
  }

  frg_stream_output_t* output = frg_malloc(sizeof(frg_stream_output_t));

  output->_flags              = flags;
  output->_file               = mut_file;
  output->_text               = NULL;

  return output;
}

frg_stream_output_t* frg_stream_output_new_buffer(frg_stream_output_flags_t flags) {
  frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_COLOR) == 0);
  frg_assert((flags & FRG_STREAM_OUTPUT_FLAG_DETECT_UNICODE) == 0);

  frg_stream_output_t* output = frg_malloc(sizeof(frg_stream_output_t));

  output->_flags              = flags;
  output->_file               = NULL;
  output->_text               = g_string_new(NULL);

  return output;
}

void frg_stream_output_destroy(frg_stream_output_t* output) {
  GString* buffer = frg_stream_output_destroy_take_buffer(output);

  if (buffer != NULL) {
    g_string_free(buffer, TRUE);
  }
}

GString* frg_stream_output_destroy_take_buffer(frg_stream_output_t* output) {
  frg_assert_pointer_non_null(output);

  GString* buffer = output->_text;

  if ((output->_flags & FRG_STREAM_OUTPUT_FLAG_OWNED) != 0 && output->_file != NULL) {
    fclose(output->_file);
  }

  frg_free(output);

  return buffer;
}

void frg_stream_output_write_character(frg_stream_output_t* mut_output, char value) {
  frg_assert_pointer_non_null(mut_output);

  if (mut_output->_file != NULL) {
    putc(value, mut_output->_file);
  } else {
    g_string_append_c(mut_output->_text, value);
  }
}

void frg_stream_output_write_string(frg_stream_output_t* mut_output,
                                    const char* value) {
  frg_assert_pointer_non_null(mut_output);
  frg_assert_pointer_non_null(value);

  if (*value == 0) {
    return;
  }

  if (mut_output->_file != NULL) {
    fputs(value, mut_output->_file);
  } else {
    g_string_append(mut_output->_text, value);
  }
}

void frg_stream_output_write_string_with_length(frg_stream_output_t* mut_output,
                                                const char* value,
                                                size_t length) {
  frg_assert_pointer_non_null(mut_output);
  frg_assert_pointer_non_null(value);

  if (length == 0) {
    return;
  }

  if (mut_output->_file != NULL) {
    fwrite(value, sizeof(char), length, mut_output->_file);
  } else {
    g_string_append_len(mut_output->_text, value, length);
  }
}

void frg_stream_output_write_printf(frg_stream_output_t* mut_output,
                                    const char* format,
                                    ...) {
  frg_assert_pointer_non_null(mut_output);
  frg_assert_string_non_empty(format);

  va_list args;
  va_start(args, format);

  frg_stream_output_write_vprintf(mut_output, format, args);

  va_end(args);
}

void frg_stream_output_write_vprintf(frg_stream_output_t* mut_output,
                                     const char* format,
                                     va_list args) {
  frg_assert_pointer_non_null(mut_output);
  frg_assert_string_non_empty(format);

  if (mut_output->_file != NULL) {
    vfprintf(mut_output->_file, format, args);
  } else {
    g_string_append_vprintf(mut_output->_text, format, args);
  }
}

void frg_stream_output_set_color(frg_stream_output_t* mut_output,
                                 frg_stream_output_color_t color) {
  frg_assert_pointer_non_null(mut_output);

  if ((mut_output->_flags & FRG_STREAM_OUTPUT_FLAG_COLOR) == 0) {
    return;
  }

  switch (color) {
  case FRG_STREAM_OUTPUT_COLOR_RESET:
    frg_stream_output_write_string(mut_output, "\033[0;0m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BOLD:
    frg_stream_output_write_string(mut_output, "\033[1m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_DIM:
    frg_stream_output_write_string(mut_output, "\033[2m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_ITALIC:
    frg_stream_output_write_string(mut_output, "\033[3m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_UNDERLINE:
    frg_stream_output_write_string(mut_output, "\033[4m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BLACK:
    frg_stream_output_write_string(mut_output, "\033[30m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_RED:
    frg_stream_output_write_string(mut_output, "\033[31m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_GREEN:
    frg_stream_output_write_string(mut_output, "\033[32m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_YELLOW:
    frg_stream_output_write_string(mut_output, "\033[33m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BLUE:
    frg_stream_output_write_string(mut_output, "\033[34m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_MAGENTA:
    frg_stream_output_write_string(mut_output, "\033[35m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_CYAN:
    frg_stream_output_write_string(mut_output, "\033[36m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_WHITE:
    frg_stream_output_write_string(mut_output, "\033[37m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK:
    frg_stream_output_write_string(mut_output, "\033[90m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED:
    frg_stream_output_write_string(mut_output, "\033[91m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BRIGHT_GREEN:
    frg_stream_output_write_string(mut_output, "\033[92m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW:
    frg_stream_output_write_string(mut_output, "\033[93m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLUE:
    frg_stream_output_write_string(mut_output, "\033[94m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BRIGHT_MAGENTA:
    frg_stream_output_write_string(mut_output, "\033[95m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BRIGHT_CYAN:
    frg_stream_output_write_string(mut_output, "\033[96m");
    break;
  case FRG_STREAM_OUTPUT_COLOR_BRIGHT_WHITE:
    frg_stream_output_write_string(mut_output, "\033[97m");
    break;
  default:
    frg_die_unexpected_enum_value(color);
  }
}

const char* frg_stream_output_choose_ascii_or_unicode(
  const frg_stream_output_t* mut_output, const char* ascii, const char* unicode) {
  frg_assert_pointer_non_null(mut_output);
  frg_assert_pointer_non_null(ascii);
  frg_assert_pointer_non_null(unicode);

  if ((mut_output->_flags & FRG_STREAM_OUTPUT_FLAG_UNICODE) != 0) {
    return unicode;
  } else {
    return ascii;
  }
}

frg_stream_output_t* frg_stream_output_get_stdout() {
  frg_assert_with_message(_frg_global_stream_output_stdout != NULL,
                          "frg_stream_output_init() was not called");

  return _frg_global_stream_output_stdout;
}

frg_stream_output_t* frg_stream_output_get_stderr() {
  frg_assert_with_message(_frg_global_stream_output_stderr != NULL,
                          "frg_stream_output_init() was not called");

  return _frg_global_stream_output_stderr;
}
