#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <forge/log.h>
#include <forge/color.h>
#include <forge/stream.h>

static size_t _frg_log_count_error = 0;
static size_t _frg_log_count_warning = 0;

frg_status_t _frg_log_prefix(frg_log_severity_t severity) {
    switch (severity) {
        case FRG_LOG_SEVERITY_NOTE:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "note: ");
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
            return FRG_STATUS_OK;
        case FRG_LOG_SEVERITY_DEBUG:
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_MAGENTA);
            frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
            fprintf(FRG_STREAM_DEFAULT, "debug: ");
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

frg_status_t frg_log_prefix_source_file(const char* filename) {
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

frg_status_t frg_log_prefix_source_line(const char* filename, frg_lineno_t lineno) {
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

frg_status_t frg_log_prefix_source_char(
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

void _frg_log_prefix_internal(const char* filename, frg_lineno_t lineno) {
    const char* filename_resolved = strstr(filename, "lib/");

    if (filename_resolved == NULL) {
        filename_resolved = strstr(filename, "src/");
    }

    if (filename_resolved == NULL) {
        filename_resolved = filename;
    }

    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BRIGHT_BLACK);
    fprintf(FRG_STREAM_DEFAULT, "forge-compiler[%s:%d]: ", filename_resolved, lineno);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);
}

frg_status_t frg_log(frg_log_severity_t severity, const char* format, ...) {
    // Start variadic arguments
    va_list args;
    va_start(args, format);

    // Log prefix
    frg_status_t result = _frg_log_prefix(severity);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    // Log message
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_BOLD);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_WHITE);
    vfprintf(FRG_STREAM_DEFAULT, format, args);
    frg_set_color(FRG_STREAM_DEFAULT, FRG_COLOR_ID_RESET);

    // Log suffix
    _frg_log_suffix();

    // Flush output
    fflush(FRG_STREAM_DEFAULT);

    // End variadic arguments
    va_end(args);

    // Increment log counts
    _frg_log_increment_counts(severity);

    return FRG_STATUS_OK;
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
