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
#include <forge/math.h>
#include <forge/memory.h>
#include <forge/messages/message_buffer.h>

frg_message_buffer_t* frg_message_buffer_new() {
  frg_message_buffer_t* message_buffer = frg_malloc(sizeof(frg_message_buffer_t));

  message_buffer->_messages            = NULL;
  message_buffer->_max_line_number     = 0;
  message_buffer->message_count        = 0;
  message_buffer->error_count          = 0;
  message_buffer->warning_count        = 0;

  return message_buffer;
}

void frg_message_buffer_destroy(frg_message_buffer_t* message_buffer) {
  frg_assert_pointer_non_null(message_buffer);

  for (GList* iter = message_buffer->_messages; iter != NULL; iter = iter->next) {
    frg_message_t* message = (frg_message_t*)iter->data;
    frg_message_destroy(message);
  }

  g_list_free(message_buffer->_messages);

  frg_free(message_buffer);
}

gint _frg_message_buffer_message_comparer(gconstpointer a, gconstpointer b) {
  const frg_message_t* message_a = (const frg_message_t*)a;
  const frg_message_t* message_b = (const frg_message_t*)b;

  frg_assert_pointer_non_null(message_a);
  frg_assert_pointer_non_null(message_b);

  // Sort by severity (high to low)
  if (message_a->severity > message_b->severity) {
    return -1;
  } else if (message_a->severity < message_b->severity) {
    return 1;
  }

  // Sort by source path (alphabetically with NULLs first)
  if (message_a->source_range.start.path == NULL
      && message_b->source_range.start.path != NULL) {
    return -1;
  } else if (message_a->source_range.start.path != NULL
             && message_b->source_range.start.path == NULL) {
    return 1;
  } else if (message_a->source_range.start.path != NULL
             && message_b->source_range.start.path != NULL) {
    int source_path_comparison
      = strcmp(message_a->source_range.start.path, message_b->source_range.start.path);

    if (source_path_comparison != 0) {
      return source_path_comparison;
    }
  }

  // Sort by source offset (low to high)
  if (message_a->source_range.start.offset < message_b->source_range.start.offset) {
    return -1;
  } else if (message_a->source_range.start.offset
             > message_b->source_range.start.offset) {
    return 1;
  }

  // Sort by source range length (high to low)
  if (message_a->source_range.length > message_b->source_range.length) {
    return -1;
  } else if (message_a->source_range.length < message_b->source_range.length) {
    return 1;
  }

  // Sort by log path (alphabetically)
  int log_path_comparison = strcmp(message_a->log_path, message_b->log_path);

  if (log_path_comparison != 0) {
    return log_path_comparison;
  }

  // Sort by log line number (low to high)
  if (message_a->log_line_number < message_b->log_line_number) {
    return -1;
  } else if (message_a->log_line_number > message_b->log_line_number) {
    return 1;
  }

  // Sort by text (alphabetically)
  int text_comparison = strcmp(message_a->text->str, message_b->text->str);

  if (text_comparison != 0) {
    return text_comparison;
  }

  return 0;
}

void _frg_message_buffer_print_summary_if_errors(
  frg_stream_output_t* mut_stream, const frg_message_buffer_t* message_buffer) {
  if (message_buffer->error_count > 0 || message_buffer->warning_count > 0) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_WHITE);
    if (message_buffer->error_count > 0) {
      frg_stream_output_write_printf(mut_stream, "Failed with ");
    } else {
      frg_stream_output_write_printf(mut_stream, "Succeeded with ");
    }
  }

  if (message_buffer->error_count > 0) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RED);
    frg_stream_output_write_printf(mut_stream,
                                   "%u error%s",
                                   message_buffer->error_count,
                                   message_buffer->error_count == 1 ? "" : "s");
  }

  if (message_buffer->error_count > 0 && message_buffer->warning_count > 0) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_WHITE);
    frg_stream_output_write_string(mut_stream, " and ");
  }

  if (message_buffer->warning_count > 0) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BOLD);
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_BRIGHT_YELLOW);
    frg_stream_output_write_printf(mut_stream,
                                   "%u warning%s",
                                   message_buffer->warning_count,
                                   message_buffer->warning_count == 1 ? "" : "s");
  }

  if (message_buffer->error_count > 0 || message_buffer->warning_count > 0) {
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_WHITE);
    frg_stream_output_write_printf(mut_stream, ".\n");
    frg_stream_output_set_color(mut_stream, FRG_STREAM_OUTPUT_COLOR_RESET);
  }
}

