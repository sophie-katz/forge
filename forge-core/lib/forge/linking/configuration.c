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
#include <forge/linking/configuration.h>
#include <forge/memory.h>

GString* _frg_linking_resolve_path(const char* name) {
  gchar* path = g_find_program_in_path(name);

  if (path == NULL) {
    return NULL;
  } else {
    // Taken from https://github.com/GNOME/glib/blob/main/glib/gstring.c#L165
    GString* result       = g_slice_new(GString);

    result->str           = path;
    result->len           = strlen(result->str);
    result->allocated_len = result->len + 1;

    return result;
  }
}

GString* _frg_linking_get_path_ld_lld() {
#ifdef G_OS_WIN32
  return NULL;
#else
  return _frg_linking_resolve_path("ld.lld");
#endif
}

GString* _frg_linking_get_path_ld64_lld() {
#ifdef G_OS_WIN32
  return NULL;
#else
  return _frg_linking_resolve_path("ld64.lld");
#endif
}

GString* _frg_linking_get_path_lld_link() {
#ifdef G_OS_WIN32
  return _frg_linking_resolve_path("lld-link");
#else
  return NULL;
#endif
}

const char* frg_linking_linker_configuration_get_path(
  const frg_linking_linker_configuration_t* configuration) {
  frg_assert_pointer_non_null(configuration);

  return configuration->_path->str;
}

void frg_linking_linker_configuration_set_path(
  frg_linking_linker_configuration_t* mut_configuration, const char* path) {
  frg_assert_pointer_non_null(mut_configuration);
  frg_assert_string_non_empty(path);

  g_string_assign(mut_configuration->_path, path);
}

frg_linking_configuration_t* frg_linking_configuration_detect() {
  frg_linking_configuration_t* configuration
    = frg_malloc(sizeof(frg_linking_configuration_t));

  GString* path = _frg_linking_get_path_ld_lld();
  if (path != NULL) {
    configuration->_current_linker_kind = FRG_LINKING_LINKER_KIND_LD_LLD;
    configuration->_linker_configuration._for_ld_lld._path = path;
    return configuration;
  }

  path = _frg_linking_get_path_ld64_lld();
  if (path != NULL) {
    configuration->_current_linker_kind = FRG_LINKING_LINKER_KIND_LD64_LLD;
    configuration->_linker_configuration._for_ld_lld._path = path;
    return configuration;
  }

  path = _frg_linking_get_path_lld_link();
  if (path != NULL) {
    configuration->_current_linker_kind = FRG_LINKING_LINKER_KIND_LLD_LINK;
    configuration->_linker_configuration._for_lld_link._path = path;
    return configuration;
  }

  configuration->_current_linker_kind = FRG_LINKING_LINKER_KIND_NONE;
  memset(&configuration->_linker_configuration,
         0,
         sizeof(configuration->_linker_configuration));

  return configuration;
}

void frg_linking_configuration_destroy(frg_linking_configuration_t* configuration) {
  frg_assert_pointer_non_null(configuration);

  switch (configuration->_current_linker_kind) {
  case FRG_LINKING_LINKER_KIND_LD_LLD:
    g_string_free(configuration->_linker_configuration._for_ld_lld._path, TRUE);
    break;
  case FRG_LINKING_LINKER_KIND_LD64_LLD:
    g_string_free(configuration->_linker_configuration._for_ld64_lld._path, TRUE);
    break;
  case FRG_LINKING_LINKER_KIND_LLD_LINK:
    g_string_free(configuration->_linker_configuration._for_lld_link._path, TRUE);
    break;
  default:
    break;
  }

  frg_free(configuration);
}

frg_linking_linker_kind_t frg_linking_configuration_get_current_linker_kind(
  const frg_linking_configuration_t* configuration) {
  frg_assert_pointer_non_null(configuration);

  return configuration->_current_linker_kind;
}

void frg_linking_configuration_set_current_linker_kind(
  frg_linking_configuration_t* mut_configuration,
  frg_linking_linker_kind_t linker_kind) {
  frg_assert_pointer_non_null(mut_configuration);

  bool found = false;

  switch (linker_kind) {
  case FRG_LINKING_LINKER_KIND_LD_LLD:
    mut_configuration->_linker_configuration._for_ld_lld._path
      = _frg_linking_get_path_ld_lld();
    found = (mut_configuration->_linker_configuration._for_ld_lld._path != NULL);
    break;
  case FRG_LINKING_LINKER_KIND_LD64_LLD:
    mut_configuration->_linker_configuration._for_ld64_lld._path
      = _frg_linking_get_path_ld64_lld();
    found = (mut_configuration->_linker_configuration._for_ld64_lld._path != NULL);
    break;
  case FRG_LINKING_LINKER_KIND_LLD_LINK:
    mut_configuration->_linker_configuration._for_lld_link._path
      = _frg_linking_get_path_lld_link();
    found = (mut_configuration->_linker_configuration._for_lld_link._path != NULL);
    break;
  default:
    frg_die_unexpected_enum_value(linker_kind);
  }

  if (!found) {
    mut_configuration->_current_linker_kind = FRG_LINKING_LINKER_KIND_NONE;
    memset(&mut_configuration->_linker_configuration,
           0,
           sizeof(mut_configuration->_linker_configuration));
  }
}

frg_linking_linker_configuration_t* frg_linking_configuration_get_current_linker(
  frg_linking_configuration_t* mut_configuration) {
  frg_assert_pointer_non_null(mut_configuration);

  switch (mut_configuration->_current_linker_kind) {
  case FRG_LINKING_LINKER_KIND_LD_LLD:
    return &mut_configuration->_linker_configuration._for_ld_lld;
  case FRG_LINKING_LINKER_KIND_LD64_LLD:
    return &mut_configuration->_linker_configuration._for_ld64_lld;
  case FRG_LINKING_LINKER_KIND_LLD_LINK:
    return &mut_configuration->_linker_configuration._for_lld_link;
  default:
    return NULL;
  }
}
