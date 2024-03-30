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
#include <forge/math.h>
#include <forge/memory.h>
#include <forge/messages/message.h>

frg_message_t* frg_message_new(const char* log_path,
                               frg_line_number_t log_line_number,
                               const frg_parsing_range_t* source_range,
                               frg_message_severity_t severity,
                               const char* code,
                               GString* text) {
  frg_assert_string_non_empty(log_path);
  frg_assert_int_greater_than_or_equal_to(log_line_number, 1);

  if (code != NULL) {
    frg_assert_string_non_empty(code);
  }

  frg_assert_gstring_non_empty(text);
  frg_assert(text->str[0] >= 'A' && text->str[0] <= 'Z');
  frg_assert_int_not_equal_to(text->str[text->len - 1], '.');
  frg_assert_int_not_equal_to(text->str[text->len - 1], ',');
  frg_assert_int_not_equal_to(text->str[text->len - 1], '\n');
  frg_assert_int_not_equal_to(text->str[text->len - 1], ':');
  frg_assert_int_not_equal_to(text->str[text->len - 1], ' ');

  frg_message_t* message   = frg_malloc(sizeof(frg_message_t));

  message->log_path        = log_path;
  message->log_line_number = log_line_number;

  if (source_range == NULL) {
    message->source_range.length              = 0;
    message->source_range.start.path          = NULL;
    message->source_range.start.offset        = 0;
    message->source_range.start.line_number   = 0;
    message->source_range.start.column_number = 0;
  } else {
    if (source_range->start.path == NULL) {
      frg_assert_int_equal_to(source_range->start.line_number, 0);
      frg_assert_int_equal_to(source_range->start.column_number, 0);
    } else {
      frg_assert_string_non_empty(source_range->start.path);

      if (source_range->start.line_number == 0) {
        frg_assert_int_equal_to(source_range->start.column_number, 0);
      }
    }

    message->source_range = *source_range;
  }

  message->severity = severity;
  message->code     = code;
  message->text     = text;
  message->children = NULL;

  return message;
}

void frg_message_destroy(frg_message_t* message) {
  frg_assert_pointer_non_null(message);

  g_string_free(message->text, TRUE);

  for (GList* iter = message->children; iter != NULL; iter = iter->next) {
    frg_message_t* child = (frg_message_t*)iter->data;
    frg_message_destroy(child);
  }

  g_list_free(message->children);

  frg_free(message);
}

void frg_message_add_child(frg_message_t* message, frg_message_t* child) {
  frg_assert_pointer_non_null(message);
  frg_assert_pointer_non_null(child);

  if (child->severity != FRG_MESSAGE_SEVERITY_NOTE) {
    frg_die("currently child messages can only have severity 'note'");
  }

  message->children = g_list_append(message->children, child);
}

bool _frg_message_should_print_prefix_internal(frg_message_severity_t severity) {
#ifdef FRG_ALWAYS_PRINT_INTERNAL_MESSAGE_PREFIX
  return true;
#else
  return severity == FRG_MESSAGE_SEVERITY_DEBUG
      || severity == FRG_MESSAGE_SEVERITY_INTERNAL_ERROR;
#endif
}

void _frg_message_print_prefix_internal(frg_stream_output_t* mut_stream,
                                        const char* log_path,
                                        frg_line_number_t log_line_number,
                                        frg_message_severity_t severity) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_string_non_empty(log_path);
  frg_assert_int_greater_than_or_equal_to(log_line_number, 1);

  if (_frg_message_should_print_prefix_internal(severity)) {
    const char* log_path_resolved = strstr(log_path, "lib/");

    if (log_path_resolved == NULL) {
      log_path_resolved = strstr(log_path, "src/");
    }

    if (log_path_resolved == NULL) {
      log_path_resolved = log_path;
    }

    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
    frg_stream_output_write_printf(mut_stream,
                                   "[internally from forge-core/%s:%d]\n",
                                   log_path_resolved,
                                   log_line_number);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  }
}

void _frg_message_print_prefix_source_location(
  frg_stream_output_t* mut_stream, const frg_parsing_location_t* source_location) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(source_location);

  if (source_location->path != NULL) {
    frg_assert_string_non_empty(source_location->path);

    if (source_location->line_number == 0) {
      frg_assert_int_equal_to(source_location->column_number, 0);
    }

    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_CYAN);
    frg_stream_output_write_printf(mut_stream, "%s:", source_location->path);

    if (source_location->line_number != 0) {
      frg_stream_output_write_printf(mut_stream, "%i:", source_location->line_number);

      if (source_location->column_number != 0) {
        frg_stream_output_write_printf(
          mut_stream, "%i:", source_location->column_number);
      }
    } else {
      frg_assert_int_equal_to(source_location->column_number, 0);
    }

    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_write_character(mut_stream, ' ');
  } else {
    frg_assert_int_equal_to(source_location->line_number, 0);
    frg_assert_int_equal_to(source_location->column_number, 0);
  }
}