void frg_message_buffer_print(frg_stream_output_t* mut_stream,
                              frg_parsing_source_context_t* mut_source_context,
                              frg_message_buffer_t* mut_message_buffer,
                              frg_message_severity_t minimum_severity,
                              frg_message_count_t max_messages) {
  frg_assert_pointer_non_null(mut_stream);
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_int_greater_than_or_equal_to(minimum_severity, FRG_MESSAGE_SEVERITY_DEBUG);
  frg_assert_int_less_than_or_equal_to(minimum_severity, FRG_MESSAGE_SEVERITY_NOTE);

  frg_column_number_t line_number_pad_to_width
    = frg_get_uint32_digit_count(mut_message_buffer->_max_line_number);

  mut_message_buffer->_messages
    = g_list_sort(mut_message_buffer->_messages, _frg_message_buffer_message_comparer);

  // frg_message_count_t printed_message_count = 0;

  for (GList* iter = mut_message_buffer->_messages; iter != NULL; iter = iter->next) {
    // if (printed_message_count >= max_messages) {
    //     // TODO: Add suggestion here
    //     // TODO: Fix this
    //     // frg_message_t* message_limit_reached = frg_message_new(
    //     //     __FILE__,
    //     //     __LINE__,
    //     //     NULL,
    //     //     FRG_MESSAGE_SEVERITY_FATAL_ERROR,
    //     //     "Too many messages to show, displaying highest priority %i/%i",
    //     //     printed_message_count,
    //     //     g_list_length(message_buffer->_messages)
    //     // );

    // // frg_message_print(
    // //     file,
    // //     message_limit_reached,
    // //     line_number_pad_to_width,
    // //     source_context
    // // );

    // break;
    // }

    frg_message_t* message = (frg_message_t*)iter->data;

    if (message->severity < minimum_severity) {
      continue;
    }

    frg_message_print(
      mut_stream, mut_source_context, message, line_number_pad_to_width);

    // printed_message_count++;
  }

  _frg_message_buffer_print_summary_if_errors(mut_stream, mut_message_buffer);
}

bool frg_message_buffer_has_message_with_text_equal_to(
  const frg_message_buffer_t* message_buffer, const char* text) {
  frg_assert_pointer_non_null(message_buffer);
  frg_assert_string_non_empty(text);

  for (GList* iter = message_buffer->_messages; iter != NULL; iter = iter->next) {
    frg_message_t* message = (frg_message_t*)iter->data;

    if (strcmp(message->text->str, text) == 0) {
      return true;
    }
  }

  return false;
}

void _frg_message_buffer_update_counters(frg_message_buffer_t* mut_message_buffer,
                                         const frg_message_t* message) {
  mut_message_buffer->_max_line_number = MAX(mut_message_buffer->_max_line_number,
                                             message->source_range.start.line_number);

  mut_message_buffer->message_count++;

  if (message->severity >= FRG_MESSAGE_SEVERITY_ERROR) {
    mut_message_buffer->error_count++;
  } else if (message->severity == FRG_MESSAGE_SEVERITY_WARNING) {
    mut_message_buffer->warning_count++;
  }
}

frg_message_t* _frg_message_emit_helper(frg_message_buffer_t* mut_message_buffer,
                                        const char* log_path,
                                        frg_line_number_t log_line_number,
                                        const frg_parsing_range_t* source_range,
                                        frg_message_severity_t severity,
                                        const char* code,
                                        const char* format,
                                        va_list args) {
  frg_assert_string_non_empty(log_path);
  frg_assert_int_positive(log_line_number);
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_string_non_empty(format);

  GString* text = g_string_new(NULL);
  g_string_vprintf(text, format, args);

  frg_assert_gstring_non_empty(text);

  frg_message_t* message
    = frg_message_new(log_path, log_line_number, source_range, severity, code, text);

  mut_message_buffer->_messages
    = g_list_prepend(mut_message_buffer->_messages, message);

  _frg_message_buffer_update_counters(mut_message_buffer, message);

  return message;
}

void _frg_message_emit_child_helper(frg_message_buffer_t* mut_message_buffer,
                                    frg_message_t* mut_parent,
                                    const char* log_path,
                                    frg_line_number_t log_line_number,
                                    const frg_parsing_range_t* source_range,
                                    frg_message_severity_t severity,
                                    const char* code,
                                    const char* format,
                                    va_list args) {
  frg_assert_string_non_empty(log_path);
  frg_assert_int_positive(log_line_number);
  frg_assert_pointer_non_null(mut_message_buffer);
  frg_assert_pointer_non_null(mut_parent);
  frg_assert_string_non_empty(format);

  GString* text = g_string_new(NULL);
  g_string_vprintf(text, format, args);

  frg_assert_gstring_non_empty(text);

  frg_message_t* message
    = frg_message_new(log_path, log_line_number, source_range, severity, code, text);

  frg_message_add_child(mut_parent, message);

  _frg_message_buffer_update_counters(mut_message_buffer, message);
}

