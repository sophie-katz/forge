// Copyright (c) 2024 Sophie Katz
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

#include <forge/common/log.h>
#include <stdlib.h>

#define frg_die(format, ...) { \
    frg_log_internal_error((format), ##__VA_ARGS__); \
    abort(); \
}

#define frg_die_unexpected_enum_value(enum_value) \
    frg_die("unexpected enum value for '%s': %i", #enum_value, (int)(enum_value))

#ifdef NDEBUG
#define frg_assert(condition)
#define frg_assert_pointer_non_null(pointer)
#define frg_assert_string_non_empty(string)
#define frg_assert_int_positive(value)
#define frg_assert_int_non_negative(value)
#define frg_assert_int_lt(lhs, rhs)
#define frg_assert_int_le(lhs, rhs)
#define frg_assert_int_gt(lhs, rhs)
#define frg_assert_int_ge(lhs, rhs)
#define frg_assert_int_ne(lhs, rhs)
#define frg_assert_int_eq(lhs, rhs)
#else
#define frg_assert(condition) { \
    if (!(condition)) { \
        frg_die("assertion must be met: %s", #condition); \
    } \
}

#define frg_assert_pointer_non_null(pointer) { \
    if ((pointer) == NULL) { \
        frg_die("'%s' must not be NULL", #pointer); \
    } \
}

#define frg_assert_string_non_empty(string) { \
    if ((string) == NULL) { \
        frg_die("'%s' must not be NULL", #string); \
    } else if ((string)[0] == 0) { \
        frg_die("'%s' must be a non-empty string", #string); \
    } \
}

#define frg_assert_gstring_non_empty(gstring) { \
    if ((gstring) == NULL) { \
        frg_die("'%s' must not be NULL", #gstring); \
    } else if ((gstring)->len == 0) { \
        frg_die("'%s' must be a non-empty string", #gstring); \
    } \
}

#define frg_assert_int_positive(value) { \
    if ((value) <= 0) { \
        frg_die("'%s' must be positive, not %Li", #value, (value)); \
    } \
}

#define frg_assert_int_non_negative(value) { \
    if ((value) < 0) { \
        frg_die("'%s' must be non-negative, not %Li", #value, (value)); \
    } \
}

#define frg_assert_int_lt(lhs, rhs) { \
    if (!((lhs) < (rhs))) { \
        frg_die("'lhs' must be less than 'rhs'\n  lhs: %s\n    == %Li\n  rhs: %s\n    == %Li", #lhs, (lhs), #rhs, (rhs)); \
    } \
}

#define frg_assert_int_le(lhs, rhs) { \
    if (!((lhs) <= (rhs))) { \
        frg_die("'lhs' must be less than or equal to 'rhs'\n  lhs: %s\n    == %Li\n  rhs: %s\n    == %Li", #lhs, (lhs), #rhs, (rhs)); \
    } \
}

#define frg_assert_int_gt(lhs, rhs) { \
    if (!((lhs) > (rhs))) { \
        frg_die("'lhs' must be greater than 'rhs'\n  lhs: %s\n    == %Li\n  rhs: %s\n    == %Li", #lhs, (lhs), #rhs, (rhs)); \
    } \
}

#define frg_assert_int_ge(lhs, rhs) { \
    if (!((lhs) >= (rhs))) { \
        frg_die("'lhs' must be greater than or equal to 'rhs'\n  lhs: %s\n    == %Li\n  rhs: %s\n    == %Li", #lhs, (lhs), #rhs, (rhs)); \
    } \
}

#define frg_assert_int_ne(lhs, rhs) { \
    if (!((lhs) != (rhs))) { \
        frg_die("'lhs' must not be equal to 'rhs'\n  lhs: %s\n    == %Li\n  rhs: %s\n    == %Li", #lhs, (lhs), #rhs, (rhs)); \
    } \
}

#define frg_assert_int_eq(lhs, rhs) { \
    if (!((lhs) == (rhs))) { \
        frg_die("'lhs' must be equal to 'rhs'\n  lhs: %s\n    == %Li\n  rhs: %s\n    == %Li", #lhs, (lhs), #rhs, (rhs)); \
    } \
}
#endif
