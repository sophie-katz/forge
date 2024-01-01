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

#include <forge/cli/option.h>
#include <forge/common/log.h>

frg_status_t callback(void* user_data, const char* value) {
    return FRG_STATUS_OK;
}

void flag(void) {
    frg_cli_option_t* option = NULL;
    frg_status_t result = frg_cli_option_new_flag(
        &option,
        "long-name",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_print_help(option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print option help: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_destroy(&option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy option: %s", frg_status_to_string(result));
    }
}

void flag_short(void) {
    frg_cli_option_t* option = NULL;
    frg_status_t result = frg_cli_option_new_flag_short(
        &option,
        'l',
        "long-name",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_print_help(option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print option help: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_destroy(&option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy option: %s", frg_status_to_string(result));
    }
}

void argument(void) {
    frg_cli_option_t* option = NULL;
    frg_status_t result = frg_cli_option_new_argument(
        &option,
        "long-name",
        "value",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_print_help(option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print option help: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_destroy(&option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy option: %s", frg_status_to_string(result));
    }
}

void argument_short(void) {
    frg_cli_option_t* option = NULL;
    frg_status_t result = frg_cli_option_new_argument_short(
        &option,
        'l',
        "long-name",
        "value",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_print_help(option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print option help: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_destroy(&option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy option: %s", frg_status_to_string(result));
    }
}

void choice(void) {
    frg_cli_option_t* option = NULL;
    frg_status_t result = frg_cli_option_new_choice(
        &option,
        "long-name",
        "value",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
    }

    frg_cli_choice_t* choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-1",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create choice: %s", frg_status_to_string(result));
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
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );

    result = frg_cli_option_print_help(option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print option help: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_destroy(&option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy option: %s", frg_status_to_string(result));
    }
}

void choice_short(void) {
    frg_cli_option_t* option = NULL;
    frg_status_t result = frg_cli_option_new_choice_short(
        &option,
        'l',
        "long-name",
        "value",
        "A description of the option",
        callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create option: %s", frg_status_to_string(result));
    }

    frg_cli_choice_t* choice = NULL;
    result = frg_cli_choice_new(
        &choice,
        "option-1",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create choice: %s", frg_status_to_string(result));
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
    }

    result = frg_cli_option_add_choice(
        option,
        choice
    );

    result = frg_cli_option_print_help(option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to print option help: %s", frg_status_to_string(result));
    }

    result = frg_cli_option_destroy(&option);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to destroy option: %s", frg_status_to_string(result));
    }
}

int main(void) {
    flag();
    printf("\n");
    flag_short();
    printf("\n");
    argument();
    printf("\n");
    argument_short();
    printf("\n");
    choice();
    printf("\n");
    choice_short();

    return 0;
}
