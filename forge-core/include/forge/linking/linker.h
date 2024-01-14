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

#pragma once

#include <forge/common/enums.h>
#include <forge/linking/linker_config.h>
#include <forge/messages/message_buffer.h>
#include <glib.h>

bool frg_link(
    frg_message_buffer_t* message_buffer,
    const frg_linker_config_t* config,
    frg_linker_mode_t mode,
    const char* output_path,
    GList* objects
);