void _frg_message_print_prefix_severity_and_code(frg_stream_output_t* mut_stream,
                                                 frg_message_severity_t severity,
                                                 const char* code) {
  frg_assert_pointer_non_null(mut_stream);

  switch (severity) {
  case FRG_MESSAGE_SEVERITY_DEBUG:
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_MAGENTA);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(mut_stream, "debug%s ", code == NULL ? ":" : "");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    break;
  case FRG_MESSAGE_SEVERITY_NOTE:
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(mut_stream, "note%s ", code == NULL ? ":" : "");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    break;
  case FRG_MESSAGE_SEVERITY_WARNING:
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(mut_stream, "warning%s ", code == NULL ? ":" : "");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    break;
  case FRG_MESSAGE_SEVERITY_ERROR:
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RED);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(mut_stream, "error%s ", code == NULL ? ":" : "");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    break;
  case FRG_MESSAGE_SEVERITY_FATAL_ERROR:
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_UNDERLINE);
    frg_stream_output_write_printf(mut_stream, "fatal error");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(mut_stream, "%s ", code == NULL ? ":" : "");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    break;
  case FRG_MESSAGE_SEVERITY_INTERNAL_ERROR:
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_CYAN);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_write_printf(
      mut_stream, "internal error%s ", code == NULL ? ":" : "");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    break;
  default:
    frg_die_unexpected_enum_value(severity);
  }

  if (code != NULL) {
    frg_assert_string_non_empty(code);

    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);
    frg_stream_output_write_printf(mut_stream, "[%s] ", code);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  }
}

void _frg_message_print_text(frg_stream_output_t* mut_stream, const char* text) {
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_WHITE);
  frg_stream_output_write_string(mut_stream, text);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
}

void _frg_message_print_suffix(frg_stream_output_t* mut_stream) {
  frg_assert_pointer_non_null(mut_stream);

  frg_stream_output_write_printf(mut_stream, "\n");
}

bool _frg_message_should_print_sample(const frg_message_t* message) {
  frg_assert_pointer_non_null(message);

  return message->source_range.start.path != NULL
      && (message->source_range.start.line_number != 0
          && message->source_range.start.column_number != 0);
}

const char* _frg_message_get_line_number_separator(frg_stream_output_t* mut_stream) {
  frg_assert_pointer_non_null(mut_stream);

  return frg_stream_output_choose_ascii_or_unicode(mut_stream, "|", "▏");
}

void _frg_message_print_sample_prefix_line_number(
  frg_stream_output_t* mut_stream,
  frg_line_number_t line_number,
  frg_column_number_t line_number_pad_to_width) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_int_greater_than_or_equal_to(line_number, 1);
  frg_assert_int_greater_than_or_equal_to(line_number_pad_to_width, 1);

  uint32_t line_number_digits = frg_get_uint32_digit_count(line_number);

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);

  frg_stream_output_write_character(mut_stream, ' ');

  for (uint32_t i = line_number_digits; i < line_number_pad_to_width; i++) {
    frg_stream_output_write_character(mut_stream, ' ');
  }

  frg_stream_output_write_printf(mut_stream,
                                 "%i %s ",
                                 line_number,
                                 _frg_message_get_line_number_separator(mut_stream));

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
}

void _frg_message_print_sample_prefix_blank(
  frg_stream_output_t* mut_stream, frg_column_number_t line_number_pad_to_width) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_int_greater_than_or_equal_to(line_number_pad_to_width, 1);

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_BLACK);

  for (uint32_t i = 0; i < line_number_pad_to_width; i++) {
    frg_stream_output_write_character(mut_stream, ' ');
  }

  frg_stream_output_write_printf(
    mut_stream, "  %s ", _frg_message_get_line_number_separator(mut_stream));

  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
}

typedef struct {
  ssize_t characters_before;
  ssize_t characters_within;
} _frg_message_print_sample_line_result_t;

_frg_message_print_sample_line_result_t _frg_message_print_sample_line(
  frg_stream_output_t* mut_stream,
  gchar** text_iterator,
  ssize_t* text_offset,
  ssize_t sample_start,
  ssize_t sample_length,
  frg_stream_output_color_t color) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(text_iterator);
  frg_assert_pointer_non_null(*text_iterator);
  frg_assert_pointer_non_null(text_offset);
  frg_assert_int_non_negative(*text_offset);
  frg_assert_int_non_negative(sample_start);
  frg_assert_int_positive(sample_length);

  _frg_message_print_sample_line_result_t result
    = { .characters_before = 0, .characters_within = 0 };

  while (**text_iterator != 0 && (**text_iterator != '\n' && **text_iterator != '\r')) {
    if (*text_offset >= sample_start && *text_offset < sample_start + sample_length) {
      frg_stream_output_set_color(mut_stream, color);
      frg_stream_output_write_character(mut_stream, **text_iterator);
      frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);

      result.characters_within++;
    } else {
      frg_stream_output_write_character(mut_stream, **text_iterator);

      if (result.characters_within == 0) {
        result.characters_before++;
      }
    }

    (*text_iterator)++;
    (*text_offset)++;
  }

  if (**text_iterator == '\n' || **text_iterator == '\r') {
    (*text_iterator)++;
    (*text_offset)++;
  }

  frg_stream_output_write_character(mut_stream, '\n');

  return result;
}

