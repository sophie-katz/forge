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
#include <forge/common/memory.h>
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

int frg_config_parse_cli(
    frg_message_buffer_t* message_buffer,
    frg_config_t* config,
    int argc,
    const char** argv
) {
    frg_assert_pointer_non_null(config);
    frg_assert_int_gt(argc, 0);
    frg_assert_pointer_non_null(argv);

    frg_cli_program_t* program = frg_config_cli_program_new();

    int result = frg_cli_program_parse(
        message_buffer,
        program,
        argc,
        argv,
        config
    );

    frg_cli_program_destroy(&program);

    return result;
}

bool _frg_parse_env_bool(
    frg_message_buffer_t* message_buffer,
    bool* result,
    const char* key,
    const char* text
) {
    frg_assert_pointer_non_null(result);
    frg_assert_string_non_empty(key);
    frg_assert_pointer_non_null(text);

    if (strcmp(text, "true") == 0) {
        *result = true;
        return true;
    } else if (strcmp(text, "false") == 0) {
        *result = false;
        return true;
    } else {
        frg_message_emit(
            message_buffer,
            FRG_MESSAGE_SEVERITY_FATAL_ERROR,
            "invalid boolean value for environment variable '%s': %s (expected either 'true' or 'false')",
            key,
            text
        );

        return false;
    }
}

bool _frg_config_parse_env_debug(
    frg_message_buffer_t* message_buffer,
    frg_config_t* config
) {
    frg_assert_pointer_non_null(config);

    const char* debug_text = g_getenv("FORGE_DEBUG");
    if (debug_text != NULL && *debug_text != 0) {
        bool debug_value = false;
        if (!_frg_parse_env_bool(
            message_buffer,
            &debug_value,
            "FORGE_DEBUG",
            debug_text
        )) {
            return false;
        }

        if (debug_value) {
            config->minimum_message_severity = FRG_MESSAGE_SEVERITY_DEBUG;
        }
    }

    return true;
}

bool _frg_config_parse_env_color_mode(
    frg_message_buffer_t* message_buffer,
    frg_config_t* config
) {
    frg_assert_pointer_non_null(config);

    const char* color_mode_text = g_getenv("FORGE_COLOR_MODE");
    if (color_mode_text != NULL && *color_mode_text != 0) {
        if (strcmp(color_mode_text, "disabled") == 0) {
            frg_stream_output_set_console_color(false);
        } else if (strcmp(color_mode_text, "auto") == 0) {
            // Do nothing
        } else if (strcmp(color_mode_text, "enabled") == 0) {
            frg_stream_output_set_console_color(true);
        } else {
            frg_message_emit(
                message_buffer,
                FRG_MESSAGE_SEVERITY_FATAL_ERROR,
                "unknown color mode: %s",
                color_mode_text
            );

            return false;
        }
    }

    return true;
}

bool _frg_config_parse_env_unicode_mode(
    frg_message_buffer_t* message_buffer,
    frg_config_t* config
) {
    frg_assert_pointer_non_null(config);

    const char* unicode_mode_text = g_getenv("FORGE_UNICODE_MODE");
    if (unicode_mode_text != NULL && *unicode_mode_text != 0) {
        if (strcmp(unicode_mode_text, "disabled") == 0) {
            frg_stream_output_set_console_unicode(false);
        } else if (strcmp(unicode_mode_text, "auto") == 0) {
            // Do nothing
        } else if (strcmp(unicode_mode_text, "enabled") == 0) {
            frg_stream_output_set_console_unicode(true);
        } else {
            frg_message_emit(
                message_buffer,
                FRG_MESSAGE_SEVERITY_FATAL_ERROR,
                "unknown unicode mode: %s",
                unicode_mode_text
            );

            return false;
        }
    }

    return true;
}

bool frg_config_parse_env(
    frg_message_buffer_t* message_buffer,
    frg_config_t* config
) {
    frg_assert_pointer_non_null(config);
    
    if (!_frg_config_parse_env_debug(
        message_buffer,
        config
    )) {
        return false;
    }

    if (!_frg_config_parse_env_color_mode(
        message_buffer,
        config
    )) {
        return false;
    }

    if (!_frg_config_parse_env_unicode_mode(
        message_buffer,
        config
    )) {
        return false;
    }

    return true;
}

void frg_config_log_debug(
    frg_message_buffer_t* message_buffer,
    const frg_config_t* config
) {
    frg_assert_pointer_non_null(config);

    frg_message_t* message = frg_message_emit(
        message_buffer,
        FRG_MESSAGE_SEVERITY_DEBUG,
        "Configuration:"
    );

    frg_message_emit_child(
        message_buffer,
        message,
        FRG_MESSAGE_SEVERITY_NOTE,
        "config.version_short == %s",
        config->version_short ? "true" : "false"
    );

    frg_message_emit_child(
        message_buffer,
        message,
        FRG_MESSAGE_SEVERITY_NOTE,
        "config.compile_output_path == \"%s\"",
        config->compile_output_path
    );

    frg_message_emit_child(
        message_buffer,
        message,
        FRG_MESSAGE_SEVERITY_NOTE,
        "config.minimum_message_severity == %i",
        config->minimum_message_severity
    );
}
