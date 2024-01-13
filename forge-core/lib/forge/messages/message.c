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

#include <forge/messages/message.h>
#include <forge/common/error.h>
#include <forge/common/math.h>
#include <forge/common/memory.h>

frg_message_t* frg_message_new(
    const char* log_path,
    frg_lineno_t log_lineno,
    const frg_parsing_range_t* source_range,
    frg_message_severity_t severity,
    GString* text
) {
    frg_assert_string_non_empty(log_path);
    frg_assert_int_ge(log_lineno, 1);
    frg_assert_gstring_non_empty(text);
    
    frg_message_t* message = frg_safe_malloc(sizeof(frg_message_t));

    message->log_path = log_path;
    message->log_lineno = log_lineno;

    if (source_range == NULL) {
        message->source_range.length = 0;
        message->source_range.start.path = NULL;
        message->source_range.start.offset = 0;
        message->source_range.start.lineno = 0;
        message->source_range.start.columnno = 0;
    } else {
        if (source_range->start.path == NULL) {
            frg_assert_int_eq(source_range->start.lineno, 0);
            frg_assert_int_eq(source_range->start.columnno, 0);
        } else {
            frg_assert_string_non_empty(source_range->start.path);

            if (source_range->start.lineno == 0) {
                frg_assert_int_eq(source_range->start.columnno, 0);
            }
        }

        message->source_range = *source_range;
    }

    message->severity = severity;
    message->text = text;
    message->children = NULL;

    return message;
}

void frg_message_destroy(frg_message_t** message) {
    frg_assert_pointer_non_null(message);
    frg_assert_pointer_non_null(*message);

    g_string_free((*message)->text, TRUE);

    for (GList* iter = (*message)->children; iter != NULL; iter = iter->next) {
        frg_message_t* child = (frg_message_t*)iter->data;
        frg_message_destroy(&child);
    }

    g_list_free((*message)->children);

    frg_safe_free((void**)message);
}

void frg_message_add_child(frg_message_t* message, frg_message_t* child) {
    frg_assert_pointer_non_null(message);
    frg_assert_pointer_non_null(child);

    if (child->severity != FRG_MESSAGE_SEVERITY_NOTE) {
        frg_die("currently child messages can only have severity 'note'");
    }

    message->children = g_list_append(message->children, child);
}

bool _frg_message_should_print_internal_prefix(frg_message_severity_t severity) {
#ifdef FRG_ALWAYS_PRINT_INTERNAL_MESSAGE_PREFIX
    return true;
#else
    return severity == FRG_MESSAGE_SEVERITY_DEBUG
        || severity == FRG_MESSAGE_SEVERITY_INTERNAL_ERROR;
#endif
}

void _frg_message_print_prefix_internal(
    frg_stream_output_t* stream,
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_message_severity_t severity
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_string_non_empty(log_path);
    frg_assert_int_ge(log_lineno, 1);

    if (_frg_message_should_print_internal_prefix(severity)) {
        const char* log_path_resolved = strstr(log_path, "lib/");

        if (log_path_resolved == NULL) {
            log_path_resolved = strstr(log_path, "src/");
        }

        if (log_path_resolved == NULL) {
            log_path_resolved = log_path;
        }

        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_BLACK);
        frg_stream_output_write_printf(stream, "[internally from forge-core/%s:%d]\n", log_path_resolved, log_lineno);
        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
    }
}

void _frg_message_print_prefix_source_location(
    frg_stream_output_t* stream,
    const frg_parsing_location_t* source_location
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(source_location);
    
    if (source_location->path != NULL) {
        frg_assert_string_non_empty(source_location->path);

        if (source_location->lineno == 0) {
            frg_assert_int_eq(source_location->columnno, 0);
        }

        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_CYAN);
        frg_stream_output_write_printf(stream, "%s:", source_location->path);

        if (source_location->lineno != 0) {
            frg_stream_output_write_printf(stream, "%i:", source_location->lineno);

            if (source_location->columnno != 0) {
                frg_stream_output_write_printf(stream, "%i:", source_location->columnno);
            }
        } else {
            frg_assert_int_eq(source_location->columnno, 0);
        }

        frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
        frg_stream_output_write_char(stream, ' ');
    } else {
        frg_assert_int_eq(source_location->lineno, 0);
        frg_assert_int_eq(source_location->columnno, 0);
    }
}

