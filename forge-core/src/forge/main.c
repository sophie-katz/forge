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
#include <build_config.h>

int main(int argc, char *argv[]) {
    // frg_parsing_source_context_t* source_context = frg_parsing_source_context_new();

    // frg_message_buffer_t* message_buffer = frg_message_buffer_new();

    // frg_config_t* config = frg_config_new_default();

    // frg_recoverable_status_t result = frg_config_parse_env(
    //     message_buffer,
    //     config
    // );
    // if (result != FRG_RECOVERABLE_STATUS_OK) {
    //     return 1;
    // }

    // int exit_status = frg_config_parse_cli(
    //     message_buffer,
    //     config,
    //     argc,
    //     (const char**)argv
    // );
    // if (exit_status != 0) {
    //     return exit_status;
    // }

    // frg_message_buffer_print(
    //     stderr,
    //     message_buffer,
    //     source_context,
    //     config->minimum_message_severity,
    //     5 // TODO: Make this configurable
    // );

    // frg_message_buffer_destroy(&message_buffer);

    // frg_parsing_source_context_destroy(&source_context);

    return 0;
}
