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

#include <forge/ast/ast.h>
#include <forge/config/commands/link.h>
#include <forge/config/config.h>
#include <forge/common/error.h>
#include <forge/common/log.h>

int _frg_config_commands_callback_link(
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    frg_assert_pointer_non_null(program);

    frg_die("not yet implemented");
}

frg_cli_command_t* frg_config_commands_new_link() {
    return frg_cli_command_new(
        "link",
        "object files",
        "Links object files into executables or libraries.",
        _frg_config_commands_callback_link
    );
}
