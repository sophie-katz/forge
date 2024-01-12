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

#include <forge/messages/message_buffer.h>
#include <forge/common/error.h>
#include <forge/common/color.h>
#include <forge/common/math.h>
#include <forge/common/memory.h>

frg_message_buffer_t* frg_message_buffer_new() {
    frg_message_buffer_t* message_buffer = frg_safe_malloc(sizeof(frg_message_buffer_t));
    
    message_buffer->messages = NULL;
    message_buffer->max_lineno = 0;
    message_buffer->message_count = 0;
    message_buffer->error_count = 0;
    message_buffer->warning_count = 0;

    return message_buffer;
}

void frg_message_buffer_destroy(frg_message_buffer_t** message_buffer) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(*message_buffer);

    for (GList* iter = (*message_buffer)->messages; iter != NULL; iter = iter->next) {
        frg_message_t* message = (frg_message_t*)iter->data;
        frg_message_destroy(&message);
    }

    g_list_free((*message_buffer)->messages);

    frg_safe_free((void**)message_buffer);
}

// TODO: Finish this
gint _frg_message_buffer_message_comparer(gconstpointer a, gconstpointer b) {
    const frg_message_t* message_a = (const frg_message_t*)a;
    const frg_message_t* message_b = (const frg_message_t*)b;

    // Sort by severity (high to low)
    if (message_a->severity > message_b->severity) {
        return -1;
    } else if (message_a->severity < message_b->severity) {
        return 1;
    }

    // Sort by source path (alphabetically with NULLs first)
    if (message_a->source_range.start.path == NULL && message_a->source_range.start.path != NULL) {
        return -1;
    } else if (message_a->source_range.start.path != NULL && message_a->source_range.start.path == NULL) {
        return 1;
    } else if (message_a->source_range.start.path != NULL && message_a->source_range.start.path != NULL) {
        int source_path_comparison = strcmp(message_a->source_range.start.path, message_b->source_range.start.path);

        if (source_path_comparison != 0) {
            return source_path_comparison;
        }
    }
    
    // Sort by source offset (low to high)
    if (message_a->source_range.start.offset < message_b->source_range.start.offset) {
        return -1;
    } else if (message_a->source_range.start.offset > message_b->source_range.start.offset) {
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
    if (message_a->log_lineno < message_b->log_lineno) {
        return -1;
    } else if (message_a->log_lineno > message_b->log_lineno) {
        return 1;
    }

    // Sort by text (alphabetically)
    int text_comparison = strcmp(message_a->text->str, message_b->text->str);

    if (text_comparison != 0) {
        return text_comparison;
    }

    return 0;
}

void _frg_log_summary_if_errors(
    FILE* file,
    frg_message_buffer_t* message_buffer
) {
    if (message_buffer->error_count > 0 || message_buffer->warning_count > 0) {
        frg_color_set(file, FRG_COLOR_ID_WHITE);
        if (message_buffer->error_count > 0) {
            fprintf(file, "Failed with ");
        } else {
            fprintf(file, "Succeeded with ");
        }
    }

    if (message_buffer->error_count > 0) {
        frg_color_set(file, FRG_COLOR_ID_BOLD);
        frg_color_set(file, FRG_COLOR_ID_RED);
        fprintf(
            file,
            "%u error%s",
            message_buffer->error_count,
            message_buffer->error_count == 1 ? "" : "s"
        );
    }

    if (message_buffer->error_count > 0 && message_buffer->warning_count > 0) {
        frg_color_set(file, FRG_COLOR_ID_RESET);
        frg_color_set(file, FRG_COLOR_ID_WHITE);
        fprintf(
            file,
            " and "
        );
    }

    if (message_buffer->warning_count > 0) {
        frg_color_set(file, FRG_COLOR_ID_BOLD);
        frg_color_set(file, FRG_COLOR_ID_BRIGHT_YELLOW);
        fprintf(
            file,
            "%u warning%s",
            message_buffer->warning_count,
            message_buffer->warning_count == 1 ? "" : "s"
        );
    }

    if (message_buffer->error_count > 0 || message_buffer->warning_count > 0) {
        frg_color_set(file, FRG_COLOR_ID_WHITE);
        fprintf(file, ".\n");
        frg_color_set(file, FRG_COLOR_ID_RESET);
    }
}

void frg_message_buffer_print(
    FILE* file,
    frg_message_buffer_t* message_buffer,
    frg_parsing_source_context_t* source_context,
    frg_message_severity_t minimum_severity,
    frg_message_count_t max_messages
) {
    frg_assert_pointer_non_null(file);
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_int_ge(minimum_severity, FRG_MESSAGE_SEVERITY_DEBUG);
    frg_assert_int_le(minimum_severity, FRG_MESSAGE_SEVERITY_NOTE);

    frg_columnno_t lineno_pad_to_width = frg_get_uint32_digit_count(
        message_buffer->max_lineno
    );

    message_buffer->messages = g_list_sort(
        message_buffer->messages,
        _frg_message_buffer_message_comparer
    );

    // frg_message_count_t printed_message_count = 0;

    for (GList* iter = message_buffer->messages; iter != NULL; iter = iter->next) {
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
        //     //     g_list_length(message_buffer->messages)
        //     // );

        //     // frg_message_print(
        //     //     file,
        //     //     message_limit_reached,
        //     //     lineno_pad_to_width,
        //     //     source_context
        //     // );

        //     break;
        // }

        frg_message_t* message = (frg_message_t*)iter->data;

        if (message->severity < minimum_severity) {
            continue;
        }

        frg_message_print(
            file,
            message,
            lineno_pad_to_width,
            source_context
        );

        // printed_message_count++;
    }

    _frg_log_summary_if_errors(
        file,
        message_buffer
    );
}

frg_message_count_t frg_message_buffer_get_message_count(
    const frg_message_buffer_t* message_buffer
) {
    frg_assert_pointer_non_null(message_buffer);
    
    return message_buffer->message_count;
}

frg_message_count_t frg_message_buffer_get_error_count(
    const frg_message_buffer_t* message_buffer
) {
    frg_assert_pointer_non_null(message_buffer);
    
    return message_buffer->error_count;
}

frg_message_count_t frg_message_buffer_get_warning_count(
    const frg_message_buffer_t* message_buffer
) {
    frg_assert_pointer_non_null(message_buffer);
    
    return message_buffer->warning_count;
}

bool frg_message_buffer_has_message_with_text(
    const frg_message_buffer_t* message_buffer,
    const char* text
) {
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_string_non_empty(text);
    
    for (GList* iter = message_buffer->messages; iter != NULL; iter = iter->next) {
        frg_message_t* message = (frg_message_t*)iter->data;

        if (strcmp(message->text->str, text) == 0) {
            return true;
        }
    }

    return false;
}

void _frg_message_buffer_update_counters(
    frg_message_buffer_t* message_buffer,
    const frg_message_t* message
) {
    message_buffer->max_lineno = MAX(
        message_buffer->max_lineno,
        message->source_range.start.lineno
    );

    message_buffer->message_count++;

    if (message->severity >= FRG_MESSAGE_SEVERITY_ERROR) {
        message_buffer->error_count++;
    } else if (message->severity == FRG_MESSAGE_SEVERITY_WARNING) {
        message_buffer->warning_count++;
    }
}

frg_message_t* _frg_message_emit_helper(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    const frg_parsing_range_t* source_range,
    frg_message_severity_t severity,
    const char* format,
    va_list args
) {
    frg_assert_string_non_empty(log_path);
    frg_assert_int_positive(log_lineno);
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_string_non_empty(format);

    GString* text = g_string_new(NULL);
    g_string_vprintf(text, format, args);

    frg_assert_gstring_non_empty(text);

    frg_message_t* message = frg_message_new(
        log_path,
        log_lineno,
        source_range,
        severity,
        text
    );

    message_buffer->messages = g_list_append(
        message_buffer->messages,
        message
    );

    _frg_message_buffer_update_counters(
        message_buffer,
        message
    );

    return message;
}

void _frg_message_emit_child_helper(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    frg_message_t* parent,
    const frg_parsing_range_t* source_range,
    frg_message_severity_t severity,
    const char* format,
    va_list args
) {
    frg_assert_string_non_empty(log_path);
    frg_assert_int_positive(log_lineno);
    frg_assert_pointer_non_null(message_buffer);
    frg_assert_pointer_non_null(parent);
    frg_assert_string_non_empty(format);

    GString* text = g_string_new(NULL);
    g_string_vprintf(text, format, args);

    frg_assert_gstring_non_empty(text);

    frg_message_t* message = frg_message_new(
        log_path,
        log_lineno,
        source_range,
        severity,
        text
    );

    frg_message_add_child(
        parent,
        message
    );

    _frg_message_buffer_update_counters(
        message_buffer,
        message
    );
}

frg_message_t* _frg_message_emit(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    frg_message_severity_t severity,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_message_t* message = _frg_message_emit_helper(
        log_path,
        log_lineno,
        message_buffer,
        NULL,
        severity,
        format,
        args
    );

    va_end(args);

    return message;
}

frg_message_t* _frg_message_emit_from_source_range(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    const frg_parsing_range_t* source_range,
    frg_message_severity_t severity,
    const char* format,
    ...
) {
    frg_assert_pointer_non_null(source_range);

    va_list args;
    va_start(args, format);

    frg_message_t* message = _frg_message_emit_helper(
        log_path,
        log_lineno,
        message_buffer,
        source_range,
        severity,
        format,
        args
    );

    va_end(args);

    return message;
}

void _frg_message_emit_child(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    frg_message_t* parent,
    frg_message_severity_t severity,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    _frg_message_emit_child_helper(
        log_path,
        log_lineno,
        message_buffer,
        parent,
        NULL,
        severity,
        format,
        args
    );

    va_end(args);
}

void _frg_message_emit_child_from_source_range(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_buffer_t* message_buffer,
    frg_message_t* parent,
    const frg_parsing_range_t* source_range,
    frg_message_severity_t severity,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    _frg_message_emit_child_helper(
        log_path,
        log_lineno,
        message_buffer,
        parent,
        source_range,
        severity,
        format,
        args
    );

    va_end(args);
}
