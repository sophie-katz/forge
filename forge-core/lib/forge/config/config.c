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
#include <forge/common/error.h>
#include <forge/common/color.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/common/stream.h>
#include <forge/cli/program.h>
#include <forge/config/config.h>
#include <forge/config/cli_program.h>
#include <forge/parsing/parsing.h>

frg_config_t* frg_config_new_default() {
    frg_config_t* config = frg_safe_malloc(sizeof(frg_config_t));

    config->version_short = false;
    config->compile_output_path = NULL;

    return config;
}

void frg_config_destroy(frg_config_t** config) {
    frg_safe_free((void**)config);
}

int frg_config_parse_cli(frg_config_t* config, int argc, const char** argv) {
    frg_assert_pointer_non_null(config);
    frg_assert_int_gt(argc, 0);
    frg_assert_pointer_non_null(argv);

    frg_cli_program_t* program = frg_config_cli_program_new();

    int result = frg_cli_program_parse(
        program,
        argc,
        argv,
        config
    );

    frg_cli_program_destroy(&program);

    return result;
}

frg_recoverable_status_t _frg_parse_env_bool(bool* result, const char* key, const char* text) {
    frg_assert_pointer_non_null(result);
    frg_assert_string_non_empty(key);
    frg_assert_pointer_non_null(text);

    if (strcmp(text, "true") == 0) {
        *result = true;
        return FRG_RECOVERABLE_STATUS_OK;
    } else if (strcmp(text, "false") == 0) {
        *result = false;
        return FRG_RECOVERABLE_STATUS_OK;
    } else {
        frg_log_fatal_error("invalid boolean value for environment variable '%s': %s (expected either 'true' or 'false')", key, text);
        return FRG_RECOVERABLE_STATUS_ERROR_WAS_LOGGED;
    }
}

frg_recoverable_status_t _frg_config_parse_env_debug(frg_config_t* config) {
    frg_assert_pointer_non_null(config);

    const char* debug_text = getenv("FORGE_DEBUG");
    if (debug_text != NULL && *debug_text != 0) {
        bool debug_value = false;
        frg_recoverable_status_t result = _frg_parse_env_bool(&debug_value, "FORGE_DEBUG", debug_text);
        if (result != FRG_RECOVERABLE_STATUS_OK) {
            return result;
        }

        if (debug_value) {
            frg_log_set_minimum_severity(FRG_LOG_SEVERITY_DEBUG);
        }
    }

    return FRG_RECOVERABLE_STATUS_OK;
}

frg_recoverable_status_t _frg_config_parse_env_color_mode(frg_config_t* config) {
    frg_assert_pointer_non_null(config);

    const char* color_mode_text = getenv("FORGE_COLOR_MODE");
    if (color_mode_text != NULL && *color_mode_text != 0) {
        if (strcmp(color_mode_text, "disabled") == 0) {
            frg_color_mode_set(FRG_COLOR_MODE_DISABLED);
        } else if (strcmp(color_mode_text, "auto") == 0) {
            frg_color_mode_set(FRG_COLOR_MODE_AUTO);
        } else if (strcmp(color_mode_text, "enabled") == 0) {
            frg_color_mode_set(FRG_COLOR_MODE_ENABLED);
        } else {
            frg_log_fatal_error("unknown color mode: %s", color_mode_text);
            return FRG_RECOVERABLE_STATUS_ERROR_WAS_LOGGED;
        }
    }

    return FRG_RECOVERABLE_STATUS_OK;
}

frg_recoverable_status_t frg_config_parse_env(frg_config_t* config) {
    frg_assert_pointer_non_null(config);
    
    frg_recoverable_status_t result = _frg_config_parse_env_debug(config);
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        return result;
    }

    result = _frg_config_parse_env_color_mode(config);
    if (result != FRG_RECOVERABLE_STATUS_OK) {
        return result;
    }

    return FRG_RECOVERABLE_STATUS_OK;
}

void frg_config_log_debug(const frg_config_t* config) {
    frg_assert_pointer_non_null(config);

    frg_log_result_t result = frg_log_debug("Configuration: %s");
    frg_log_note(&result, "config.version_short == %s", config->version_short ? "true" : "false");
}
