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

#include <forge/config/cli/choice.h>
#include <forge/common/log.h>

int main(void) {
    frg_cli_choice_t* choice = NULL;
    frg_status_t result = frg_cli_choice_new(
        &choice,
        "option-name",
        "A description of the choice"
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create choice: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_choice_print_help(choice);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to print choice help: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_choice_destroy(&choice);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to destroy choice: %s", frg_status_to_string(result));
        return 1;
    }

    return 0;
}
