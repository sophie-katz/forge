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

#include <forge/parsing/domain.h>

const frg_parsing_range_t frg_global_parsing_range_null = {
  .start  = {.path = NULL, .offset = 0, .line_number = 0, .column_number = 0},
  .length = 0
};

frg_parsing_range_t frg_parsing_range_get_span(const frg_parsing_range_t* first,
                                               const frg_parsing_range_t* last) {
  frg_parsing_range_t range
    = { .start  = first->start,
        .length = last->start.offset + last->length - first->start.offset };

  return range;
}
