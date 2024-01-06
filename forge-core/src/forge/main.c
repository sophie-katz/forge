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
#include <build_config.h>

int main(int argc, char *argv[]) {
    frg_config_t* config = frg_config_new_default();

    frg_recoverable_status_t result = frg_config_parse_env(config);
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        return 1;
    }

    int exit_status = frg_config_parse_cli(config, argc, (const char**)argv);
    if (exit_status != 0) {
        return exit_status;
    }

    if (frg_log_summary_if_errors()) {
        return 1;
    }

    return 0;
}
