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

#include <forge/parsing/source.h>
#include <glib.h>

typedef struct {
  GHashTable* _sources;
} frg_parsing_source_context_t;

frg_parsing_source_context_t* frg_parsing_source_context_new();

void frg_parsing_source_context_destroy(frg_parsing_source_context_t* source_context);

void frg_parsing_source_context_add_source(
  frg_parsing_source_context_t* mut_source_context, frg_parsing_source_t* source);

frg_parsing_source_t* frg_parsing_source_context_get_source_by_path(
  frg_parsing_source_context_t* mut_source_context, const char* path);
