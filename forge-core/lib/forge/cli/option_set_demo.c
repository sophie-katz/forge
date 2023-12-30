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

#include <forge/cli/option_set.h>
#include <forge/common/log.h>

frg_status_t callback(void* user_data, const char* value) {
    return FRG_STATUS_OK;
}

int main(void) {
    frg_cli_option_set_t* option_set = NULL;
    frg_status_t result = frg_cli_option_set_new(
        &option_set
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create option set: %s", frg_status_to_string(result));
        return 1;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(
        &option,
        "long-name-1",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_flag_short(
        &option,
        'a',
        "long-name-2",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_argument(
        &option,
        "long-name-3",
        "value",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_argument_short(
        &option,
        'b',
        "long-name-4",
        "value",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_add_option(option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_choice(
        &option,
        "long-name-5",
        "value",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    frg_cli_choice_t* choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-1",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create choice: %s", frg_status_to_string(result));
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
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create choice: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );

    result = frg_cli_option_set_add_option(option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    option = NULL;
    result = frg_cli_option_new_choice_short(
        &option,
        'c',
        "long-name-6",
        "value",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create option: %s", frg_status_to_string(result));
        return 1;
    }

    choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-1",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create choice: %s", frg_status_to_string(result));
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
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create choice: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );

    result = frg_cli_option_set_add_option(option_set, option);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add option to option set: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_print_help(
        option_set
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to print option set help: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_option_set_destroy(&option_set);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable destroy option set: %s", frg_status_to_string(result));
        return 1;
    }

    return 0;
}
