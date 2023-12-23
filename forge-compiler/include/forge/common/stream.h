#pragma once

#include <forge/common/enums.h>
#include <stdio.h>

#define FRG_STREAM_DEFAULT stderr

/// Validates that \a stream is a console stream.
///
/// \retval FRG_STATUS_ERROR_NULL_ARGUMENT If \a stream is \c NULL
/// \retval FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE If \a stream is not \c stdout or
///         \c stderr
frg_status_t frg_validate_console_stream(FILE* stream);
