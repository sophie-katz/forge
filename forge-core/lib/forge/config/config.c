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

#include <build_config.h>
#include <forge/ast/ast.h>
#include <forge/ast/debug.h>
#include <forge/codegen/codegen.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/common/stream.h>
#include <forge/cli/program.h>
#include <forge/config/config.h>
#include <forge/config/cli_program.h>
#include <forge/parse/parse.h>

frg_status_t frg_config_new_default(frg_config_t** config) {
    frg_status_t result = frg_safe_malloc((void**)config, sizeof(frg_config_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*config)->source_file = NULL;
    (*config)->version_short = false;

    return FRG_STATUS_OK;
}

void frg_config_destroy(frg_config_t** config) {
    if (*config == NULL) {
        return;
    }

    g_string_free((*config)->source_file, true);

    frg_safe_free((void**)config);
}

frg_status_t frg_config_parse_cli(int* exit_status, frg_config_t* config, int argc, const char** argv) {
    frg_cli_program_t* program = NULL;
    frg_status_t result = frg_config_cli_program_new(&program);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI program: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_parse(
        exit_status,
        program,
        argc,
        argv,
        config
    );
    if (result == FRG_STATUS_CLI_ERROR) {
        return FRG_STATUS_CLI_ERROR;
    } else if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal(FRG_LOG_SEVERITY_INTERNAL_ERROR);
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to parse args with CLI program: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_config_log_debug(const frg_config_t* config) {
    if (config == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_log(FRG_LOG_SEVERITY_DEBUG, "Configuration:");
    frg_log(FRG_LOG_SEVERITY_NOTE, "config.source_file == %s", config->source_file != NULL ? config->source_file->str : "(null)");
    frg_log(FRG_LOG_SEVERITY_NOTE, "config.version_short == %s", config->version_short ? "true" : "false");

    return FRG_STATUS_OK;
}
