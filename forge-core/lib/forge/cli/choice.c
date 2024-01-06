// Copyright (c) 2024 Sophie Katz
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

#include <forge/common/error.h>
#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/cli/choice.h>

frg_cli_choice_t* frg_cli_choice_new(
    const char* name,
    const char* help
) {
    frg_assert_string_non_empty(name);
    frg_assert_string_non_empty(help);

    frg_cli_choice_t* choice = frg_safe_malloc(
        sizeof(frg_cli_choice_t)
    );

    choice->name = name;
    choice->help = help;

    return choice;
}

void frg_cli_choice_destroy(
    frg_cli_choice_t** choice
) {
    frg_safe_free((void**)choice);
}

void frg_cli_choice_print_help(
    const frg_cli_choice_t* choice
) {
    frg_assert_pointer_non_null(choice);

    if (frg_color_mode_get() == FRG_COLOR_MODE_ENABLED) {
        printf("      • ");
    } else {
        printf("      - ");
    }

    frg_color_set(stdout, FRG_COLOR_ID_BOLD);
    printf("%s", choice->name);
    frg_color_set(stdout, FRG_COLOR_ID_RESET);
    printf(" (%s)\n", choice->help);
}
