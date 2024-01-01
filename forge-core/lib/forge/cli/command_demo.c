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

#include <forge/cli/command.h>
#include <forge/common/log.h>

frg_status_t callback_command(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    return FRG_STATUS_OK;
}

frg_status_t callback_option(void* user_data, const char* value) {
    return FRG_STATUS_OK;
}

int main(void) {
    frg_cli_command_t* command = NULL;
    frg_status_t result = frg_cli_command_new(
        &command,
        "command",
        NULL,
        "A description of the command",
        callback_command
    );

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(
        &option,
        "long-name-1",
        "A description of the option",
        callback_option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(command->option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_flag_short(
        &option,
        'a',
        "long-name-2",
        "A description of the option",
        callback_option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(command->option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_argument(
        &option,
        "long-name-3",
        "value",
        "A description of the option",
        callback_option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(command->option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_argument_short(
        &option,
        'b',
        "long-name-4",
        "value",
        "A description of the option",
        callback_option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(command->option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_choice(
        &option,
        "long-name-5",
        "value",
        "A description of the option",
        callback_option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    frg_cli_choice_t* choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-1",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create choice: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );

    choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-2",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create choice: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );

    result = frg_cli_option_set_add_option(command->option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_choice_short(
        &option,
        'c',
        "long-name-6",
        "value",
        "A description of the option",
        callback_option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-1",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create choice: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );

    choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-2",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create choice: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );

    result = frg_cli_option_set_add_option(command->option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_command_print_help(
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print command set help: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_command_destroy(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable destroy command set: %s", frg_status_to_string(result));
        return 1;
    }

    return 0;
}
