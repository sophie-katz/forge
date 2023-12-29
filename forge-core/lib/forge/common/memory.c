// Copyright (c) 2023 Sophie Katz
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
#include <stdlib.h>

frg_status_t frg_safe_malloc(void** ptr, size_t size) {
    if (ptr == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (size == 0) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    *ptr = malloc(size);

    if (*ptr == NULL) {
        return FRG_STATUS_ERROR_OUT_OF_MEMORY;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_safe_free(void** ptr) {
    if (ptr == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    if (*ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }

    return FRG_STATUS_OK;
}
