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

#include <forge/config/cli_program.h>
#include <forge/config/config.h>
#include <build_config.h>
#include <forge/common/error.h>
#include <forge/config/commands/compile.h>
#include <forge/config/commands/dump_ast.h>
#include <forge/config/commands/dump_ir.h>
#include <forge/config/commands/help.h>
#include <forge/config/commands/link.h>
#include <forge/config/commands/version.h>

void frg_config_cli_program_banner(void) {
    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_ID_BOLD);
    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_YELLOW);
    frg_stream_output_write_printf(frg_stream_output_get_stdout(), "\n       \\ | / ,\n");
    frg_stream_output_write_printf(frg_stream_output_get_stdout(), "     `\n");
    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_ID_BRIGHT_RED);
    frg_stream_output_write_printf(frg_stream_output_get_stdout(), "---------------\n");
    frg_stream_output_write_printf(frg_stream_output_get_stdout(), "  ----.      /\n");
    frg_stream_output_write_printf(frg_stream_output_get_stdout(), "      /___^__\\\n\n");
    frg_stream_output_set_color(frg_stream_output_get_stdout(), FRG_STREAM_OUTPUT_COLOR_ID_RESET);
}

int _frg_config_cli_program_callback(
    frg_message_buffer_t* message_buffer,
    const frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    frg_config_cli_program_banner();

    if (!frg_cli_program_try_print_help(
        frg_stream_output_get_stdout(),
        message_buffer,
        program,
        NULL
    )) {
        return 1;
    }

    return 0;
}

bool _frg_config_cli_option_callback_debug(
    frg_message_buffer_t* message_buffer,
    void* user_data,
    const char* value
) {
    frg_config_t* config = (frg_config_t*)user_data;

    config->minimum_message_severity = FRG_MESSAGE_SEVERITY_DEBUG;

    return true;
}

bool _frg_config_cli_option_callback_color_mode(
    frg_message_buffer_t* message_buffer,
    void* user_data,
    const char* value
) {
    if (strcmp(value, "disabled") == 0) {
        frg_stream_output_set_console_color(false);
    } else if (strcmp(value, "auto") == 0) {
        // Do nothing
    } else if (strcmp(value, "enabled") == 0) {
        frg_stream_output_set_console_color(true);
    } else {
        frg_message_emit(
            message_buffer,
            FRG_MESSAGE_SEVERITY_FATAL_ERROR,
            "unknown color mode: %s",
            value
        );

        return false;
    }

    return true;
}

bool _frg_config_cli_option_callback_unicode_mode(
    frg_message_buffer_t* message_buffer,
    void* user_data,
    const char* value
) {
    if (strcmp(value, "disabled") == 0) {
        frg_stream_output_set_console_unicode(false);
    } else if (strcmp(value, "auto") == 0) {
        // Do nothing
    } else if (strcmp(value, "enabled") == 0) {
        frg_stream_output_set_console_unicode(true);
    } else {
        frg_message_emit(
            message_buffer,
            FRG_MESSAGE_SEVERITY_FATAL_ERROR,
            "unknown unicode mode: %s",
            value
        );

        return false;
    }

    return true;
}

frg_cli_program_t* frg_config_cli_program_new() {
    GString* version_details = g_string_new(NULL);
    g_string_append_printf(version_details, "Git commit: %s\n", FRG_GIT_COMMIT_SHA);
    g_string_append_printf(version_details, "Compiled with: %s %s\n", FRG_COMPILER_ID, FRG_COMPILER_VERSION);
    g_string_append_printf(version_details, "Linked with: %s\n", FRG_LINKER_ID);
    g_string_append_printf(version_details, "Built for: %s (%s)\n", FRG_HOST_MACHINE_SYSTEM, FRG_HOST_MACHINE_CPU);
    g_string_append_printf(version_details, "Using LLVM %i.%i.%i", FRG_LLVM_VERSION_MAJOR, FRG_LLVM_VERSION_MINOR, FRG_LLVM_VERSION_PATCH);

    frg_cli_program_t* program = frg_cli_program_new(
        "Forge",
        "forge",
        NULL,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        FRG_VERSION_LABEL,
        version_details,
        _frg_config_cli_program_callback
    );

    frg_cli_option_t* option = frg_cli_option_new_choice(
        "color-mode",
        "mode",
        "Set the color mode",
        _frg_config_cli_option_callback_color_mode
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "disabled",
            "Disable color output"
        )
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "auto",
            "Automatically detect whether or not to use color output"
        )
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "enabled",
            "Force color output to be enabled"
        )
    );

    frg_cli_option_set_add_option(
        program->global_options,
        option
    );

    frg_cli_option_set_add_option(
        program->global_options,
        frg_cli_option_new_flag(
            "debug",
            "Enable debug logging",
            _frg_config_cli_option_callback_debug
        )
    );

    option = frg_cli_option_new_choice(
        "unicode-mode",
        "mode",
        "Set the unicode mode",
        _frg_config_cli_option_callback_unicode_mode
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "disabled",
            "Disable unicode characters and use ASCII instead"
        )
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "auto",
            "Automatically detect whether or not to use unicode characters"
        )
    );

    frg_cli_option_add_choice(
        option,
        frg_cli_choice_new(
            "enabled",
            "Force unicode characters to be used when available"
        )
    );

    frg_cli_option_set_add_option(
        program->global_options,
        option
    );

    frg_cli_program_add_command(
        program,
        frg_config_commands_new_compile()
    );

    frg_cli_program_add_command(
        program,
        frg_config_commands_new_dump_ast()
    );

    frg_cli_program_add_command(
        program,
        frg_config_commands_new_dump_ir()
    );

    frg_cli_program_add_command(
        program,
        frg_config_commands_new_help()
    );

    frg_cli_program_add_command(
        program,
        frg_config_commands_new_link()
    );

    frg_cli_program_add_command(
        program,
        frg_config_commands_new_version()
    );

    return program;
}
