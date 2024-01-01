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

#include <forge/common/check.h>
#include <forge/common/log.h>

bool _frg_check(
    const char* log_path,
    frg_lineno_t log_lineno,
    frg_status_t status
) {
    if (status == FRG_STATUS_OK) {
        return true;
    }

    frg_log_trace(
        log_path,
        log_lineno,
        "check failed (%s)",
        frg_status_to_string(status)
    );

    return false;
}
