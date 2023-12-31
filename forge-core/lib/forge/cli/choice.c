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
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/cli/choice.h>

frg_status_t frg_cli_choice_new(
    frg_cli_choice_t** choice,
    const char* name,
    const char* help
) {
    if (choice == NULL || name == NULL || help == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*choice != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (*name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_status_t result = frg_safe_malloc(
        (void**)choice,
        sizeof(frg_cli_choice_t)
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*choice)->name = name;
    (*choice)->help = help;

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_choice_destroy(
    frg_cli_choice_t** choice
) {
    return frg_safe_free((void**)choice);
}

frg_status_t frg_cli_choice_print_help(
    const frg_cli_choice_t* choice
) {
    if (choice == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_color_mode_t mode;
    frg_status_t result = frg_get_color_mode(&mode);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to get color mode: %s", frg_status_to_string(result)); 
        return result;
    }

    if (mode == FRG_COLOR_MODE_ENABLED) {
        printf("      • ");
    } else {
        printf("      - ");
    }

    frg_set_color(stdout, FRG_COLOR_ID_BOLD);
    printf("%s", choice->name);
    frg_set_color(stdout, FRG_COLOR_ID_RESET);
    printf(" (%s)\n", choice->help);

    return FRG_STATUS_OK;
}
