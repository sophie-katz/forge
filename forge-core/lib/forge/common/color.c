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

#include <forge/common/color.h>
#include <forge/common/stream.h>
#include <unistd.h>

// Implementation is heavily based off of:
// https://github.com/ararslan/termcolor-c/blob/master/include/termcolor-c.h

/// The global color mode
static frg_color_mode_t _frg_color_mode = FRG_COLOR_MODE_AUTO;

/// \brief Resolves a color mode hint.
///
/// Pass the hint in via the `mode` parameter and then it will be resolved in place
/// unless an error is returned.
///
/// \param mode The mode hint to resolve.
///
/// \retval FRG_STATUS_ERROR_NULL_ARGUMENT If either argument is null
/// \retval FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE If \a stream is not \c stdout or
///         \c stderr
frg_status_t _frg_resolve_color_mode_from_hint(frg_color_mode_t* mode) {
    // Error checking
    if (mode == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    // Resolve the color mode if the hint is auto
    if (*mode == FRG_COLOR_MODE_AUTO) {
        // printf("checking if %d is a tty\n", fileno(stream));
        if (isatty(fileno(stdout)) && isatty(fileno(stderr))) {
            *mode = FRG_COLOR_MODE_ENABLED;
        } else {
            *mode = FRG_COLOR_MODE_DISABLED;
        }
    }

    // Return OK status
    return FRG_STATUS_OK;
}

frg_status_t frg_get_color_mode(frg_color_mode_t* mode) {
    frg_status_t result = _frg_resolve_color_mode_from_hint(
        &_frg_color_mode
    );

    if (result != FRG_STATUS_OK) {
        return result;
    }

    *mode = _frg_color_mode;

    return FRG_STATUS_OK;
}

frg_status_t frg_set_color_mode(frg_color_mode_t hint) {
    _frg_color_mode = hint;

    frg_status_t result = _frg_resolve_color_mode_from_hint(
        &_frg_color_mode
    );

    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_set_color(FILE* stream, frg_color_id_t id) {
    frg_status_t result = frg_validate_console_stream(stream);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    frg_color_mode_t mode;
    result = frg_get_color_mode(&mode);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    if (mode == FRG_COLOR_MODE_ENABLED) {
        switch (id) {
            case FRG_COLOR_ID_RESET:
                fprintf(stream, "\033[0;0m");
                break;
            case FRG_COLOR_ID_BOLD:
                fprintf(stream, "\033[1m");
                break;
            case FRG_COLOR_ID_DIM:
                fprintf(stream, "\033[2m");
                break;
            case FRG_COLOR_ID_ITALIC:
                fprintf(stream, "\033[3m");
                break;
            case FRG_COLOR_ID_UNDERLINE:
                fprintf(stream, "\033[4m");
                break;
            case FRG_COLOR_ID_BLACK:
                fprintf(stream, "\033[30m");
                break;
            case FRG_COLOR_ID_RED:
                fprintf(stream, "\033[31m");
                break;
            case FRG_COLOR_ID_GREEN:
                fprintf(stream, "\033[32m");
                break;
            case FRG_COLOR_ID_YELLOW:
                fprintf(stream, "\033[33m");
                break;
            case FRG_COLOR_ID_BLUE:
                fprintf(stream, "\033[34m");
                break;
            case FRG_COLOR_ID_MAGENTA:
                fprintf(stream, "\033[35m");
                break;
            case FRG_COLOR_ID_CYAN:
                fprintf(stream, "\033[36m");
                break;
            case FRG_COLOR_ID_WHITE:
                fprintf(stream, "\033[37m");
                break;
            case FRG_COLOR_ID_BRIGHT_BLACK:
                fprintf(stream, "\033[90m");
                break;
            case FRG_COLOR_ID_BRIGHT_RED:
                fprintf(stream, "\033[91m");
                break;
            case FRG_COLOR_ID_BRIGHT_GREEN:
                fprintf(stream, "\033[92m");
                break;
            case FRG_COLOR_ID_BRIGHT_YELLOW:
                fprintf(stream, "\033[93m");
                break;
            case FRG_COLOR_ID_BRIGHT_BLUE:
                fprintf(stream, "\033[94m");
                break;
            case FRG_COLOR_ID_BRIGHT_MAGENTA:
                fprintf(stream, "\033[95m");
                break;
            case FRG_COLOR_ID_BRIGHT_CYAN:
                fprintf(stream, "\033[96m");
                break;
            case FRG_COLOR_ID_BRIGHT_WHITE:
                fprintf(stream, "\033[97m");
                break;
            default:
                return FRG_STATUS_ERROR_UNEXPECTED_ENUM_VALUE;
        }
    }

    return FRG_STATUS_OK;
}
