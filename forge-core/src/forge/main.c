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

#include <forge/common/log.h>
#include <forge/config/cli.h>
#include <forge/config/config.h>
#include <build_config.h>

frg_status_t _frg_cli_callback_option_color_mode(void* user_data, const char* value) {
    frg_config_t* config = (frg_config_t*)user_data;

    if (strcmp(value, "auto") == 0) {
        config->color_mode = FRG_COLOR_MODE_AUTO;
    } else if (strcmp(value, "enabled") == 0) {
        config->color_mode = FRG_COLOR_MODE_ENABLED;
    } else if (strcmp(value, "disabled") == 0) {
        config->color_mode = FRG_COLOR_MODE_DISABLED;
    } else {
        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "invalid color mode: %s", value);
        frg_log(FRG_LOG_SEVERITY_NOTE, "see help to see valid color modes");
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_option_color_mode(frg_cli_option_t** option) {
    GList* choices = NULL;
    frg_cli_option_choice_t* choice = NULL;

    frg_status_t result = frg_cli_option_choice_new(&choice, "auto", "Automatically detect whether to use color or not");
    if (result != FRG_STATUS_OK) {
        return result;
    }
    choices = g_list_append(choices, choice);

    choice = NULL;
    result = frg_cli_option_choice_new(&choice, "enabled", "Forces the use of color");
    if (result != FRG_STATUS_OK) {
        return result;
    }
    choices = g_list_append(choices, choice);

    choice = NULL;
    result = frg_cli_option_choice_new(&choice, "disabled", "Disables the ues of color");
    if (result != FRG_STATUS_OK) {
        return result;
    }
    choices = g_list_append(choices, choice);

    return frg_cli_option_new_choice(
        option,
        "color-mode",
        "mode",
        "Changes whether color is used or not for console output.",
        choices,
        _frg_cli_callback_option_color_mode
    );
}

frg_status_t _frg_cli_callback_option_debug(void* user_data, const char* value) {
    frg_config_t* config = (frg_config_t*)user_data;

    config->debug = true;

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_option_debug(frg_cli_option_t** option) {
    return frg_cli_option_new_flag(
        option,
        "debug",
        "Enables debug logging output.",
        _frg_cli_callback_option_debug
    );
}

frg_status_t _frg_cli_option_set_global(frg_cli_option_set_t* option_set) {
    frg_cli_option_t* option_color_mode = NULL;
    frg_status_t result = _frg_cli_option_color_mode(&option_color_mode);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_set_add(option_set, option_color_mode);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    frg_cli_option_t* option_debug = NULL;
    result = _frg_cli_option_debug(&option_debug);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_option_set_add(option_set, option_debug);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_callback_command_version(int* exit_status, void* user_data, GList* pos_args) {
    if (pos_args != NULL) {
        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "version command does not take any positional arguments");
        *exit_status = 1;
        return FRG_STATUS_OK;
    }

    printf("version\n");

    *exit_status = 0;

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_command_version(frg_cli_command_t** command) {
    return frg_cli_command_new(
        command,
        "version",
        "Prints the current version.",
        _frg_cli_callback_command_version
    );
}

frg_status_t _frg_cli_program(frg_cli_program_t** program) {
    GString* version_details = g_string_new(NULL);
    g_string_append_printf(version_details, "Git commit: %s\n", FRG_GIT_COMMIT_SHA);
    g_string_append_printf(version_details, "Compiled with: %s %s\n", FRG_COMPILER_ID, FRG_COMPILER_VERSION);
    g_string_append_printf(version_details, "Linked with: %s\n", FRG_LINKER_ID);
    g_string_append_printf(version_details, "Built for: %s (%s)", FRG_HOST_MACHINE_SYSTEM, FRG_HOST_MACHINE_CPU);

    frg_status_t result = frg_cli_program_new(
        program,
        "Forge",
        "forge",
        NULL,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        FRG_VERSION_LABEL,
        version_details
    );
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = _frg_cli_option_set_global((*program)->global_options);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    frg_cli_command_t* command_version = NULL;
    result = _frg_cli_command_version(&command_version);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    result = frg_cli_program_add_command(*program, command_version);
    if (result != FRG_STATUS_OK) {
        return result;
    }

    return FRG_STATUS_OK;
}

int main(int argc, char *argv[]) {
    frg_cli_program_t* program = NULL;
    frg_status_t result = _frg_cli_program(&program);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI program: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_print_help(program, "version");
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to print CLI program help: %s", frg_status_to_string(result));
        return 1;
    }

    result = frg_cli_program_destroy(&program);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to destroy CLI program: %s", frg_status_to_string(result));
        return 1;
    }

    return 0;
}