void _frg_message_print_prefix_severity(
    frg_stream_output_t* stream,
    frg_message_severity_t severity
) {
    frg_assert_pointer_non_null(stream);

    switch (severity) {
        case FRG_MESSAGE_SEVERITY_DEBUG:
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_MAGENTA);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
            frg_stream_output_write_printf(stream, "debug: ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
            break;
        case FRG_MESSAGE_SEVERITY_NOTE:
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_BLACK);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
            frg_stream_output_write_printf(stream, "note: ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
            break;
        case FRG_MESSAGE_SEVERITY_WARNING:
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_YELLOW);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
            frg_stream_output_write_printf(stream, "warning: ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
            break;
        case FRG_MESSAGE_SEVERITY_ERROR:
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RED);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
            frg_stream_output_write_printf(stream, "error: ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
            break;
        case FRG_MESSAGE_SEVERITY_FATAL_ERROR:
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_RED);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_UNDERLINE);
            frg_stream_output_write_printf(stream, "fatal error");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_RED);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
            frg_stream_output_write_printf(stream, ": ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
            break;
        case FRG_MESSAGE_SEVERITY_INTERNAL_ERROR:
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_CYAN);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
            frg_stream_output_write_printf(stream, "internal error: ");
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
            break;
        default:
            frg_die_unexpected_enum_value(severity);
    }
}

void _frg_message_print_text(
    frg_stream_output_t* stream,
    const char* text
) {
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_WHITE);
    frg_stream_output_write_string(stream, text);
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
}

void _frg_message_print_suffix(frg_stream_output_t* stream) {
    frg_assert_pointer_non_null(stream);

    frg_stream_output_write_printf(stream, "\n");
}

bool _frg_message_should_print_sample(const frg_message_t* message) {
    frg_assert_pointer_non_null(message);

    return message->source_range.start.path != NULL
        && (message->source_range.start.lineno != 0
        && message->source_range.start.columnno != 0);
}

void _frg_message_print_sample_prefix_lineno(
    frg_stream_output_t* stream,
    frg_lineno_t lineno,
    frg_columnno_t lineno_pad_to_width
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_int_ge(lineno, 1);
    frg_assert_int_ge(lineno_pad_to_width, 1);

    uint32_t lineno_digits = frg_get_uint32_digit_count(lineno);

    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_BLACK);

    frg_stream_output_write_char(stream, ' ');

    for (uint32_t i = lineno_digits; i < lineno_pad_to_width; i++) {
        frg_stream_output_write_char(stream, ' ');
    }

    frg_stream_output_write_printf(
        stream,
        "%i %s ",
        lineno,
        frg_stream_output_choose_unicode(
            stream,
            "|",
            "▏"
        )
    );

    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
}

void _frg_message_print_sample_prefix_blank(
    frg_stream_output_t* stream,
    frg_columnno_t lineno_pad_to_width
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_int_ge(lineno_pad_to_width, 1);

    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_BLACK);

    for (uint32_t i = 0; i < lineno_pad_to_width; i++) {
        frg_stream_output_write_char(stream, ' ');
    }

    frg_stream_output_write_printf(
        stream,
        "  %s ",
        frg_stream_output_choose_unicode(
            stream,
            "|",
            "▏"
        )
    );

    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);
}

typedef struct {
    ssize_t chars_before;
    ssize_t chars_within;
} _frg_message_print_sample_line_result_t;

_frg_message_print_sample_line_result_t _frg_message_print_sample_line(
    frg_stream_output_t* stream,
    gchar** text_iterator,
    ssize_t* text_offset,
    ssize_t sample_start,
    ssize_t sample_length,
    frg_stream_output_color_id_t color_id
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(text_iterator);
    frg_assert_pointer_non_null(*text_iterator);
    frg_assert_pointer_non_null(text_offset);
    frg_assert_int_non_negative(*text_offset);
    frg_assert_int_non_negative(sample_start);
    frg_assert_int_positive(sample_length);

    _frg_message_print_sample_line_result_t result = {
        .chars_before = 0,
        .chars_within = 0
    };

    while (**text_iterator != 0 &&
        (**text_iterator != '\n' && **text_iterator != '\r')) {
        if (*text_offset >= sample_start &&
            *text_offset <= sample_start + sample_length) {
            frg_stream_output_set_color(stream, color_id);
            frg_stream_output_write_char(stream, **text_iterator);
            frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);

            result.chars_within++;
        } else {
            frg_stream_output_write_char(stream, **text_iterator);

            if (result.chars_within == 0) {
                result.chars_before++;
            }
        }

        (*text_iterator)++;
        (*text_offset)++;
    }

    if (**text_iterator == '\n' || **text_iterator == '\r') {
        (*text_iterator)++;
        (*text_offset)++;
    }

    frg_stream_output_write_char(stream, '\n');

    return result;
}

