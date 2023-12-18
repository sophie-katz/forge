#include <stdio.h>
#include <stdarg.h>
#include <forge/log.h>

static frg_size_t _frg_log_count_error = 0;
static frg_size_t _frg_log_count_warning = 0;

void _frg_log_prefix(frg_log_severity_t severity) {
    switch (severity) {
        case FRG_LOG_SEVERITY_NOTE:
            fprintf(stderr, "note: ");
            break;
        case FRG_LOG_SEVERITY_DEBUG:
            fprintf(stderr, "debug: ");
            break;
        case FRG_LOG_SEVERITY_WARNING:
            fprintf(stderr, "warning: ");
            break;
        case FRG_LOG_SEVERITY_ERROR:
            fprintf(stderr, "error: ");
            break;
        case FRG_LOG_SEVERITY_FATAL_ERROR:
            fprintf(stderr, "fatal error: ");
            break;
        case FRG_LOG_SEVERITY_INTERNAL_ERROR:
            fprintf(stderr, "internal error: ");
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

void frg_log(frg_log_severity_t severity, const char* format, ...) {
    // Start variadic arguments
    va_list args;
    va_start(args, format);

    // Log prefix
    _frg_log_prefix(severity);

    // Log message
    vfprintf(stderr, format, args);

    // Log suffix
    _frg_log_suffix();

    // Flush output
    fflush(stderr);

    // End variadic arguments
    va_end(args);

    // Increment log counts
    _frg_log_increment_counts(severity);
}

frg_size_t frg_log_get_count_error(void) {
    return _frg_log_count_error;
}

frg_size_t frg_log_get_count_warning(void) {
    return _frg_log_count_warning;
}
