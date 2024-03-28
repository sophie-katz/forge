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

#include <glib.h>

#define FRG_TESTING_STRING_SUBSTITUTER_MAX_KEY_LENGTH 255

typedef struct {
  GHashTable* _substitutions;
} frg_testing_string_substituter_t;

frg_testing_string_substituter_t* frg_testing_string_substituter_new();

void frg_testing_string_substituter_destroy(
  frg_testing_string_substituter_t* substituter);

void frg_testing_string_substituter_add_str(
  frg_testing_string_substituter_t* mut_substituter,
  const char* key,
  const char* value);

void frg_testing_string_substituter_add_string(
  frg_testing_string_substituter_t* mut_substituter, const char* key, GString* value);

void frg_testing_string_substituter_add_printf(
  frg_testing_string_substituter_t* mut_substituter,
  const char* key,
  const char* format,
  ...);

void frg_testing_string_substituter_add_vprintf(
  frg_testing_string_substituter_t* mut_substituter,
  const char* key,
  const char* format,
  va_list args);

GString* frg_testing_string_substituter_run(
  const frg_testing_string_substituter_t* substituter, const char* input);
