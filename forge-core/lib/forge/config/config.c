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

#include <build_config.h>
#include <forge/ast/ast.h>
#include <forge/ast/debug.h>
#include <forge/codegen/codegen.h>
#include <forge/common/check.h>
#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/common/stream.h>
#include <forge/cli/program.h>
#include <forge/config/config.h>
#include <forge/config/cli_program.h>
#include <forge/parse/parse.h>

frg_status_t frg_config_new_default(frg_config_t** config) {
    frg_check(
        frg_safe_malloc((void**)config, sizeof(frg_config_t))
    );

    (*config)->version_short = false;
    (*config)->compile_output_path = NULL;

    return FRG_STATUS_OK;
}

frg_status_t frg_config_destroy(frg_config_t** config) {
    frg_check(
        frg_safe_free((void**)config)
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_config_parse_cli(int* exit_status, frg_config_t* config, int argc, const char** argv) {
    frg_cli_program_t* program = NULL;
    frg_check(
        frg_config_cli_program_new(&program)
    );

    frg_status_t result = frg_cli_program_parse(
        exit_status,
        program,
        argc,
        argv,
        config
    );
    if (result == FRG_STATUS_CLI_ERROR) {
        return FRG_STATUS_CLI_ERROR;
    } else {
        frg_check(result);
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_parse_env_bool(bool* result, const char* text) {
    if (strcmp(text, "true") == 0) {
        *result = true;
    } else if (strcmp(text, "false") == 0) {
        *result = false;
    } else {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_config_parse_env_debug(frg_config_t* config) {
    if (config == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    const char* debug_text = getenv("FORGE_DEBUG");
    if (debug_text != NULL && *debug_text != 0) {
        bool debug_value = false;
        frg_status_t result = _frg_parse_env_bool(&debug_value, debug_text);
        if (result != FRG_STATUS_OK) {
            frg_log_fatal_error("invalid value for FORGE_DEBUG: %s (expected either 'true' or 'false')");
            return FRG_STATUS_CLI_ERROR;
        }

        if (debug_value) {
            frg_check(
                frg_log_set_minimum_severity(FRG_LOG_SEVERITY_DEBUG)
            );
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_config_parse_env_color_mode(frg_config_t* config) {
    if (config == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    const char* color_mode_text = getenv("FORGE_COLOR_MODE");
    if (color_mode_text != NULL && *color_mode_text != 0) {
        if (strcmp(color_mode_text, "disabled") == 0) {
            frg_check(
                frg_set_color_mode(FRG_COLOR_MODE_DISABLED)
            );
        } else if (strcmp(color_mode_text, "auto") == 0) {
            frg_check(
                frg_set_color_mode(FRG_COLOR_MODE_AUTO)
            );
        } else if (strcmp(color_mode_text, "enabled") == 0) {
            frg_check(
                frg_set_color_mode(FRG_COLOR_MODE_ENABLED)
            );
        } else {
            frg_log_fatal_error("unknown color mode: %s", color_mode_text);
            return FRG_STATUS_CLI_ERROR;
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_config_parse_env(frg_config_t* config) {
    frg_check(
        _frg_config_parse_env_debug(config)
    );

    frg_check(
        _frg_config_parse_env_color_mode(config)
    );

    return FRG_STATUS_OK;
}

frg_status_t frg_config_log_debug(const frg_config_t* config) {
    if (config == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_log_result_t result = frg_log_debug("Configuration: %s");
    frg_log_note(&result, "config.version_short == %s", config->version_short ? "true" : "false");

    return FRG_STATUS_OK;
}
