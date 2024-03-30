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

#pragma once

#include <forge/messages/message.h>

typedef struct {
  GList* _messages;
  frg_line_number_t _max_line_number;
  frg_message_count_t message_count;
  frg_message_count_t error_count;
  frg_message_count_t warning_count;
} frg_message_buffer_t;

frg_message_buffer_t* frg_message_buffer_new();

void frg_message_buffer_destroy(frg_message_buffer_t* message_buffer);

void frg_message_buffer_print(frg_stream_output_t* mut_stream,
                              frg_parsing_source_context_t* mut_source_context,
                              frg_message_buffer_t* mut_message_buffer,
                              frg_message_severity_t minimum_severity,
                              frg_message_count_t max_messages);

frg_message_t* _frg_message_emit(frg_message_buffer_t* mut_message_buffer,
                                 const char* log_path,
                                 frg_line_number_t log_line_number,
                                 frg_message_severity_t severity,
                                 const char* code,
                                 const char* format,
                                 ...);

#define frg_message_emit(mut_message_buffer, severity, code, format, ...)              \
  _frg_message_emit((mut_message_buffer),                                              \
                    __FILE__,                                                          \
                    __LINE__,                                                          \
                    (severity),                                                        \
                    (code),                                                            \
                    (format),                                                          \
                    ##__VA_ARGS__)

frg_message_t* _frg_message_emit_from_source_range(
  frg_message_buffer_t* mut_message_buffer,
  const char* log_path,
  frg_line_number_t log_line_number,
  const frg_parsing_range_t* source_range,
  frg_message_severity_t severity,
  const char* code,
  const char* format,
  ...);

#define frg_message_emit_from_source_range(                                            \
  mut_message_buffer, source_range, severity, code, format, ...)                       \
  _frg_message_emit_from_source_range((mut_message_buffer),                            \
                                      __FILE__,                                        \
                                      __LINE__,                                        \
                                      (source_range),                                  \
                                      (severity),                                      \
                                      (code),                                          \
                                      (format),                                        \
                                      ##__VA_ARGS__)

void _frg_message_emit_child(frg_message_buffer_t* mut_message_buffer,
                             frg_message_t* mut_parent,
                             const char* log_path,
                             frg_line_number_t log_line_number,
                             frg_message_severity_t severity,
                             const char* code,
                             const char* format,
                             ...);

#define frg_message_emit_child(                                                        \
  mut_message_buffer, mut_parent, severity, code, format, ...)                         \
  _frg_message_emit_child((mut_message_buffer),                                        \
                          (mut_parent),                                                \
                          __FILE__,                                                    \
                          __LINE__,                                                    \
                          (severity),                                                  \
                          (code),                                                      \
                          (format),                                                    \
                          ##__VA_ARGS__)

void _frg_message_emit_child_from_source_range(frg_message_buffer_t* mut_message_buffer,
                                               frg_message_t* mut_parent,
                                               const char* log_path,
                                               frg_line_number_t log_line_number,
                                               const frg_parsing_range_t* source_range,
                                               frg_message_severity_t severity,
                                               const char* code,
                                               const char* format,
                                               ...);

#define frg_message_emit_child_from_source_range(                                      \
  mut_message_buffer, mut_parent, source_range, severity, code, format, ...)           \
  _frg_message_emit_child_from_source_range((mut_message_buffer),                      \
                                            (mut_parent),                              \
                                            __FILE__,                                  \
                                            __LINE__,                                  \
                                            (source_range),                            \
                                            (severity),                                \
                                            (code),                                    \
                                            (format),                                  \
                                            ##__VA_ARGS__)

size_t frg_message_buffer_query_count(const frg_message_buffer_t* message_buffer,
                                      const frg_message_query_t* query);

frg_message_buffer_query_single_result_t frg_message_buffer_query_single(
  frg_message_t** out_message,
  const frg_message_buffer_t* message_buffer,
  const frg_message_query_t* query);

GList* frg_message_buffer_query(const frg_message_buffer_t* message_buffer,
                                const frg_message_query_t* query);