frg_message_t* _frg_message_emit(frg_message_buffer_t* mut_message_buffer,
                                 const char* log_path,
                                 frg_line_number_t log_line_number,
                                 frg_message_severity_t severity,
                                 const char* code,
                                 const char* format,
                                 ...) {
  va_list args;
  va_start(args, format);

  frg_message_t* message = _frg_message_emit_helper(
    mut_message_buffer, log_path, log_line_number, NULL, severity, code, format, args);

  va_end(args);

  return message;
}

frg_message_t* _frg_message_emit_from_source_range(
  frg_message_buffer_t* mut_message_buffer,
  const char* log_path,
  frg_line_number_t log_line_number,
  const frg_parsing_range_t* source_range,
  frg_message_severity_t severity,
  const char* code,
  const char* format,
  ...) {
  frg_assert_pointer_non_null(source_range);

  va_list args;
  va_start(args, format);

  frg_message_t* message = _frg_message_emit_helper(mut_message_buffer,
                                                    log_path,
                                                    log_line_number,
                                                    source_range,
                                                    severity,
                                                    code,
                                                    format,
                                                    args);

  va_end(args);

  return message;
}

void _frg_message_emit_child(frg_message_buffer_t* mut_message_buffer,
                             frg_message_t* mut_parent,
                             const char* log_path,
                             frg_line_number_t log_line_number,
                             frg_message_severity_t severity,
                             const char* code,
                             const char* format,
                             ...) {
  va_list args;
  va_start(args, format);

  _frg_message_emit_child_helper(mut_message_buffer,
                                 mut_parent,
                                 log_path,
                                 log_line_number,
                                 NULL,
                                 severity,
                                 code,
                                 format,
                                 args);

  va_end(args);
}

void _frg_message_emit_child_from_source_range(frg_message_buffer_t* mut_message_buffer,
                                               frg_message_t* mut_parent,
                                               const char* log_path,
                                               frg_line_number_t log_line_number,
                                               const frg_parsing_range_t* source_range,
                                               frg_message_severity_t severity,
                                               const char* code,
                                               const char* format,
                                               ...) {
  va_list args;
  va_start(args, format);

  _frg_message_emit_child_helper(mut_message_buffer,
                                 mut_parent,
                                 log_path,
                                 log_line_number,
                                 source_range,
                                 severity,
                                 code,
                                 format,
                                 args);

  va_end(args);
}

size_t frg_message_buffer_query_count(const frg_message_buffer_t* message_buffer,
                                      const frg_message_query_t* query) {
  frg_assert_pointer_non_null(message_buffer);
  frg_assert_pointer_non_null(query);

  size_t count = 0;

  for (GList* iter = message_buffer->_messages; iter != NULL; iter = iter->next) {
    frg_message_t* message = (frg_message_t*)iter->data;

    if (frg_message_matches_query(message, query)) {
      count++;
    }
  }

  return count;
}

frg_message_buffer_query_single_result_t frg_message_buffer_query_single(
  frg_message_t** out_message,
  const frg_message_buffer_t* message_buffer,
  const frg_message_query_t* query) {
  frg_assert_pointer_non_null(message_buffer);
  frg_assert_pointer_non_null(query);

  *out_message = NULL;

  for (GList* iter = message_buffer->_messages; iter != NULL; iter = iter->next) {
    frg_message_t* message = (frg_message_t*)iter->data;

    if (frg_message_matches_query(message, query)) {
      if (*out_message == NULL) {
        *out_message = message;
      } else {
        // The order that messages appear in _messages is not guaranteed to be stable,
        // so we just null it out to prevent unexpected behavior.
        *out_message = NULL;
        return FRG_MESSAGE_BUFFER_QUERY_RESULT_MULTIPLE;
      }
    }
  }

  if (*out_message == NULL) {
    return FRG_MESSAGE_BUFFER_QUERY_RESULT_NONE;
  } else {
    return FRG_MESSAGE_BUFFER_QUERY_RESULT_SINGLE;
  }
}

GList* frg_message_buffer_query(const frg_message_buffer_t* message_buffer,
                                const frg_message_query_t* query) {
  frg_assert_pointer_non_null(message_buffer);
  frg_assert_pointer_non_null(query);

  GList* results = NULL;

  for (GList* iter = message_buffer->_messages; iter != NULL; iter = iter->next) {
    frg_message_t* message = (frg_message_t*)iter->data;

    if (frg_message_matches_query(message, query)) {
      results = g_list_append(results, message);
    }
  }

  return results;
}
