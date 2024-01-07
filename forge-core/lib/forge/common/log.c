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

#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/common/stream.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <build_config.h>

static frg_log_severity_t _frg_log_minimum_severity = FRG_LOG_SEVERITY_NOTE;
static size_t _frg_log_count_error = 0;
static size_t _frg_log_count_warning = 0;

void frg_log_set_minimum_severity(frg_log_severity_t severity) {
    _frg_log_minimum_severity = severity;
}

void _frg_log_prefix_severity(frg_log_severity_t severity) {
    switch (severity) {
        case FRG_LOG_SEVERITY_TRACE:
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "trace: ");
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            break;
        case FRG_LOG_SEVERITY_DEBUG:
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_MAGENTA);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "debug: ");
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            break;
        case FRG_LOG_SEVERITY_NOTE:
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "note: ");
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            break;
        case FRG_LOG_SEVERITY_WARNING:
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_YELLOW);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "warning: ");
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            break;
        case FRG_LOG_SEVERITY_ERROR:
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RED);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "error: ");
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            break;
        case FRG_LOG_SEVERITY_FATAL_ERROR:
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_RED);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_UNDERLINE);
            fprintf(FRG_STREAM_DEFAULT, "fatal error");
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_RED);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, ": ");
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            break;
        case FRG_LOG_SEVERITY_INTERNAL_ERROR:
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_CYAN);
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "internal error: ");
            frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            break;
        default:
            fprintf(stderr, "internal logging error: unexpected value for 'severity': %i\n", severity);
            abort();
    }
}

void _frg_log_suffix(void) {
    fprintf(FRG_STREAM_DEFAULT, "\n");
}

void _frg_log_increment_counts(frg_log_severity_t severity) {
    switch (severity) {
        case FRG_LOG_SEVERITY_WARNING:
            _frg_log_count_warning++;
            break;
        case FRG_LOG_SEVERITY_ERROR:
        case FRG_LOG_SEVERITY_FATAL_ERROR:
        case FRG_LOG_SEVERITY_INTERNAL_ERROR:
            _frg_log_count_error++;
            break;
        default:
            break;
    }
}

void _frg_log_prefix_source_file(const char* source_path) {
    if (source_path == NULL) {
        fprintf(stderr, "internal logging error: source_path cannot be NULL\n");
        abort();
    } else if (*source_path == 0) {
        fprintf(stderr, "internal logging error: source_path must be non-empty\n");
        abort();
    }

    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "%s: ", source_path);
    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
}

void _frg_log_prefix_source_line(const char* source_path, frg_lineno_t source_lineno) {
    if (source_path == NULL) {
        fprintf(stderr, "internal logging error: source_path cannot be NULL\n");
        abort();
    } else if (*source_path == 0) {
        fprintf(stderr, "internal logging error: source_path must be non-empty\n");
        abort();
    } else if (source_lineno <= 0) {
        fprintf(stderr, "internal logging error: source_lineno must be positive\n");
        abort();
    }

    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "%s:%d: ", source_path, source_lineno);
    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
}

void _frg_log_prefix_source_char(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno
) {
    if (source_path == NULL) {
        fprintf(stderr, "internal logging error: source_path cannot be NULL\n");
        abort();
    } else if (*source_path == 0) {
        fprintf(stderr, "internal logging error: source_path must be non-empty\n");
        abort();
    } else if (source_lineno <= 0) {
        fprintf(stderr, "internal logging error: source_lineno must be positive\n");
        abort();
    } else if (source_columnno <= 0) {
        fprintf(stderr, "internal logging error: source_columnno must be positive\n");
        abort();
    }

    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "%s:%d:%d: ", source_path, source_lineno, source_columnno);
    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
}

void _frg_log_prefix_internal(
    const char* log_path,
    frg_lineno_t log_lineno
) {
    if (log_path == NULL) {
        fprintf(stderr, "internal logging error: log_path cannot be NULL\n");
        abort();
    } else if (*log_path == 0) {
        fprintf(stderr, "internal logging error: log_path must be non-empty\n");
        abort();
    } else if (log_lineno <= 0) {
        fprintf(stderr, "internal logging error: log_lineno must be positive\n");
        abort();
    }

    const char* log_path_resolved = strstr(log_path, "lib/");

    if (log_path_resolved == NULL) {
        log_path_resolved = strstr(log_path, "src/");
    }

    if (log_path_resolved == NULL) {
        log_path_resolved = log_path;
    }

    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "forge-core[%s:%d]: ", log_path_resolved, log_lineno);
    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
}

