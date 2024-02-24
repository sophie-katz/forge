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

#include <forge/cli/option.h>
#include <forge/enums.h>
#include <forge/messages/message_buffer.h>
#include <forge/streams/output.h>
#include <glib.h>

/// \brief The maximum number of option short names possible.
///
/// There are 26 for lowercase ASCII letters and 26 more for uppercase.
#define FRG_CLI_OPTION_SET_SHORT_NAME_COUNT_MAX 52

typedef struct {
  GList* _options;
  frg_cli_option_t*
    _options_index_by_short_name[FRG_CLI_OPTION_SET_SHORT_NAME_COUNT_MAX];
  GHashTable* _options_index_by_long_name;
} frg_cli_option_set_t;

frg_cli_option_set_t* frg_cli_option_set_new();

void frg_cli_option_set_destroy(frg_cli_option_set_t* option_set);

bool frg_cli_option_set_is_empty(const frg_cli_option_set_t* option_set);

void frg_cli_option_set_add_option(frg_cli_option_set_t* mut_option_set,
                                   frg_cli_option_t* option);

frg_cli_option_t* frg_cli_option_set_get_option_by_long_name(
  frg_cli_option_set_t* mut_option_set, const char* name);

frg_cli_option_t* frg_cli_option_set_get_option_by_short_name(
  frg_cli_option_set_t* mut_option_set, char name);

void frg_cli_option_set_print_help(frg_stream_output_t* mut_stream,
                                   const frg_cli_option_set_t* option_set);

bool frg_cli_option_set_parse_next(frg_message_buffer_t* mut_message_buffer,
                                   int* mut_argi,
                                   void* mut_user_data,
                                   const frg_cli_option_set_t* option_set,
                                   int argc,
                                   const char** argv);
