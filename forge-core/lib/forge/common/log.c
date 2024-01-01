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
#include <unistd.h>

static frg_log_severity_t _frg_log_minimum_severity = FRG_LOG_SEVERITY_NOTE;
static size_t _frg_log_count_error = 0;
static size_t _frg_log_count_warning = 0;

frg_status_t frg_log_set_minimum_severity(frg_log_severity_t severity) {
    if (severity > FRG_LOG_SEVERITY_NOTE) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    _frg_log_minimum_severity = severity;

    return FRG_STATUS_OK;
}

frg_status_t _frg_log_prefix_severity(frg_log_severity_t severity) {
    switch (severity) {
        case FRG_LOG_SEVERITY_TRACE:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "trace: ");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            return FRG_STATUS_OK;
        case FRG_LOG_SEVERITY_DEBUG:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_MAGENTA);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "debug: ");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            return FRG_STATUS_OK;
        case FRG_LOG_SEVERITY_NOTE:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "note: ");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            return FRG_STATUS_OK;
        case FRG_LOG_SEVERITY_WARNING:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_YELLOW);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "warning: ");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            return FRG_STATUS_OK;
        case FRG_LOG_SEVERITY_ERROR:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RED);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "error: ");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            return FRG_STATUS_OK;
        case FRG_LOG_SEVERITY_FATAL_ERROR:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_RED);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_UNDERLINE);
            fprintf(FRG_STREAM_DEFAULT, "fatal error");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_RED);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, ": ");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            return FRG_STATUS_OK;
        case FRG_LOG_SEVERITY_INTERNAL_ERROR:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_CYAN);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "internal error: ");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            return FRG_STATUS_OK;
        default:
            return FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE;
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

frg_status_t _frg_log_prefix_source_file(const char* filename) {
    if (filename == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (filename[0] == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "%s: ", filename);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

    return FRG_STATUS_OK;
}

frg_status_t _frg_log_prefix_source_line(const char* filename, frg_lineno_t lineno) {
    if (filename == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (filename[0] == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (lineno <= 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "%s:%d: ", filename, lineno);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

    return FRG_STATUS_OK;
}

frg_status_t _frg_log_prefix_source_char(
    const char* filename,
    frg_lineno_t lineno,
    frg_columnno_t columnno
) {
    if (filename == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (filename[0] == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (lineno <= 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (columnno <= 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "%s:%d:%d: ", filename, lineno, columnno);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

    return FRG_STATUS_OK;
}

frg_status_t _frg_log_prefix_internal(
    const char* log_path,
    frg_lineno_t log_lineno
) {
    if (log_path == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (log_path[0] == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (log_lineno <= 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    const char* log_path_resolved = strstr(log_path, "lib/");

    if (log_path_resolved == NULL) {
        log_path_resolved = strstr(log_path, "src/");
    }

    if (log_path_resolved == NULL) {
        log_path_resolved = log_path;
    }

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "forge-core[%s:%d]: ", log_path_resolved, log_lineno);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

    return FRG_STATUS_OK;
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
            _exit(1);
        } else if (*log_path == 0) {
            fprintf(stderr, "internal logging error: log_path cannot be empty\n");
            _exit(1);
        } else if (log_lineno <= 0) {
            fprintf(stderr, "internal logging error: log_lineno must be positive\n");
            _exit(1);
        }
    }

    if (format == NULL) {
        fprintf(stderr, "internal logging error: format cannot be NULL\n");
        _exit(1);
    } else if (*format == 0) {
        fprintf(stderr, "internal logging error: format cannot be empty\n");
        _exit(1);
    } else if (_frg_log_minimum_severity > severity) {
        return (frg_log_result_t) {
            .emitted = false
        };
    }

    if (has_internal_prefix) {
        frg_status_t result = _frg_log_prefix_internal(
            log_path,
            log_lineno
        );
        if (result != FRG_STATUS_OK) {
            fprintf(
                stderr,
                "internal logging error: unable to print internal log prefix: %s\n",
                frg_status_to_string(result)
            );
            _exit(1);
        }
    }

    if (source_path != NULL) {
        if (*source_path == 0) {
            fprintf(stderr, "internal logging error: source_path cannot be empty\n");
            _exit(1);
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

    frg_status_t result = _frg_log_prefix_severity(severity);
    if (result != FRG_STATUS_OK) {
        fprintf(
            stderr,
            "internal logging error: unable to print log severity prefix: %s\n",
            frg_status_to_string(result)
        );
        _exit(1);
    }

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
    vfprintf(FRG_STREAM_DEFAULT, format, args);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

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
        _exit(1);
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
        _exit(1);
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
        _exit(1);
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
        _exit(1);
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

frg_log_result_t frg_log_fatal_error_in_source_file(
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
        FRG_LOG_SEVERITY_FATAL_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t frg_log_fatal_error_on_source_line(
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

frg_log_result_t _frg_log_internal_error_in_source_file(
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
        FRG_LOG_SEVERITY_INTERNAL_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t _frg_log_internal_error_on_source_line(
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
        FRG_LOG_SEVERITY_INTERNAL_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

frg_log_result_t _frg_log_internal_error_at_source_char(
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
        FRG_LOG_SEVERITY_INTERNAL_ERROR,
        format,
        args
    );

    va_end(args);

    return result;
}

bool frg_log_summary_if_errors(void) {
    if (_frg_log_count_error > 0 || _frg_log_count_warning > 0) {
        fprintf(FRG_STREAM_DEFAULT, "\n");

        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
        if (_frg_log_count_error > 0) {
            fprintf(FRG_STREAM_DEFAULT, "failed with ");
        } else {
            fprintf(FRG_STREAM_DEFAULT, "compiled with ");
        }
    }

    if (_frg_log_count_error > 0) {
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RED);
        fprintf(
            FRG_STREAM_DEFAULT,
            "%lu error%s",
            _frg_log_count_error,
            _frg_log_count_error == 1 ? "" : "s"
        );
    }

    if (_frg_log_count_error > 0 && _frg_log_count_warning > 0) {
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
        fprintf(
            FRG_STREAM_DEFAULT,
            " and "
        );
    }

    if (_frg_log_count_warning > 0) {
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_YELLOW);
        fprintf(
            FRG_STREAM_DEFAULT,
            "%lu warning%s",
            _frg_log_count_warning,
            _frg_log_count_warning == 1 ? "" : "s"
        );
    }

    if (_frg_log_count_error > 0 || _frg_log_count_warning > 0) {
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
        fprintf(FRG_STREAM_DEFAULT, ".\n");
        frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
    }

    return _frg_log_count_error > 0;
}
