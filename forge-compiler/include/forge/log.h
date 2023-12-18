#pragma once

#include <forge/types.h>

typedef uint8_t frg_log_severity_t;

#define FRG_LOG_SEVERITY_NOTE 0
#define FRG_LOG_SEVERITY_DEBUG 1
#define FRG_LOG_SEVERITY_WARNING 2
#define FRG_LOG_SEVERITY_ERROR 3
#define FRG_LOG_SEVERITY_FATAL_ERROR 4
#define FRG_LOG_SEVERITY_INTERNAL_ERROR 5

void frg_log(frg_log_severity_t severity, const char* format, ...);
frg_size_t frg_log_get_count_error(void);
frg_size_t frg_log_get_count_warning(void);
