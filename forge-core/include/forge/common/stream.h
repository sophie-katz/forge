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
#include <stdio.h>

#define FRG_STREAM_DEFAULT stderr

/// Validates that \a stream is a console stream.
///
/// \retval FRG_STATUS_ERROR_NULL_ARGUMENT If \a stream is \c NULL
/// \retval FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE If \a stream is not \c stdout or
///         \c stderr
frg_status_t frg_validate_console_stream(FILE* stream);
