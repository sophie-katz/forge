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

#include <dlfcn.h>
#include <forge-testing/compilation_test/phases/linking.h>
#include <forge-testing/compilation_test/utilities.h>
#include <forge-testing/string_comparison.h>
#include <forge/assert.h>
#include <forge/linking/link.h>
#include <forge/memory.h>
#include <unity.h>

frg_testing_compilation_test_context_linked_t*
  frg_testing_compilation_test_context_linked_new(void* shared_library_handle) {
  frg_assert_pointer_non_null(shared_library_handle);

  frg_testing_compilation_test_context_linked_t* context_linked
    = (frg_testing_compilation_test_context_linked_t*)frg_malloc(
      sizeof(frg_testing_compilation_test_context_linked_t));

  context_linked->shared_library_handle = shared_library_handle;

  return context_linked;
}

void frg_testing_compilation_test_context_linked_destroy(
  frg_testing_compilation_test_context_linked_t* context_linked) {
  frg_assert_pointer_non_null(context_linked);

  if (context_linked->shared_library_handle != NULL) {
    dlclose(context_linked->shared_library_handle);
  }

  frg_free(context_linked);
}

gchar* _frg_testing_test_compilation_phase_linking_format_path(
  const frg_testing_compilation_test_options_t* options, const gchar* extension) {
  frg_assert_pointer_non_null(options);
  frg_assert_string_non_empty(options->name);
  frg_assert_string_non_empty(extension);

  // Get current directory
  gchar* current_dir = g_get_current_dir();

  // Build object path
  GString* filename  = g_string_new(NULL);
  g_string_printf(filename, "test-compilation-%s.%s", options->name, extension);

  // Join path
  gchar* path = g_build_filename(current_dir, filename->str, NULL);

  // Free temporary strings
  g_string_free(filename, TRUE);
  g_free(current_dir);

  // Return path
  return path;
}

gchar* _frg_testing_test_compilation_phase_linking_format_object_path(
  const frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);
  return _frg_testing_test_compilation_phase_linking_format_path(options, "o");
}

gchar* _frg_testing_test_compilation_phase_linking_format_shared_object_path(
  const frg_testing_compilation_test_options_t* options) {
  frg_assert_pointer_non_null(options);
  return _frg_testing_test_compilation_phase_linking_format_path(options, "so");
}

void* _frg_testing_test_compilation_phase_linking_link(
  GString** out_message,
  const frg_testing_compilation_test_context_initialized_t* context_initialized,
  const gchar* object_path,
  const gchar* shared_object_path,
  const frg_linking_configuration_t* linking_configuration) {
  frg_assert_pointer_non_null(context_initialized);
  frg_assert_string_non_empty(object_path);
  frg_assert_string_non_empty(shared_object_path);

  GString* object_path_string = g_string_new(object_path);
  GList* objects              = g_list_append(NULL, object_path_string);

  bool link_result            = frg_link(context_initialized->message_buffer,
                              linking_configuration,
                              FRG_LINKING_MODE_SHARED_LIBRARY,
                              shared_object_path,
                              objects);

  g_list_free(objects);
  g_string_free(object_path_string, TRUE);

  if (!link_result) {
    return NULL;
  }

  void* handle = dlopen(shared_object_path, RTLD_NOW | RTLD_GLOBAL);

  if (handle == NULL) {
    GString* message = g_string_new(NULL);
    g_string_printf(message, "dlopen failed: %s", dlerror());
    *out_message = g_string_new(message->str);
    g_string_free(message, TRUE);
  }

  return handle;
}

frg_testing_compilation_test_context_linked_t*
  frg_testing_test_compilation_phase_run_linking(
    GString** out_message,
    const frg_testing_compilation_test_options_t* options,
    const frg_testing_compilation_test_context_initialized_t* context_initialized,
    const frg_testing_compilation_test_context_codegen_t* context_codegen) {
  frg_assert_pointer_non_null(options);
  frg_assert_pointer_non_null(context_initialized);
  frg_assert_pointer_non_null(context_initialized->message_buffer);
  frg_assert_pointer_non_null(context_codegen);
  frg_assert_pointer_non_null(context_codegen->codegen_module);

  gchar* object_path
    = _frg_testing_test_compilation_phase_linking_format_object_path(options);

  if (!frg_codegen_module_write_object_file(context_initialized->message_buffer,
                                            context_codegen->codegen_module,
                                            object_path)) {
    g_free(object_path);
    return NULL;
  }

  frg_linking_configuration_t* linking_configuration
    = frg_linking_configuration_detect();

  gchar* shared_object_path
    = _frg_testing_test_compilation_phase_linking_format_shared_object_path(options);

  void* handle
    = _frg_testing_test_compilation_phase_linking_link(out_message,
                                                       context_initialized,
                                                       object_path,
                                                       shared_object_path,
                                                       linking_configuration);

  frg_linking_configuration_destroy(linking_configuration);
  g_free(object_path);
  g_free(shared_object_path);

  if (handle == NULL) {
    return NULL;
  }

  return frg_testing_compilation_test_context_linked_new(handle);
}
