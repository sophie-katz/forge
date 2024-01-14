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
#include <forge/messages/message_buffer.h>
#include <glib.h>

typedef struct {
    bool version_short;
    const char* compile_output_path;
    frg_message_severity_t minimum_message_severity;
} frg_config_t;

frg_config_t* frg_config_new_default();
void frg_config_destroy(frg_config_t** config);

int frg_config_parse_cli(
    frg_message_buffer_t* message_buffer,
    frg_config_t* config,
    int argc,
    const char** argv
);

bool frg_config_parse_env(
    frg_message_buffer_t* message_buffer,
    frg_config_t* config
);

void frg_config_log_debug(
    frg_message_buffer_t* message_buffer,
    const frg_config_t* config
);
