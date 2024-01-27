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

#include <forge/common/enums.h>
#include <forge/common/error.h>

GString* frg_ast_decl_prop_flags_to_string(frg_ast_decl_prop_flags_t flags) {
    GString* result = g_string_new(NULL);
    bool first = true;

    if (flags & FRG_AST_DECL_PROP_FLAG_OPTIONAL) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "optional");

        first = false;
    }

    if (flags & FRG_AST_DECL_PROP_FLAG_NON_OPTIONAL) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "non-optional");

        first = false;
    }

    if (flags & FRG_AST_DECL_PROP_FLAG_SPREAD) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "spread");

        first = false;
    }

    if (first) {
        g_string_append_printf(result, "none");
    }

    return result;
}

GString* frg_ast_decl_iface_flags_to_string(frg_ast_decl_iface_flags_t flags) {
    GString* result = g_string_new(NULL);
    bool first = true;

    if (flags & FRG_AST_DECL_IFACE_FLAG_ABSTRACT) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "abstract");

        first = false;
    }

    if (first) {
        g_string_append_printf(result, "none");
    }

    return result;
}

GString* frg_ast_decl_fn_arg_flags_to_string(frg_ast_decl_fn_arg_flags_t flags) {
    GString* result = g_string_new(NULL);
    bool first = true;

    if (flags & FRG_AST_DECL_FN_ARG_FLAG_KW) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "kw");

        first = false;
    }

    if (first) {
        g_string_append_printf(result, "none");
    }

    return result;
}

GString* frg_ast_decl_fn_flags_to_string(frg_ast_decl_fn_flags_t flags) {
    GString* result = g_string_new(NULL);
    bool first = true;

    if (flags & FRG_AST_DECL_FN_FLAG_MUT) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "mut");

        first = false;
    }

    if (flags & FRG_AST_DECL_FN_FLAG_OVERRIDE) {
        if (!first) {
            g_string_append_printf(result, ",");
        }

        g_string_append_printf(result, "override");

        first = false;
    }

    if (first) {
        g_string_append_printf(result, "none");
    }

    return result;
}
