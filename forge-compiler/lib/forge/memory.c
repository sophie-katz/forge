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

#include <forge/assert.h>
#include <forge/memory.h>
#include <stdio.h>
#include <stdlib.h>

void* frg_malloc(size_t size) {
  void* ptr = malloc(size);

  if (ptr == NULL) {
    frg_die("out of memory");
  }

  return ptr;
}

void frg_free(void* ptr) {
  frg_assert_pointer_non_null(ptr);

  free(ptr);
}
