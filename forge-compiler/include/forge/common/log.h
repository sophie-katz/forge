#pragma once

#include <forge/common/enums.h>
#include <forge/common/types.h>
#include <stdbool.h>

/// \brief Log a message prefix that indicates that the following message is from the
///        source file \a filename.
frg_status_t frg_log_prefix_source_file(const char* filename);

/// \brief Log a message prefix that indicates that the following message is from the
///        source file \a filename on line \a lineno.
frg_status_t frg_log_prefix_source_line(const char* filename, frg_lineno_t lineno);

/// \brief Log a message prefix that indicates that the following message is from the
///        source file \a filename on line \a lineno at column \a columnno.
frg_status_t frg_log_prefix_source_char(
    const char* filename,
    frg_lineno_t lineno,
    frg_columnno_t columnno
);

/// \brief Log a message prefix that indicates that the following message is from the
///        current line of the Forge compiler source code.
#define frg_log_prefix_internal() _frg_log_prefix_internal(__FILE__, __LINE__)

void _frg_log_prefix_internal(const char* filename, frg_lineno_t lineno);

/// \brief Log a message.
frg_status_t frg_log(frg_log_severity_t severity, const char* format, ...);

bool frg_log_summary_if_errors(void);
