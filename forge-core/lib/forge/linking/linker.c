// Copyright (c) 2023-2024 Sophie Katz
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

#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/linking/linker.h>
#include <forge/messages/codes.h>

/// \brief Converts a list of \c GString objects into a static array.
///
/// The strings are freed as they are converted, but the original memory segments
/// contained in them are passed along into the returned array. It is the responsibility
/// of the caller to free this array and the character arrays within it.
///
/// The input list is also freed.
gchar** _frg_spawn_convert_arguments_to_argv(
    const char* executable,
    GList** arguments
) {
    frg_assert_string_non_empty(executable);

    gchar** argv = frg_safe_malloc(sizeof(gchar*) * ((g_list_length(*arguments) + 2)));

    GList* arguments_iter = *arguments;
    gchar** argv_iter = argv;

    *argv_iter = g_strdup(executable);
    argv_iter++;

    while (arguments_iter != NULL) {
        *argv_iter = ((GString*)arguments_iter->data)->str;
        g_string_free((GString*)arguments_iter->data, FALSE);

        arguments_iter = arguments_iter->next;
        argv_iter++;
    }

    *argv_iter = NULL;

    g_list_free(*arguments);
    *arguments = NULL;

    return argv;
}

void _frg_spawn_free_argv(gchar*** argv) {
    frg_assert_pointer_non_null(argv);
    frg_assert_pointer_non_null(*argv);

    gchar** argv_iter = *argv;

    while (*argv_iter != NULL) {
        g_free((gpointer)*argv_iter);
        argv_iter++;
    }

    frg_safe_free((void**)argv);
}

bool _frg_spawn(
    frg_message_buffer_t* message_buffer,
    int* exit_status,
    const char* working_directory,
    const char* executable,
    GList* arguments
) {
    gchar** argv = _frg_spawn_convert_arguments_to_argv(
        executable,
        &arguments
    );

    *exit_status = 0;
    GError* error = NULL;

    if (g_spawn_sync(
        working_directory,
        argv,
        NULL,
        G_SPAWN_DEFAULT,
        NULL,
        NULL,
        NULL,
        NULL,
        exit_status,
        &error
    )) {
        frg_assert_pointer_null(error);

        _frg_spawn_free_argv(&argv);

        return true;
    } else {
        frg_assert_pointer_non_null(error);

        frg_message_emit_fl_1_unable_to_spawn(
            message_buffer,
            error->message,
            executable,
            argv,
            working_directory
        );

        g_error_free(error);

        _frg_spawn_free_argv(&argv);

        return false;
    }
}

GList* _frg_link_lld_create_arguments(
    frg_linker_mode_t mode,
    const char* output_path,
    GList* objects
) {
    GList* arguments = NULL;

    if (mode == FRG_LINKER_MODE_SHARED_LIBRARY) {
        arguments = g_list_append(
            arguments,
            g_string_new("--shared")
        );
    }

    arguments = g_list_append(
        arguments,
        g_string_new("-o")
    );

    arguments = g_list_append(
        arguments,
        g_string_new(output_path)
    );

    for (GList* objects_iter = objects; objects_iter != NULL; objects_iter = objects_iter->next) {
        arguments = g_list_append(
            arguments,
            g_string_new(((GString*)objects_iter->data)->str)
        );
    }

    return arguments;
}

bool _frg_link_lld(
    frg_message_buffer_t* message_buffer,
    const frg_linker_config_t* config,
    frg_linker_mode_t mode,
    const char* output_path,
    GList* objects
) {
    int exit_status = 0;

    GList* arguments = _frg_link_lld_create_arguments(
        mode,
        output_path,
        objects
    );

    if (_frg_spawn(
        message_buffer,
        &exit_status,
        NULL,
        frg_linker_config_get_linker_path(
            config
        ),
        arguments
    )) {
        if (exit_status != 0) {
            gchar** argv = _frg_spawn_convert_arguments_to_argv(
                frg_linker_config_get_linker_path(
                    config
                ),
                &arguments
            );

            frg_message_emit_fl_2_exited_nonzero(
                message_buffer,
                exit_status,
                frg_linker_config_get_linker_path(
                    config
                ),
                argv,
                NULL
            );

            _frg_spawn_free_argv(&argv);

            return false;
        }

        return true;
    } else {
        return false;
    }
}

bool frg_link(
    frg_message_buffer_t* message_buffer,
    const frg_linker_config_t* config,
    frg_linker_mode_t mode,
    const char* output_path,
    GList* objects
) {
    switch (config->linker_id) {
        case FRG_LINKER_ID_NONE:
            frg_message_emit_fl_3_no_linker(message_buffer);

            return false;
        case FRG_LINKER_ID_LD_LLD:
        case FRG_LINKER_ID_LD64_LLD:
        case FRG_LINKER_ID_LLD_LINK:
            return _frg_link_lld(
                message_buffer,
                config,
                mode,
                output_path,
                objects
            );
        default:
            frg_die_unexpected_enum_value(config->linker_id);
    }
}