frg_log_result_t _frg_log_helper(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_log_severity_t severity,
    const char* format,
    va_list args
) {
    bool has_internal_prefix = severity == FRG_LOG_SEVERITY_DEBUG
        || severity == FRG_LOG_SEVERITY_INTERNAL_ERROR
        || severity == FRG_LOG_SEVERITY_TRACE;

    if (has_internal_prefix) {
        if (log_path == NULL) {
            fprintf(stderr, "internal logging error: log_path cannot be NULL\n");
            abort();
        } else if (*log_path == 0) {
            fprintf(stderr, "internal logging error: log_path cannot be empty\n");
            abort();
        } else if (log_lineno <= 0) {
            fprintf(stderr, "internal logging error: log_lineno must be positive\n");
            abort();
        }
    }

    if (format == NULL) {
        fprintf(stderr, "internal logging error: format cannot be NULL\n");
        abort();
    } else if (*format == 0) {
        fprintf(stderr, "internal logging error: format cannot be empty\n");
        abort();
    } else if (_frg_log_minimum_severity > severity) {
        return (frg_log_result_t) {
            .emitted = false
        };
    }

    if (has_internal_prefix) {
        _frg_log_prefix_internal(
            log_path,
            log_lineno
        );
    }

    if (source_path != NULL) {
        if (*source_path == 0) {
            fprintf(stderr, "internal logging error: source_path must be non-empty\n");
            abort();
        } else if (source_lineno > 0) {
            if (source_columnno > 0) {
                _frg_log_prefix_source_char(
                    source_path,
                    source_lineno,
                    source_columnno
                );
            } else {
                _frg_log_prefix_source_line(
                    source_path,
                    source_lineno
                );
            }
        } else {
            _frg_log_prefix_source_file(
                source_path
            );
        }
    }

    _frg_log_prefix_severity(severity);

    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
    vfprintf(FRG_STREAM_DEFAULT, format, args);
    frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

    _frg_log_suffix();

    fflush(FRG_STREAM_DEFAULT);

    _frg_log_increment_counts(severity);

    return (frg_log_result_t) {
        .emitted = true
    };
}

