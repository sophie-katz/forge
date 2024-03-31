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

#include <forge/streams/output.h>

extern bool frg_global_log_enabled;

frg_stream_output_t* frg_log_stream_get();

bool _frg_log_event(
  const char* file, int line_number, const char* function, const char* format, ...);

#define frg_log_event(format, ...)                                                     \
  _frg_log_event(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

void frg_log_property(const char* name, const char* format, ...);
