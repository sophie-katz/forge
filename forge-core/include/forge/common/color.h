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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/// Gets the current global color mode.
///
/// \param mode An output parameter for the current color mode.
frg_status_t frg_get_color_mode(frg_color_mode_t* mode);

/// \brief Sets the color mode based on \a hint.
///
/// If \a hint is \c FRG_COLOR_MODE_AUTO it will auto-detect. Otherwise it will set the
/// mode to \a hint exactly.
frg_status_t frg_set_color_mode(frg_color_mode_t hint);

/// \brief Sets the color for \a stream to \a id.
///
/// \param stream The output stream for which to set the color. Can be either \c stdout
///               or \c stderr
/// \param id The color to set
///
/// \retval FRG_STATUS_ERROR_NULL_ARGUMENT If \a stream is \c NULL
/// \retval FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE If \a stream is not \c stdout or
///         \c stderr
/// \retval FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE If \a id is unexpected
frg_status_t frg_set_color(FILE* stream, frg_color_id_t id);
