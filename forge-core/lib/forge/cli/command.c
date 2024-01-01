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

#include <forge/common/check.h>
#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/cli/command.h>

frg_status_t frg_cli_command_new(
    frg_cli_command_t** command,
    const char* name,
    const char* pos_args_name,
    const char* help,
    frg_cli_command_callback_t callback
) {
    if (command == NULL || name == NULL || help == NULL || callback == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*name == 0 || *help == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (*command != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_check(
        frg_safe_malloc((void**)command, sizeof(frg_cli_command_t))
    );

    (*command)->name = name;
    (*command)->pos_args_name = pos_args_name;
    (*command)->help = help;
    (*command)->option_set = NULL;
    (*command)->callback = callback;

    frg_check(
        frg_cli_option_set_new(
            &(*command)->option_set
        )
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_command_destroy(
    frg_cli_command_t** command
) {
    if (command == NULL || *command == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_check(
        frg_cli_option_set_destroy(
            &(*command)->option_set
        )
    );

    return frg_safe_free((void**)command);
}

frg_status_t frg_cli_command_print_help(
    const frg_cli_command_t* command
) {
    printf("%s\n", command->help);

    return FRG_STATUS_OK;
}
