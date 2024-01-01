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

#include <forge/config/config.h>
#include <forge/common/log.h>

int main(int argc, char *argv[]) {
    frg_config_t* config = NULL;
    frg_status_t result = frg_config_new_default(&config);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to create config: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_config_parse_env(config);
    if (result == FRG_STATUS_CLI_ERROR) {
        return 1;
    } else if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to parse config from environment variables: %s", frg_status_to_string(result));
        return 1;
    }

    int exit_status = 0;
    result = frg_config_parse_cli(&exit_status, config, argc, (const char**)argv);
    if (result == FRG_STATUS_CLI_ERROR) {
        return 1;
    } else if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to parse config from CLI: %s", frg_status_to_string(result));
        return 1;
    } else if (exit_status != 0) {
        return exit_status;
    }

    if (frg_log_summary_if_errors()) {
        return 1;
    }

    return 0;
}
