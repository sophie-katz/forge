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

#pragma once

#include <build_configuration.h>
#include <forge/die.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef FRG_SHOULD_BUILD_ASSERTS
  #define frg_assert(condition)                                                        \
    {                                                                                  \
      if (!(condition)) {                                                              \
        frg_die("assertion must be met: %s", #condition);                              \
      }                                                                                \
    }

  #define frg_assert_with_message(condition, message)                                  \
    {                                                                                  \
      if (!(condition)) {                                                              \
        frg_die("assertion must be met: %s (%s)", #condition, (message));              \
      }                                                                                \
    }

  #define frg_assert_pointer_non_null(pointer)                                         \
    {                                                                                  \
      if ((pointer) == NULL) {                                                         \
        frg_die("'%s' must not be NULL", #pointer);                                    \
      }                                                                                \
    }

  #define frg_assert_pointer_null(pointer)                                             \
    {                                                                                  \
      if ((pointer) != NULL) {                                                         \
        frg_die("'%s' must be NULL", #pointer);                                        \
      }                                                                                \
    }

  #define frg_assert_string_non_empty(string)                                          \
    {                                                                                  \
      if ((string) == NULL) {                                                          \
        frg_die("'%s' must not be NULL", #string);                                     \
      } else if ((string)[0] == 0) {                                                   \
        frg_die("'%s' must be a non-empty string", #string);                           \
      }                                                                                \
    }

  #define frg_assert_gstring_non_empty(gstring)                                        \
    {                                                                                  \
      if ((gstring) == NULL) {                                                         \
        frg_die("'%s' must not be NULL", #gstring);                                    \
      } else if ((gstring)->len == 0) {                                                \
        frg_die("'%s' must be a non-empty string", #gstring);                          \
      }                                                                                \
    }

  #define frg_assert_int_positive(value)                                               \
    {                                                                                  \
      if ((value) <= 0) {                                                              \
        frg_die("'%s' must be positive, not %li", #value, (int64_t)(value));           \
      }                                                                                \
    }

  #define frg_assert_int_non_negative(value)                                           \
    {                                                                                  \
      if ((value) < 0) {                                                               \
        frg_die("'%s' must be non-negative, not %li", #value, (int64_t)(value));       \
      }                                                                                \
    }

  #define frg_assert_int_less_than_or_equal_toss_than(lhs, rhs)                        \
    {                                                                                  \
      if (!((lhs) < (rhs))) {                                                          \
        frg_die(                                                                       \
          "'lhs' must be less than 'rhs'\n  lhs: %s\n    == %li\n  rhs: %s\n    == "   \
          "%li",                                                                       \
          #lhs,                                                                        \
          (int64_t)(lhs),                                                              \
          #rhs,                                                                        \
          (int64_t)(rhs));                                                             \
      }                                                                                \
    }

  #define frg_assert_int_less_than_or_equal_to(lhs, rhs)                               \
    {                                                                                  \
      if (!((lhs) <= (rhs))) {                                                         \
        frg_die(                                                                       \
          "'lhs' must be less than or equal to 'rhs'\n  lhs: %s\n    == %li\n  rhs: "  \
          "%s\n    == %li",                                                            \
          #lhs,                                                                        \
          (int64_t)(lhs),                                                              \
          #rhs,                                                                        \
          (int64_t)(rhs));                                                             \
      }                                                                                \
    }

  #define frg_assert_int_greater_than(lhs, rhs)                                        \
    {                                                                                  \
      if (!((lhs) > (rhs))) {                                                          \
        frg_die(                                                                       \
          "'lhs' must be greater than 'rhs'\n  lhs: %s\n    == %li\n  rhs: %s\n    "   \
          "== %li",                                                                    \
          #lhs,                                                                        \
          (int64_t)(lhs),                                                              \
          #rhs,                                                                        \
          (int64_t)(rhs));                                                             \
      }                                                                                \
    }

  #define frg_assert_int_greater_than_or_equal_to(lhs, rhs)                            \
    {                                                                                  \
      if (!((lhs) >= (rhs))) {                                                         \
        frg_die(                                                                       \
          "'lhs' must be greater than or equal to 'rhs'\n  lhs: %s\n    == %li\n  "    \
          "rhs: %s\n    == %li",                                                       \
          #lhs,                                                                        \
          (int64_t)(lhs),                                                              \
          #rhs,                                                                        \
          (int64_t)(rhs));                                                             \
      }                                                                                \
    }

  #define frg_assert_int_not_equal_to(lhs, rhs)                                        \
    {                                                                                  \
      if (!((lhs) != (rhs))) {                                                         \
        frg_die(                                                                       \
          "'lhs' must not be equal to 'rhs'\n  lhs: %s\n    == %li\n  rhs: %s\n    "   \
          "== %li",                                                                    \
          #lhs,                                                                        \
          (int64_t)(lhs),                                                              \
          #rhs,                                                                        \
          (int64_t)(rhs));                                                             \
      }                                                                                \
    }

  #define frg_assert_int_equal_to(lhs, rhs)                                            \
    {                                                                                  \
      if (!((lhs) == (rhs))) {                                                         \
        frg_die(                                                                       \
          "'lhs' must be equal to 'rhs'\n  lhs: %s\n    == %li\n  rhs: %s\n    == "    \
          "%li",                                                                       \
          #lhs,                                                                        \
          (int64_t)(lhs),                                                              \
          #rhs,                                                                        \
          (int64_t)(rhs));                                                             \
      }                                                                                \
    }

  #define frg_assert_uint_positive(value)                                              \
    {                                                                                  \
      if ((value) <= 0) {                                                              \
        frg_die("'%s' must be positive, not %Lu", #value, (uint64_t)(value));          \
      }                                                                                \
    }

  #define frg_assert_uint_less_than_or_equal_toss_than(lhs, rhs)                       \
    {                                                                                  \
      if (!((lhs) < (rhs))) {                                                          \
        frg_die(                                                                       \
          "'lhs' must be less than 'rhs'\n  lhs: %s\n    == %Lu\n  rhs: %s\n    == "   \
          "%Lu",                                                                       \
          #lhs,                                                                        \
          (uint64_t)(lhs),                                                             \
          #rhs,                                                                        \
          (uint64_t)(rhs));                                                            \
      }                                                                                \
    }

  #define frg_assert_uint_less_than_or_equal_to(lhs, rhs)                              \
    {                                                                                  \
      if (!((lhs) <= (rhs))) {                                                         \
        frg_die(                                                                       \
          "'lhs' must be less than or equal to 'rhs'\n  lhs: %s\n    == %Lu\n  rhs: "  \
          "%s\n    == %Lu",                                                            \
          #lhs,                                                                        \
          (uint64_t)(lhs),                                                             \
          #rhs,                                                                        \
          (uint64_t)(rhs));                                                            \
      }                                                                                \
    }

  #define frg_assert_uint_greater_than(lhs, rhs)                                       \
    {                                                                                  \
      if (!((lhs) > (rhs))) {                                                          \
        frg_die(                                                                       \
          "'lhs' must be greater than 'rhs'\n  lhs: %s\n    == %Lu\n  rhs: %s\n    "   \
          "== %Lu",                                                                    \
          #lhs,                                                                        \
          (uint64_t)(lhs),                                                             \
          #rhs,                                                                        \
          (uint64_t)(rhs));                                                            \
      }                                                                                \
    }

  #define frg_assert_uint_greater_than_or_equal_to(lhs, rhs)                           \
    {                                                                                  \
      if (!((lhs) >= (rhs))) {                                                         \
        frg_die(                                                                       \
          "'lhs' must be greater than or equal to 'rhs'\n  lhs: %s\n    == %Lu\n  "    \
          "rhs: %s\n    == %Lu",                                                       \
          #lhs,                                                                        \
          (uint64_t)(lhs),                                                             \
          #rhs,                                                                        \
          (uint64_t)(rhs));                                                            \
      }                                                                                \
    }

  #define frg_assert_uint_not_equal_to(lhs, rhs)                                       \
    {                                                                                  \
      if (!((lhs) != (rhs))) {                                                         \
        frg_die(                                                                       \
          "'lhs' must not be equal to 'rhs'\n  lhs: %s\n    == %Lu\n  rhs: %s\n    "   \
          "== %Lu",                                                                    \
          #lhs,                                                                        \
          (uint64_t)(lhs),                                                             \
          #rhs,                                                                        \
          (uint64_t)(rhs));                                                            \
      }                                                                                \
    }

  #define frg_assert_uint_equal_to(lhs, rhs)                                           \
    {                                                                                  \
      if (!((lhs) == (rhs))) {                                                         \
        frg_die(                                                                       \
          "'lhs' must be equal to 'rhs'\n  lhs: %s\n    == %Lu\n  rhs: %s\n    == "    \
          "%Lu",                                                                       \
          #lhs,                                                                        \
          (uint64_t)(lhs),                                                             \
          #rhs,                                                                        \
          (uint64_t)(rhs));                                                            \
      }                                                                                \
    }
#else
  #define frg_assert(condition)
  #define frg_assert_with_message(condition, message)
  #define frg_assert_pointer_non_null(pointer)
  #define frg_assert_pointer_null(pointer)
  #define frg_assert_string_non_empty(string)
  #define frg_assert_int_positive(value)
  #define frg_assert_int_non_negative(value)
  #define frg_assert_int_less_than_or_equal_toss_than(lhs, rhs)
  #define frg_assert_int_less_than_or_equal_to(lhs, rhs)
  #define frg_assert_int_greater_than(lhs, rhs)
  #define frg_assert_int_greater_than_or_equal_to(lhs, rhs)
  #define frg_assert_int_not_equal_to(lhs, rhs)
  #define frg_assert_int_equal_to(lhs, rhs)
  #define frg_assert_uint_positive(value)
  #define frg_assert_uint_less_than_or_equal_toss_than(lhs, rhs)
  #define frg_assert_uint_less_than_or_equal_to(lhs, rhs)
  #define frg_assert_uint_greater_than(lhs, rhs)
  #define frg_assert_uint_greater_than_or_equal_to(lhs, rhs)
  #define frg_assert_uint_not_equal_to(lhs, rhs)
  #define frg_assert_uint_equal_to(lhs, rhs)
#endif
