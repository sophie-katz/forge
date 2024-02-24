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

#include <forge/configuration/commands/utilities.h>
#include <forge/messages/codes.h>

const char* frg_configuration_get_single_source_file_from_positional_arguments(
    frg_message_buffer_t* mut_message_buffer,
    const GList* positional_arguments
) {
    if (positional_arguments == NULL) {
        frg_message_emit_fc_4_expects_source_file(
            mut_message_buffer
        );

        return NULL;
    } else if (positional_arguments->next != NULL) {
        frg_message_emit_fc_5_expects_only_one_source_file(
            mut_message_buffer,
            (const char*)positional_arguments->next->data
        );

        return NULL;
    } else {
        return (const char*)positional_arguments->data;
    }
}
