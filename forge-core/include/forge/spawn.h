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

#include <forge/messages/message_buffer.h>

const gchar** frg_spawn_convert_arguments_to_argv(const char* executable,
                                                  const GList* arguments);

void frg_spawn_free_argv(const gchar** argv);

GError* frg_spawn(frg_message_buffer_t* mut_message_buffer,
                  int* exit_status,
                  const char* working_directory,
                  const char* executable,
                  const gchar** argv);
