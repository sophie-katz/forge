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

int callback_command(
    frg_message_buffer_t* message_buffer,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    return 0;
}

bool callback_option(frg_message_buffer_t* message_buffer, void* user_data, const char* value) {
    return true;
}

int main(void) {
    frg_cli_command_t* command = frg_cli_command_new(
        "command",
        NULL,
        "A description of the command",
        callback_command
    );

    frg_cli_command_print_help(command);

    frg_cli_command_destroy(&command);

    return 0;
}
