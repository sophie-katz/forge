// Copyright (c) 2024 Sophie Katz
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

#pragma once

#include <forge/messages/message.h>

typedef struct {
    GList* messages;
    frg_lineno_t max_lineno;
    frg_message_count_t message_count;
    frg_message_count_t error_count;
    frg_message_count_t warning_count;
} frg_message_buffer_t;

frg_message_buffer_t* frg_message_buffer_new();
void frg_message_buffer_destroy(frg_message_buffer_t** message_buffer);

void frg_message_buffer_print(
    FILE* file,
    frg_message_buffer_t* message_buffer,
    frg_parsing_source_context_t* source_context,
    frg_message_severity_t minimum_severity,
    frg_message_count_t max_messages
);

frg_message_count_t frg_message_buffer_get_message_count(
    const frg_message_buffer_t* message_buffer
);

frg_message_count_t frg_message_buffer_get_error_count(
    const frg_message_buffer_t* message_buffer
);

frg_message_count_t frg_message_buffer_get_warning_count(
    const frg_message_buffer_t* message_buffer
);

bool frg_message_buffer_has_message_with_text(
    const frg_message_buffer_t* message_buffer,
    const char* text
);

frg_message_t* _frg_message_emit(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    frg_message_severity_t severity,
    const char* format,
    ...
);

#define frg_message_emit(message_buffer, severity, format, ...) \
    _frg_message_emit( \
        __FILE__, \
        __LINE__, \
        (message_buffer), \
        (severity), \
        (format), \
        ##__VA_ARGS__ \
    )

frg_message_t* _frg_message_emit_from_source_range(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    const frg_parsing_range_t* source_range,
    frg_message_severity_t severity,
    const char* format,
    ...
);

#define frg_message_emit_from_source_range(message_buffer, source_range, severity, format, ...) \
    _frg_message_emit_from_source_range( \
        __FILE__, \
        __LINE__, \
        (message_buffer), \
        (source_range), \
        (severity), \
        (format), \
        ##__VA_ARGS__ \
    )

void _frg_message_emit_child(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    frg_message_t* parent,
    frg_message_severity_t severity,
    const char* format,
    ...
);

#define frg_message_emit_child(message_buffer, parent, severity, format, ...) \
    _frg_message_emit_child( \
        __FILE__, \
        __LINE__, \
        (message_buffer), \
        (parent), \
        (severity), \
        (format), \
        ##__VA_ARGS__ \
    )

void _frg_message_emit_child_from_source_range(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    frg_message_t* parent,
    const frg_parsing_range_t* source_range,
    frg_message_severity_t severity,
    const char* format,
    ...
);

#define frg_message_emit_child_from_source_range(message_buffer, parent, source_range, severity, format, ...) \
    _frg_message_emit_child_from_source_range( \
        __FILE__, \
        __LINE__, \
        (message_buffer), \
        (parent), \
        (source_range), \
        (severity), \
        (format), \
        ##__VA_ARGS__ \
    )
