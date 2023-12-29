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

#pragma once

#include <forge/common/enums.h>
#include <glib.h>

typedef struct {
    frg_color_mode_t color_mode;
    bool debug;
    GString* source_file;
    bool version_short;
} frg_config_t;

frg_status_t frg_config_new_default(frg_config_t** config);
void frg_config_destroy(frg_config_t** config);

frg_status_t frg_config_parse_cli(int* exit_status, frg_config_t* config, int argc, const char** argv);
frg_status_t frg_config_parse_env(frg_config_t* config);

frg_status_t frg_config_log_debug(const frg_config_t* config);
