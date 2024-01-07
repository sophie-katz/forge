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

#include <forge/parsing/domain.h>

typedef struct {
    frg_parsing_location_t start;
    const char* text;
    frg_parsing_location_t current_location;
    const char* current_char;
} frg_parsing_token_reader_t;

frg_parsing_token_reader_t* frg_parsing_token_reader_new(frg_parsing_location_t start, const char* text);
void frg_parsing_token_reader_destroy(frg_parsing_token_reader_t** reader);

const frg_parsing_location_t* frg_parsing_token_reader_get_current_location(const frg_parsing_token_reader_t* reader);
char frg_parsing_token_reader_get_current_char(const frg_parsing_token_reader_t* reader);

void frg_parsing_token_reader_step(frg_parsing_token_reader_t* reader);

frg_parsing_token_reader_t* frg_parsing_token_reader_save(const frg_parsing_token_reader_t* reader);
void frg_parsing_token_reader_restore(frg_parsing_token_reader_t* reader, frg_parsing_token_reader_t** state);
