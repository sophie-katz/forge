#include <stdio.h>
#include <stdarg.h>
#include <forge/log.h>
#include <forge/color.h>

static size_t _frg_log_count_error = 0;
static size_t _frg_log_count_warning = 0;

void _frg_log_prefix(frg_log_severity_t severity) {
    switch (severity) {
        case FRG_LOG_SEVERITY_NOTE:
            frg_set_color(stderr, FRG_COLOR_CODE_NOTE);
            fprintf(stderr, "note: ");
            frg_set_color(stderr, FRG_COLOR_CODE_RESET);
            break;
        case FRG_LOG_SEVERITY_DEBUG:
            frg_set_color(stderr, FRG_COLOR_CODE_DEBUG);
            fprintf(stderr, "debug: ");
            frg_set_color(stderr, FRG_COLOR_CODE_RESET);
            break;
        case FRG_LOG_SEVERITY_WARNING:
            frg_set_color(stderr, FRG_COLOR_CODE_WARNING);
            fprintf(stderr, "warning: ");
            frg_set_color(stderr, FRG_COLOR_CODE_RESET);
            break;
        case FRG_LOG_SEVERITY_ERROR:
            frg_set_color(stderr, FRG_COLOR_CODE_ERROR);
            fprintf(stderr, "error: ");
            frg_set_color(stderr, FRG_COLOR_CODE_RESET);
            break;
        case FRG_LOG_SEVERITY_FATAL_ERROR:
            frg_set_color(stderr, FRG_COLOR_CODE_FATAL_ERROR);
            fprintf(stderr, "fatal error: ");
            frg_set_color(stderr, FRG_COLOR_CODE_RESET);
            break;
        case FRG_LOG_SEVERITY_INTERNAL_ERROR:
            frg_set_color(stderr, FRG_COLOR_CODE_INTERNAL_ERROR);
            fprintf(stderr, "internal error: ");
            frg_set_color(stderr, FRG_COLOR_CODE_RESET);
            break;
        default:
            fprintf(stderr, "internal error: unknown severity level %d\n", severity);
            break;
    }
}

void _frg_log_suffix(void) {
    fprintf(stderr, "\n");
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

void frg_log_location(const char* filename, size_t line, size_t column) {
    frg_set_color(stderr, FRG_COLOR_CODE_LOCATION);
    fprintf(stderr, "%s:%lu:%lu: ", filename, line, column);
    frg_set_color(stderr, FRG_COLOR_CODE_RESET);
}

void frg_log(frg_log_severity_t severity, const char* format, ...) {
    // Start variadic arguments
    va_list args;
    va_start(args, format);

    // Log prefix
    _frg_log_prefix(severity);

    // Log message
    frg_set_color(stderr, FRG_COLOR_CODE_MESSAGE);
    vfprintf(stderr, format, args);
    frg_set_color(stderr, FRG_COLOR_CODE_RESET);

    // Log suffix
    _frg_log_suffix();

    // Flush output
    fflush(stderr);

    // End variadic arguments
    va_end(args);

    // Increment log counts
    _frg_log_increment_counts(severity);
}

size_t frg_log_get_count_error(void) {
    return _frg_log_count_error;
}

size_t frg_log_get_count_warning(void) {
    return _frg_log_count_warning;
}
