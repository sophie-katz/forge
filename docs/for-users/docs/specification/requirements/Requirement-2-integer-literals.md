<!--
This program is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this
program. If not, see <https://www.gnu.org/licenses/>.

Copyright 2023-2024 Sophie Katz
-->

# Requirement-2: Integer literals

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

**Start date:** 2024-01-05
**Fully supported as of Forge version:** In progress

## Subitems

It contains subitems:

1. Binary literals
2. Octal literals
3. Hexadecimal literals
4. Type suffixes
5. Underscores

See below for details on what these subitems are. They are used to help describe the progress of implementation.

## Summary

Integer literals represent integer values.

## Syntax specification

```
0b[0-1_]*[0-1](i8|i16|i32|i64|u8|u16|u32|u64)?

0o[0-7_]*[0-7](i8|i16|i32|i64|u8|u16|u32|u64)?

(0|([1-9]([0-9_]*[0-9])?))(i8|i16|i32|i64|u8|u16|u32|u64)?

0x[0-9a-fA-F_]*[0-9a-fA-F](i8|i16|i32|i64|u8|u16|u32|u64)?
```

Regular expression above is specified using the [PCRE2 syntax](https://www.pcre.org/current/doc/html/pcre2syntax.html).

## Behavior

### Prefixes

Integer literals can have the following prefixes:

- `0b` for base 2 literals (subitem 1)

- `0o` for base 8 literals (subitem 2)

- `0x` for base 16 literals (subitem 3)

Integer literals with none of the above prefixes are base 10 literals. These can be `0` but cannot start with a `0` character followed by other digits.

### Suffixes (subitem 4)

Integer literals can have the following suffixes:

- `i8` for 8-bit signed integer literals

- `i16` for 16-bit signed integer literals

- `i32` for 32-bit signed integer literals

- `i64` for 64-bit signed integer literals

- `u8` for 8-bit unsigned integer literals

- `u16` for 16-bit unsigned integer literals

- `u32` for 32-bit unsigned integer literals

- `u64` for 64-bit unsigned integer literals

The integer literal will always be put into a value of this type. If the value cannot be properly represented without truncation, an error will be thrown. For example, `128i8` is invalid because 8-bit signed integers can only represent values from `-128` to `127`.

### Underscores (subitem 5)

Integer literals can have underscores (`_`) in them. These are ignored by the compiler and are only used to make the literal easier to read. They cannot appear at the beginning or end of string literals.

## Examples

Here are some examples of usage.

### Valid

These examples are expected to compile successfully.

```
0

10

10u32

0b1010 # This is 10 in decimal

0b11111111i8 # This is -128 in decimal

0o33 # This is 27 in decimal

0x10 # This is 16 in decimal
```

### Invalid

These are examples of invalid usage of this feature and should cause compilation errors.

```
0o8 # This is invalid because 8 is not a valid octal digit

128i8 # This is invalid because 8-bit signed integers can only represent values from -128 to 127

_10 # This is invalid because underscores cannot be at the beginning of integer literals

10_ # This is invalid because underscores cannot be at the end of integer literals

0100 # This is invalid because decimal literals cannot be prefixed by zeros

0x

0b

0b2

0xg

0B10 # This is invalid because the base prefix must be lowercase

0O10 # This is invalid because the base prefix must be lowercase

0X10 # This is invalid because the base prefix must be lowercase

10U32 # This is invalid because the suffix must be lowercase
```

## Related

- [Requirement-3: Float literals](Requirement-3-float-literals.md)
