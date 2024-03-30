// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#pragma once

#include <forge/enums.h>
#include <forge/linking/configuration.h>
#include <forge/messages/message_buffer.h>
#include <glib.h>

bool frg_link(frg_message_buffer_t* mut_message_buffer,
              const frg_linking_configuration_t* configuration,
              frg_linking_mode_t mode,
              const char* output_path,
              const GList* objects);
