// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright 2023-2024 Sophie Katz

#include <forge/assert.h>
#include <forge/memory.h>
#include <forge/spawn.h>

const gchar** frg_spawn_convert_arguments_to_argv(const char* executable,
                                                  const GList* arguments) {
  frg_assert_string_non_empty(executable);

  const gchar** argv
    = frg_malloc(sizeof(gchar*) * ((g_list_length((GList*)arguments) + 2)));

  const GList* arguments_iter = arguments;
  const gchar** argv_iter     = argv;

  *argv_iter                  = g_strdup(executable);
  argv_iter++;

  while (arguments_iter != NULL) {
    *argv_iter     = g_strdup(((const GString*)arguments_iter->data)->str);

    arguments_iter = arguments_iter->next;
    argv_iter++;
  }

  *argv_iter = NULL;

  return argv;
}

void frg_spawn_free_argv(const gchar** argv) {
  frg_assert_pointer_non_null(argv);

  const gchar** argv_iter = argv;

  while (*argv_iter != NULL) {
    g_free((gpointer)*argv_iter);
    argv_iter++;
  }

  frg_free(argv);
}

GError* frg_spawn(frg_message_buffer_t* mut_message_buffer,
                  int* exit_status,
                  const char* working_directory,
                  const char* executable,
                  const gchar** argv) {
  *exit_status  = 0;
  GError* error = NULL;

  if (g_spawn_sync(working_directory,
                   (gchar**)argv,
                   NULL,
                   G_SPAWN_DEFAULT,
                   NULL,
                   NULL,
                   NULL,
                   NULL,
                   exit_status,
                   &error)) {
    frg_assert_pointer_null(error);

    return NULL;
  } else {
    frg_assert_pointer_non_null(error);

    return error;
  }
}
