// Copyright (c) 2023 Sophie Katz
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
