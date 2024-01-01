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

#include <forge/config/commands/common.h>
#include <forge/common/log.h>

frg_status_t frg_config_commands_get_single_source_file(
    const char** path,
    GList* pos_args
) {
    if (path == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*path != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (pos_args == NULL) {
        frg_log_fatal_error("command compile expects a source file as an argument");
        return FRG_STATUS_CLI_ERROR;
    } else if (pos_args->next != NULL) {
        frg_log_fatal_error("command compile expects only one source file as an argument");
        return FRG_STATUS_CLI_ERROR;
    } else {
        *path = (const char*)pos_args->data;
        return FRG_STATUS_OK;
    }
}
