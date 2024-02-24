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

#pragma once

#include <forge/enums.h>

typedef struct {
    GString* _path;
} frg_linking_linker_configuration_t;

const char* frg_linking_linker_configuration_get_path(
    const frg_linking_linker_configuration_t* configuration
);

void frg_linking_linker_configuration_set_path(
    frg_linking_linker_configuration_t* mut_configuration,
    const char* path
);

typedef struct {
    frg_linking_linker_kind_t _current_linker_kind;
    union {
        frg_linking_linker_configuration_t _for_ld_lld;
        frg_linking_linker_configuration_t _for_ld64_lld;
        frg_linking_linker_configuration_t _for_lld_link;
    } _linker_configuration;
} frg_linking_configuration_t;

frg_linking_configuration_t* frg_linking_configuration_detect();

void frg_linking_configuration_destroy(frg_linking_configuration_t* configuration);

frg_linking_linker_kind_t frg_linking_configuration_get_current_linker_kind(
    const frg_linking_configuration_t* configuration
);

void frg_linking_configuration_set_current_linker_kind(
    frg_linking_configuration_t* mut_configuration,
    frg_linking_linker_kind_t linker_kind
);

frg_linking_linker_configuration_t* frg_linking_configuration_get_current_linker(
    frg_linking_configuration_t* mut_configuration
);