void _frg_message_print_sample_underline(
    frg_stream_output_t* stream,
    const _frg_message_print_sample_line_result_t* line_result,
    frg_stream_output_color_id_t color_id
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(line_result);
    frg_assert_int_non_negative(line_result->chars_before);
    frg_assert_int_positive(line_result->chars_within);

    for (ssize_t i = 0; i < line_result->chars_before; i++) {
        frg_stream_output_write_char(stream, ' ');
    }

    frg_stream_output_set_color(stream, color_id);
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
    for (ssize_t i = 0; i < line_result->chars_within; i++) {
        frg_stream_output_write_string(
            stream,
            frg_stream_output_choose_unicode(
                stream,
                "^",
                "▔"
            )
        );
    }
    frg_stream_output_set_color(stream, FRG_STREAM_OUTPUT_COLOR_ID_RESET);

    frg_stream_output_write_char(stream, '\n');
}

void _frg_message_print_sample(
    frg_stream_output_t* stream,
    const frg_message_t* message,
    frg_columnno_t lineno_pad_to_width,
    frg_parsing_source_context_t* source_context
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(message);
    frg_assert_int_ge(lineno_pad_to_width, 1);
    frg_assert_pointer_non_null(source_context);

    if (_frg_message_should_print_sample(message)) {
        frg_parsing_source_t* source = frg_parsing_source_context_get_source_by_path(
            source_context,
            message->source_range.start.path
        );

        frg_assert_pointer_non_null(source);

        GString* loaded = frg_parsing_source_load_range(
            source,
            &message->source_range
        );

        frg_assert_gstring_non_empty(loaded);

        gchar* text_iterator = loaded->str;
        ssize_t text_offset = message->source_range.start.offset - (message->source_range.start.columnno - 1);
        frg_lineno_t current_lineno = message->source_range.start.lineno;

        while (*text_iterator != 0) {
            _frg_message_print_sample_prefix_lineno(
                stream,
                current_lineno,
                lineno_pad_to_width
            );

            _frg_message_print_sample_line_result_t line_result = _frg_message_print_sample_line(
                stream,
                &text_iterator,
                &text_offset,
                message->source_range.start.offset,
                message->source_range.length,
                message->severity == FRG_MESSAGE_SEVERITY_WARNING
                    ? FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_YELLOW
                    : FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_RED
            );

            _frg_message_print_sample_prefix_blank(
                stream,
                lineno_pad_to_width
            );

            _frg_message_print_sample_underline(
                stream,
                &line_result,
                message->severity == FRG_MESSAGE_SEVERITY_WARNING
                    ? FRG_STREAM_OUTPUT_COLOR_ID_YELLOW
                    : FRG_STREAM_OUTPUT_COLOR_ID_RED
            );

            current_lineno++;
        }
    } else {
        frg_stream_output_write_printf(stream, "\n");
    }
}

void frg_message_print(
    frg_stream_output_t* stream,
    const frg_message_t* message,
    frg_columnno_t lineno_pad_to_width,
    frg_parsing_source_context_t* source_context
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(message);
    frg_assert_int_ge(lineno_pad_to_width, 1);

    _frg_message_print_prefix_internal(
        stream,
        message->log_path,
        message->log_lineno,
        message->severity
    );

    _frg_message_print_prefix_source_location(
        stream,
        &message->source_range.start
    );

    _frg_message_print_prefix_severity(
        stream,
        message->severity
    );

    _frg_message_print_text(
        stream,
        message->text->str
    );

    _frg_message_print_suffix(stream);

    // TODO: Add a warning here
    if (source_context != NULL) {
        _frg_message_print_sample(
            stream,
            message,
            lineno_pad_to_width,
            source_context
        );
    }

    for (GList* iter = message->children; iter != NULL; iter = iter->next) {
        frg_message_t* child = (frg_message_t*)iter->data;
        frg_message_print(
            stream,
            child,
            lineno_pad_to_width,
            source_context
        );
    }
}
