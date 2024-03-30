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

#include <forge/messages/query.h>

const frg_message_query_t frg_message_query_empty
  = { .with_severity = FRG_MESSAGE_SEVERITY_DEBUG,
      .with_code     = NULL,
      .with_text     = NULL,
      .on_line       = 0,
      .from_path     = NULL };
