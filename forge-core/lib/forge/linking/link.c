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

#include <forge/assert.h>
#include <forge/linking/link.h>
#include <forge/memory.h>
#include <forge/messages/codes.h>
#include <forge/spawn.h>

void _frg_linking_free_arguments(GList* arguments) {
  for (GList* arguments_iter = arguments; arguments_iter != NULL;
       arguments_iter        = arguments_iter->next) {
    g_string_free((GString*)arguments_iter->data, TRUE);
  }

  g_list_free(arguments);
}

GList* _frg_linking_lld_create_arguments(frg_linking_mode_t mode,
                                         const char* output_path,
                                         const GList* objects) {
  GList* arguments = NULL;

  if (mode == FRG_LINKING_MODE_SHARED_LIBRARY) {
    arguments = g_list_append(arguments, g_string_new("--shared"));
  }

  arguments = g_list_append(arguments, g_string_new("-o"));

  arguments = g_list_append(arguments, g_string_new(output_path));

  for (const GList* objects_iter = objects; objects_iter != NULL;
       objects_iter              = objects_iter->next) {
    arguments = g_list_append(arguments,
                              g_string_new_len(((GString*)objects_iter->data)->str,
                                               ((GString*)objects_iter->data)->len));
  }

  return arguments;
}

bool _frg_linking_link_lld(frg_message_buffer_t* mut_message_buffer,
                           const frg_linking_configuration_t* configuration,
                           frg_linking_mode_t mode,
                           const char* output_path,
                           const GList* objects) {
  int exit_status    = 0;

  GList* arguments   = _frg_linking_lld_create_arguments(mode, output_path, objects);

  const gchar** argv = frg_spawn_convert_arguments_to_argv(
    frg_linking_linker_configuration_get_path(
      frg_linking_configuration_get_current_linker(
        (frg_linking_configuration_t*)configuration)),
    arguments);

  _frg_linking_free_arguments(arguments);

  GError* error = frg_spawn(mut_message_buffer,
                            &exit_status,
                            NULL,
                            frg_linking_linker_configuration_get_path(
                              frg_linking_configuration_get_current_linker(
                                (frg_linking_configuration_t*)configuration)),
                            argv);

  if (error == NULL) {
    if (exit_status != 0) {
      frg_message_emit_fl_2_exited_nonzero(
        mut_message_buffer,
        exit_status,
        frg_linking_linker_configuration_get_path(
          frg_linking_configuration_get_current_linker(
            (frg_linking_configuration_t*)configuration)),
        argv,
        NULL);

      return false;
    }

    frg_spawn_free_argv(argv);

    return true;
  } else {
    frg_message_emit_fl_1_unable_to_spawn(
      mut_message_buffer,
      error->message,
      frg_linking_linker_configuration_get_path(
        frg_linking_configuration_get_current_linker(
          (frg_linking_configuration_t*)configuration)),
      argv,
      NULL);

    g_error_free(error);
    frg_spawn_free_argv(argv);

    return false;
  }
}

bool frg_link(frg_message_buffer_t* mut_message_buffer,
              const frg_linking_configuration_t* configuration,
              frg_linking_mode_t mode,
              const char* output_path,
              const GList* objects) {
  switch (frg_linking_configuration_get_current_linker_kind(configuration)) {
  case FRG_LINKING_LINKER_KIND_NONE:
    frg_message_emit_fl_3_no_linker(mut_message_buffer);

    return false;
  case FRG_LINKING_LINKER_KIND_LD_LLD:
  case FRG_LINKING_LINKER_KIND_LD64_LLD:
  case FRG_LINKING_LINKER_KIND_LLD_LINK:
    return _frg_linking_link_lld(
      mut_message_buffer, configuration, mode, output_path, objects);
  default:
    frg_die_unexpected_enum_value(
      frg_linking_configuration_get_current_linker_kind(configuration));
  }
}
