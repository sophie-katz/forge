// Copyright (c) 2023-2024 Sophie Katz
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

#include <forge/config/commands/common.h>
#include <forge/messages/codes.h>

const char* frg_config_commands_get_single_source_file(
    frg_message_buffer_t* message_buffer,
    GList* pos_args
) {
    if (pos_args == NULL) {
        frg_message_emit_fc_4_expects_source_file(
            message_buffer
        );

        return NULL;
    } else if (pos_args->next != NULL) {
        frg_message_emit_fc_5_expects_only_one_source_file(
            message_buffer,
            (const char*)pos_args->next->data
        );

        return NULL;
    } else {
        return (const char*)pos_args->data;
    }
}
