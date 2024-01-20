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

#include <forge/common/memory.h>
#include <forge/common/error.h>
#include <forge/linking/linker_config.h>

GString* _frg_linker_resolve_path(const char* name) {
    gchar* path = g_find_program_in_path(name);

    if (path == NULL) {
        return NULL;
    } else {
        // Taken from https://github.com/GNOME/glib/blob/main/glib/gstring.c#L165
        GString* result = g_slice_new(GString);
        
        result->str = path;
        result->len = strlen(result->str);
        result->allocated_len = result->len + 1;
        
        return result;
    }
}

frg_linker_config_t* frg_linker_config_new_default(void) {
    frg_linker_config_t* config = frg_safe_malloc(sizeof(frg_linker_config_t));

#ifdef G_OS_WIN32
    config->ld_lld_path = NULL;
    config->ld64_lld_path = NULL;
    config->lld_link_path = _frg_linker_resolve_path("lld-link");
#else
    config->ld_lld_path = _frg_linker_resolve_path("ld.lld");
    config->ld64_lld_path = _frg_linker_resolve_path("ld64.lld");
    config->lld_link_path = NULL;
#endif
    
    if (config->ld64_lld_path != NULL) {
        config->linker_id = FRG_LINKER_ID_LD64_LLD;
    } else if (config->ld_lld_path != NULL) {
        config->linker_id = FRG_LINKER_ID_LD_LLD;
    } else if (config->lld_link_path != NULL) {
        config->linker_id = FRG_LINKER_ID_LLD_LINK;
    } else {
        config->linker_id = FRG_LINKER_ID_NONE;
    }

    return config;
}

void frg_linker_config_destroy(frg_linker_config_t** config) {
    frg_assert_pointer_non_null(config);
    frg_assert_pointer_non_null(*config);

    if ((*config)->ld_lld_path != NULL) {
        g_string_free((*config)->ld_lld_path, TRUE);
    }

    if ((*config)->ld64_lld_path != NULL) {
        g_string_free((*config)->ld64_lld_path, TRUE);
    }

    if ((*config)->lld_link_path != NULL) {
        g_string_free((*config)->lld_link_path, TRUE);
    }

    frg_safe_free((void**)config);
}

void frg_linker_config_set_linker_id(
    frg_linker_config_t* config,
    frg_linker_id_t linker_id
) {
    frg_assert_pointer_non_null(config);

    config->linker_id = linker_id;
}

void frg_linker_config_set_linker_path(
    frg_linker_config_t* config,
    frg_linker_id_t linker_id,
    const char* linker_path
) {
    frg_assert_pointer_non_null(config);
    frg_assert_string_non_empty(linker_path);

    config->linker_id = linker_id;
    
    switch (linker_id) {
        case FRG_LINKER_ID_NONE:
            frg_die("cannot set linker path for linker ID 'none'");
        case FRG_LINKER_ID_LD_LLD:
            if (config->ld_lld_path != NULL) {
                g_string_free(config->ld_lld_path, TRUE);
            }

            config->ld_lld_path = g_string_new(linker_path);

            break;
        case FRG_LINKER_ID_LD64_LLD:
            if (config->ld64_lld_path != NULL) {
                g_string_free(config->ld64_lld_path, TRUE);
            }

            config->ld64_lld_path = g_string_new(linker_path);

            break;
        case FRG_LINKER_ID_LLD_LINK:
            if (config->lld_link_path != NULL) {
                g_string_free(config->lld_link_path, TRUE);
            }

            config->lld_link_path = g_string_new(linker_path);

            break;
        default:
            frg_die_unexpected_enum_value(linker_id);
    }
}

const char* frg_linker_config_get_linker_path(
    const frg_linker_config_t* config
) {
    frg_assert_pointer_non_null(config);

    switch (config->linker_id) {
        case FRG_LINKER_ID_NONE:
            frg_die("cannot get linker path for linker ID 'none'");
        case FRG_LINKER_ID_LD_LLD:
            frg_assert_gstring_non_empty(config->ld_lld_path);
            return config->ld_lld_path->str;
        case FRG_LINKER_ID_LD64_LLD:
            frg_assert_gstring_non_empty(config->ld64_lld_path);
            return config->ld64_lld_path->str;
        case FRG_LINKER_ID_LLD_LINK:
            frg_assert_gstring_non_empty(config->lld_link_path);
            return config->lld_link_path->str;
        default:
            frg_die_unexpected_enum_value(config->linker_id);
    }
}