frg_log_result_t frg_log_trace(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        log_path,
        log_lineno,
        NULL,
        0,
        0,
        FRG_LOG_SEVERITY_TRACE,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t _frg_log_debug(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        log_path,
        log_lineno,
        NULL,
        0,
        0,
        FRG_LOG_SEVERITY_DEBUG,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t _frg_log_debug_in_source_file(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* source_path,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        log_path,
        log_lineno,
        source_path,
        0,
        0,
        FRG_LOG_SEVERITY_DEBUG,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t _frg_log_debug_on_source_line(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* source_path,
    frg_lineno_t source_lineno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        log_path,
        log_lineno,
        source_path,
        source_lineno,
        0,
        FRG_LOG_SEVERITY_DEBUG,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t _frg_log_debug_at_source_char(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        log_path,
        log_lineno,
        source_path,
        source_lineno,
        source_columnno,
        FRG_LOG_SEVERITY_DEBUG,
        format,
        args
    );

    va_end(args);

    return result;
}

void frg_log_note(
    const frg_log_result_t* parent_result,
    const char* format,
    ...
) {
    if (parent_result == NULL) {
        fprintf(stderr, "internal logging error: parent_result cannot be NULL\n");
        abort();
    } else if (!parent_result->emitted) {
        return;
    }

    va_list args;
    va_start(args, format);

    _frg_log_helper(
        NULL,
        0,
        NULL,
        0,
        0,
        FRG_LOG_SEVERITY_NOTE,
        format,
        args
    );

    va_end(args);
}

void frg_log_note_in_source_file(
    const frg_log_result_t* parent_result,
    const char* source_path,
    const char* format,
    ...
) {
    if (parent_result == NULL) {
        fprintf(stderr, "internal logging error: parent_result cannot be NULL\n");
        abort();
    } else if (!parent_result->emitted) {
        return;
    }

    va_list args;
    va_start(args, format);

    _frg_log_helper(
        NULL,
        0,
        source_path,
        0,
        0,
        FRG_LOG_SEVERITY_NOTE,
        format,
        args
    );

    va_end(args);
}

void frg_log_note_on_source_line(
    const frg_log_result_t* parent_result,
    const char* source_path,
    frg_lineno_t source_lineno,
    const char* format,
    ...
) {
    if (parent_result == NULL) {
        fprintf(stderr, "internal logging error: parent_result cannot be NULL\n");
        abort();
    } else if (!parent_result->emitted) {
        return;
    }

    va_list args;
    va_start(args, format);

    _frg_log_helper(
        NULL,
        0,
        source_path,
        source_lineno,
        0,
        FRG_LOG_SEVERITY_NOTE,
        format,
        args
    );

    va_end(args);
}

void frg_log_note_at_source_char(
    const frg_log_result_t* parent_result,
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    const char* format,
    ...
) {
    if (parent_result == NULL) {
        fprintf(stderr, "internal logging error: parent_result cannot be NULL\n");
        abort();
    } else if (!parent_result->emitted) {
        return;
    }

    va_list args;
    va_start(args, format);

    _frg_log_helper(
        NULL,
        0,
        source_path,
        source_lineno,
        source_columnno,
        FRG_LOG_SEVERITY_NOTE,
        format,
        args
    );

    va_end(args);
}

frg_log_result_t frg_log_warning(
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        NULL,
        0,
        0,
        FRG_LOG_SEVERITY_WARNING,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_warning_in_source_file(
    const char* source_path,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        source_path,
        0,
        0,
        FRG_LOG_SEVERITY_WARNING,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_warning_on_source_line(
    const char* source_path,
    frg_lineno_t source_lineno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        source_path,
        source_lineno,
        0,
        FRG_LOG_SEVERITY_WARNING,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_warning_at_source_char(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        source_path,
        source_lineno,
        source_columnno,
        FRG_LOG_SEVERITY_WARNING,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_error(
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        NULL,
        0,
        0,
        FRG_LOG_SEVERITY_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_error_in_source_file(
    const char* source_path,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        source_path,
        0,
        0,
        FRG_LOG_SEVERITY_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_error_on_source_line(
    const char* source_path,
    frg_lineno_t source_lineno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        source_path,
        source_lineno,
        0,
        FRG_LOG_SEVERITY_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_error_at_source_char(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        source_path,
        source_lineno,
        source_columnno,
        FRG_LOG_SEVERITY_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_fatal_error(
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        NULL,
        0,
        NULL,
        0,
        0,
        FRG_LOG_SEVERITY_FATAL_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t _frg_log_internal_error(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* format,
    ...
) {
    va_list args;
    va_start(args, format);

    frg_log_result_t result = _frg_log_helper(
        log_path,
        log_lineno,
        NULL,
        0,
        0,
        FRG_LOG_SEVERITY_INTERNAL_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_error_requirement_not_implemented(
    frg_requirement_id_t requirement_id,
    const char* title
) {
    return frg_log_error(
        "%s not yet implemented in Forge %i.%i.%i (Requirement-%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        requirement_id
    );
}

frg_log_result_t frg_log_error_in_source_file_requirement_not_implemented(
    const char* source_path,
    frg_requirement_id_t requirement_id,
    const char* title
) {
    return frg_log_error_in_source_file(
        source_path,
        "%s not yet implemented in Forge %i.%i.%i (Requirement-%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        requirement_id
    );
}

frg_log_result_t frg_log_error_on_source_line_requirement_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_requirement_id_t requirement_id,
    const char* title
) {
    return frg_log_error_on_source_line(
        source_path,
        source_lineno,
        "%s not yet implemented in Forge %i.%i.%i (Requirement-%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        requirement_id
    );
}

frg_log_result_t frg_log_error_at_source_char_requirement_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_requirement_id_t requirement_id,
    const char* title
) {
    return frg_log_error_at_source_char(
        source_path,
        source_lineno,
        source_columnno,
        "%s not yet implemented in Forge %i.%i.%i (Requirement-%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        requirement_id
    );
}

frg_log_result_t frg_log_error_requirement_subitem_not_implemented(
    frg_requirement_id_t requirement_id,
    frg_subitem_id_t subitem_id,
    const char* title
) {
    return frg_log_error(
        "%s not yet implemented in Forge %i.%i.%i (Requirement-%i.%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        requirement_id,
        subitem_id
    );
}

frg_log_result_t frg_log_error_in_source_file_requirement_subitem_not_implemented(
    const char* source_path,
    frg_requirement_id_t requirement_id,
    frg_subitem_id_t subitem_id,
    const char* title
) {
    return frg_log_error_in_source_file(
        source_path,
        "%s not yet implemented in Forge %i.%i.%i (Requirement-%i.%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        requirement_id,
        subitem_id
    );
}

frg_log_result_t frg_log_error_on_source_line_requirement_subitem_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_requirement_id_t requirement_id,
    frg_subitem_id_t subitem_id,
    const char* title
) {
    return frg_log_error_on_source_line(
        source_path,
        source_lineno,
        "%s not yet implemented in Forge %i.%i.%i (Requirement-%i.%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        requirement_id,
        subitem_id
    );
}

frg_log_result_t frg_log_error_at_source_char_requirement_subitem_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_requirement_id_t requirement_id,
    frg_subitem_id_t subitem_id,
    const char* title
) {
    return frg_log_error_at_source_char(
        source_path,
        source_lineno,
        source_columnno,
        "%s not yet implemented in Forge %i.%i.%i (Requirement-%i.%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        requirement_id,
        subitem_id
    );
}

frg_log_result_t frg_log_error_proposal_not_implemented(
    frg_proposal_id_t proposal_id,
    const char* title
) {
    return frg_log_error(
        "%s not yet implemented in Forge %i.%i.%i (Proposal-%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        proposal_id
    );
}

frg_log_result_t frg_log_error_in_source_file_proposal_not_implemented(
    const char* source_path,
    frg_proposal_id_t proposal_id,
    const char* title
) {
    return frg_log_error_in_source_file(
        source_path,
        "%s not yet implemented in Forge %i.%i.%i (Proposal-%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        proposal_id
    );
}

frg_log_result_t frg_log_error_on_source_line_proposal_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_proposal_id_t proposal_id,
    const char* title
) {
    return frg_log_error_on_source_line(
        source_path,
        source_lineno,
        "%s not yet implemented in Forge %i.%i.%i (Proposal-%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        proposal_id
    );
}

frg_log_result_t frg_log_error_at_source_char_proposal_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_proposal_id_t proposal_id,
    const char* title
) {
    return frg_log_error_at_source_char(
        source_path,
        source_lineno,
        source_columnno,
        "%s not yet implemented in Forge %i.%i.%i (Proposal-%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        proposal_id
    );
}

frg_log_result_t frg_log_error_proposal_subitem_not_implemented(
    frg_proposal_id_t proposal_id,
    frg_subitem_id_t subitem_id,
    const char* title
) {
    return frg_log_error(
        "%s not yet implemented in Forge %i.%i.%i (Proposal-%i.%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        proposal_id,
        subitem_id
    );
}

frg_log_result_t frg_log_error_in_source_file_proposal_subitem_not_implemented(
    const char* source_path,
    frg_proposal_id_t proposal_id,
    frg_subitem_id_t subitem_id,
    const char* title
) {
    return frg_log_error_in_source_file(
        source_path,
        "%s not yet implemented in Forge %i.%i.%i (Proposal-%i.%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        proposal_id,
        subitem_id
    );
}

frg_log_result_t frg_log_error_on_source_line_proposal_subitem_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_proposal_id_t proposal_id,
    frg_subitem_id_t subitem_id,
    const char* title
) {
    return frg_log_error_on_source_line(
        source_path,
        source_lineno,
        "%s not yet implemented in Forge %i.%i.%i (Proposal-%i.%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        proposal_id,
        subitem_id
    );
}

frg_log_result_t frg_log_error_at_source_char_proposal_subitem_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_proposal_id_t proposal_id,
    frg_subitem_id_t subitem_id,
    const char* title
) {
    return frg_log_error_at_source_char(
        source_path,
        source_lineno,
        source_columnno,
        "%s not yet implemented in Forge %i.%i.%i (Proposal-%i.%i)",
        title,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        proposal_id,
        subitem_id
    );
}

bool frg_log_summary_if_errors(void) {
    if (_frg_log_count_error > 0 || _frg_log_count_warning > 0) {
        fprintf(FRG_STREAM_DEFAULT, "\n");

        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
        if (_frg_log_count_error > 0) {
            fprintf(FRG_STREAM_DEFAULT, "failed with ");
        } else {
            fprintf(FRG_STREAM_DEFAULT, "compiled with ");
        }
    }

    if (_frg_log_count_error > 0) {
        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RED);
        fprintf(
            FRG_STREAM_DEFAULT,
            "%lu error%s",
            _frg_log_count_error,
            _frg_log_count_error == 1 ? "" : "s"
        );
    }

    if (_frg_log_count_error > 0 && _frg_log_count_warning > 0) {
        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
        fprintf(
            FRG_STREAM_DEFAULT,
            " and "
        );
    }

    if (_frg_log_count_warning > 0) {
        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_YELLOW);
        fprintf(
            FRG_STREAM_DEFAULT,
            "%lu warning%s",
            _frg_log_count_warning,
            _frg_log_count_warning == 1 ? "" : "s"
        );
    }

    if (_frg_log_count_error > 0 || _frg_log_count_warning > 0) {
        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
        fprintf(FRG_STREAM_DEFAULT, ".\n");
        frg_color_set(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
    }

    return _frg_log_count_error > 0;
}
