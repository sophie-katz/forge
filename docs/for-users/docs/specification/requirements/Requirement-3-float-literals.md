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

# Requirement-3: Float literals

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

**Start date:** 2024-01-05

**Fully supported as of Forge version:** In progress

## Subitems

This requirement contains subitems:

1. Binary literals
2. Octal literals
3. Hexadecimal literals
4. Type suffixes
5. Underscores
6. Scientific notation

See below for details on what these subitems are. They are used to help describe the progress of implementation.

## Summary

Float literals represent floating-point values.

## Syntax specification

```
0b[0-1_]*[0-1]\.[0-1_]*[0-1](e-?[0-1_]*[0-1]\.[0-1_]*[0-1])?(f32|f64)?

0o[0-7_]*[0-7]\.[0-7_]*[0-7](e-?[0-7_]*[0-7]\.[0-7_]*[0-7])?(f32|f64)?

(0|([1-9]([0-9_]*[0-9])?))\.[0-9_]*[0-9](f32|f64)?e-?(0|([1-9]([0-9_]*[0-9])?))\.[0-9_]*[0-9]

0x[0-9a-fA-F_]*[0-9a-fA-F]\.[0-9a-fa-F_]*(e-?[0-9a-fA-F_]*[0-9a-fA-F]\.[0-9a-fa-F_]*)?[0-9a-fA-F](f32|f64)?
```

Regular expression above is specified using the [PCRE2 syntax](https://www.pcre.org/current/doc/html/pcre2syntax.html).

## Behavior

### Prefixes

Float literals can have the following prefixes:

- `0b` for base 2 literals (subitem 1)

- `0o` for base 8 literals (subitem 2)

- `0x` for base 16 literals (subitem 3)

Float literals with none of the above prefixes are base 10 literals. The integral part can be `0` but cannot start with a `0` character followed by other digits.

### Suffixes (subitem 4)

Float literals can have the following suffixes:

- `f32` for 32-bit signed float literals

- `f64` for 64-bit signed float literals

The float literal will always be put into a value of this type. The value will be truncated if needed.

### Scientific notation (subitem 6)

Float literals can have a scientific notation suffix. This is specified using the `e` character followed by an optional `-` character and then another floating point literal.

### Underscores (subitem 5)

Float literals can have underscores (`_`) in them. These are ignored by the compiler and are only used to make the literal easier to read. They cannot appear at the beginning or end of string literals.

## Examples

Here are some examples of usage.

### Valid

These examples are expected to compile successfully.

```
0.0

10.0

10.0f32

3e-5f32

0b10.01

0o07.70

0xaa.bb

10.000_3
```

### Invalid

These are examples of invalid usage of this feature and should cause compilation errors.

```
0o8.0 # This is invalid because 8 is not a valid octal digit

_1.0 # This is invalid because underscores cannot appear at the beginning of a literal

1.0_ # This is invalid because underscores cannot appear at the end of a literal

00.3 # This is invalid because decimal literals cannot be prefixed by zeros
```

## Related

- [Requirement-2: Integer literals](Requirement-2-integer-literals.md)
