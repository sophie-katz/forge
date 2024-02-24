<!--
Copyright (c) 2023-2024 Sophie Katz

This file is part of Forge.

Forge is free software: you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation, either version
3 of the License, or (at your option) any later version.

Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Forge. If
not, see <https://www.gnu.org/licenses/>.
-->

# Enum conventions

Enums should use this declaration style:

```c
typedef enum {

} frg_enum_name_t;
```

See [Naming convention](naming-convention.md) for details on how to name structs.

## Integer enums

Integer enums should be defined like so:

```c
typedef enum {
    FRG_ENUM_NAME_VALUE_A,
    FRG_ENUM_NAME_VALUE_B,
    FRG_ENUM_NAME_VALUE_C,

    FRG_ENUM_NAME_FIRST = FRG_ENUM_NAME_VALUE_A,
    FRG_ENUM_NAME_LAST = FRG_ENUM_NAME_VALUE_C,
} frg_enum_name_t;

#define FRG_ENUM_NAME_COUNT (FRG_ENUM_NAME_LAST - FRG_ENUM_NAME_FIRST + 1)
```

The `*_FIRST` and `*_LAST` values are defined to make it easier to iterate over the enum values. The `*_COUNT` macro is defined to make it easier to get the number of values in the enum. These should be present for all integer enums.

## Flag enums

Flag enums should be defined like so:

```c
typedef enum {
    FRG_ENUM_NAME_FLAG_NONE = 0,
    FRG_ENUM_NAME_FLAG_A = 1,
    FRG_ENUM_NAME_FLAG_B = 1 << 1,
    FRG_ENUM_NAME_FLAG_C = 1 << 2,
    FRG_ENUM_NAME_FLAG_D = 1 << 3,

    FRG_ENUM_NAME_FLAG_MAX = FRG_ENUM_NAME_FLAG_D
} frg_enum_name_flags_t;
```

The `*_MAX` value is defined to make it easier to iterate over the flag bits. This should be present for all flag enums.
