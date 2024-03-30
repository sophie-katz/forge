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

#pragma once

#include <build_configuration.h>
#include <stdio.h>
#include <stdlib.h>

#define frg_die(format, ...)                                                           \
  {                                                                                    \
    fprintf(stderr, "%s:%i: forge internal error: ", __FILE__, __LINE__);              \
    fprintf(stderr, (format), ##__VA_ARGS__);                                          \
    fprintf(stderr, "\n");                                                             \
    abort();                                                                           \
  }

#define frg_die_unexpected_enum_value(enum_value)                                      \
  frg_die("unexpected enum value for '%s', %i", #enum_value, (int)(enum_value))

#define frg_die_ast_kind_not_yet_supported(ast_kind)                                   \
  frg_die("support for AST kind '%s', %s not yet supported for this operation",        \
          #ast_kind,                                                                   \
          frg_ast_node_kind_info_get(ast_kind)->name)

#define frg_die_message_no_code_yet()                                                  \
  frg_die("message emitted here does not yet have a code defined")