void _frg_message_print_sample_underline(
  frg_stream_output_t* mut_stream,
  const _frg_message_print_sample_line_result_t* line_result,
  frg_stream_output_color_t color) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(line_result);
  frg_assert_int_non_negative(line_result->characters_before);
  frg_assert_int_positive(line_result->characters_within);

  for (ssize_t i = 0; i < line_result->characters_before; i++) {
    frg_stream_output_write_character(mut_stream, ' ');
  }

  frg_stream_output_set_color(mut_stream, color);
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
  for (ssize_t i = 0; i < line_result->characters_within; i++) {
    frg_stream_output_write_string(
      mut_stream, frg_stream_output_choose_ascii_or_unicode(mut_stream, "^", "▔"));
  }
  frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);

  frg_stream_output_write_character(mut_stream, '\n');
}

void _frg_message_print_sample(frg_stream_output_t* mut_stream,
                               frg_parsing_source_context_t* mut_source_context,
                               const frg_message_t* message,
                               frg_column_number_t line_number_pad_to_width) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(message);
  frg_assert_int_greater_than_or_equal_to(line_number_pad_to_width, 1);
  frg_assert_pointer_non_null(mut_source_context);

  if (_frg_message_should_print_sample(message)) {
    frg_parsing_source_t* source = frg_parsing_source_context_get_source_by_path(
      mut_source_context, message->source_range.start.path);

    frg_assert_pointer_non_null(source);

    GString* loaded = frg_parsing_source_load_range(source, &message->source_range);

    frg_assert_gstring_non_empty(loaded);

    gchar* text_iterator = loaded->str;
    ssize_t text_offset  = message->source_range.start.offset
                        - (message->source_range.start.column_number - 1);
    frg_line_number_t current_line_number = message->source_range.start.line_number;

    while (*text_iterator != 0) {
      _frg_message_print_sample_prefix_line_number(
        mut_stream, current_line_number, line_number_pad_to_width);

      _frg_message_print_sample_line_result_t line_result
        = _frg_message_print_sample_line(mut_stream,
                                         &text_iterator,
                                         &text_offset,
                                         message->source_range.start.offset,
                                         message->source_range.length,
                                         message->severity
                                             == FRG_MESSAGE_SEVERITY_WARNING
                                           ? FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW
                                           : FRG_STREAM_OUTPUT_COLOR_BRIGHT_RED);

      _frg_message_print_sample_prefix_blank(mut_stream, line_number_pad_to_width);

      _frg_message_print_sample_underline(mut_stream,
                                          &line_result,
                                          message->severity
                                              == FRG_MESSAGE_SEVERITY_WARNING
                                            ? FRG_STREAM_OUTPUT_COLOR_YELLOW
                                            : FRG_STREAM_OUTPUT_COLOR_RED);

      current_line_number++;
    }

    g_string_free(loaded, TRUE);
  } else {
    frg_stream_output_write_printf(mut_stream, "\n");
  }
}

void frg_message_print(frg_stream_output_t* mut_stream,
                       frg_parsing_source_context_t* mut_source_context,
                       const frg_message_t* message,
                       frg_column_number_t line_number_pad_to_width) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(message);
  frg_assert_int_greater_than_or_equal_to(line_number_pad_to_width, 1);

  _frg_message_print_prefix_internal(
    mut_stream, message->log_path, message->log_line_number, message->severity);

  _frg_message_print_prefix_source_location(mut_stream, &message->source_range.start);

  _frg_message_print_prefix_severity_and_code(
    mut_stream, message->severity, message->code);

  _frg_message_print_text(mut_stream, message->text->str);

  _frg_message_print_suffix(mut_stream);

  // TODO: Add a warning here
  if (mut_source_context != NULL) {
    _frg_message_print_sample(
      mut_stream, mut_source_context, message, line_number_pad_to_width);
  }

  for (GList* iter = message->children; iter != NULL; iter = iter->next) {
    frg_message_t* child = (frg_message_t*)iter->data;
    frg_message_print(mut_stream, mut_source_context, child, line_number_pad_to_width);
  }
}

bool frg_message_matches_query(const frg_message_t* message,
                               const frg_message_query_t* query) {
  frg_assert_pointer_non_null(message);
  frg_assert_pointer_non_null(query);
  frg_assert_gstring_non_empty(message->text);

  if (message->severity != query->with_severity) {
    return false;
  }

  if (query->with_code != NULL) {
    if (message->code == NULL) {
      return false;
    } else {
      if (strcmp(message->code, query->with_code) != 0) {
        return false;
      }
    }
  }

  if (query->with_text != NULL && strcmp(message->text->str, query->with_text) != 0) {
    return false;
  }

  if (query->on_line > 0 && message->source_range.start.line_number != query->on_line) {
    return false;
  }

  if (query->from_path != NULL) {
    if (message->source_range.start.path == NULL) {
      return false;
    } else {
      if (strcmp(message->source_range.start.path, query->from_path) != 0) {
        return false;
      }
    }
  }

  return true;
}
