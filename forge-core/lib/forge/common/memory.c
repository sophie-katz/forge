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

#include <forge/common/memory.h>
#include <forge/common/error.h>
#include <stdlib.h>
#include <stdio.h>

void* frg_safe_malloc(size_t size) {
    void* ptr = malloc(size);

    if (ptr == NULL) {
        // We avoid using the error system here, because it may try to allocate memory.
        fputs("internal error: out of memory\n", stderr);
        abort();
    }

    return ptr;
}

void frg_safe_free(void** ptr) {
    frg_assert_pointer_non_null(ptr);
    frg_assert_pointer_non_null(*ptr);

    free(*ptr);
    *ptr = NULL;
}
