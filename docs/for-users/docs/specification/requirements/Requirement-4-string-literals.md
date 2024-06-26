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

<!--
Follow any instructions prefixed with 'CHANGE:' and then remove the comments (including this one).
-->

# Requirement-4: String literals

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

**Start date:** 2024-01-05

**Fully supported as of Forge version:** In progress

## Subitems

This requirement contains subitems:

1. Escape codes
2. Hex byte escape codes
3. Unicode codepoint escape codes

See below for details on what these subitems are. They are used to help describe the progress of implementation.

## Summary

String literals which represent all possible string values.

## Background

Forge string literals are heavily based on [C string literals](https://en.cppreference.com/w/c/language/string_literal), as are most modern programming languages.

This specification references:

- [ASCII specification](https://www.ascii-code.com/)

- [UTF-8 specification](https://www.unicode.org/versions/Unicode15.1.0/)

- [Summary of UTF-32](https://www.ibm.com/docs/en/i/7.1?topic=unicode-utf-32)

## Description

String literals should be able represent all possible strings, including those with unicode characters and those with null bytes inside them.

All strings in Forge are UTF-8. Other encodings are not supported directly by the syntax. ASCII is supported in that ASCII is a subset of UTF-8.

## Syntax specification

```
\"([^"\\]|\\.)*\"
```

Regular expression above is specified using the [PCRE2 syntax](https://www.pcre.org/current/doc/html/pcre2syntax.html).

## Behavior

String literals are prefixed and suffixed by `"` characters. All characters in the middle can match any of these patterns:

- Printable ASCII characters other than `\` (decimal 34), `"` (decimal 92), and delete (decimal 127)
- Printable UTF-8 characters
- `\` followed by any ASCII character, including `\` and `"`

### Escape codes (subitem 1)

The following escape codes are supported:

- `\a` for the alert character (decimal 7)

- `\b` for the backspace character (decimal 8)

- `\e` for the ASCII escape character (decimal 27)

- `\f` for the form feed character (decimal 12)

- `\n` for the line feed character (decimal 10)

- `\r` for the carriage return character (decimal 13)

- `\t` for the horizontal tab character (decimal 9)

- `\v` for the vertical tab character (decimal 11)

- `\0` for a null byte (decimal 0)

- `\xHH` where `H` represents a hexadecimal digit (subitem 2, exactly 2 digits must be used)

- `\u{HHHHHH}` where `H` represents a hexadecimal digit (subitem 3, 1-6 digits may be used)

`\` followed by any other character simply has the value of that character. For example, `\"` is a string literal with the value `"`.

## Examples

Here are some examples of usage.

### Valid

These examples are expected to compile successfully.

```
""

"hello, world."

"\""

"\\"

"\x00"

"\u{0000}"
```

### Invalid

These are examples of invalid usage of this feature and should cause compilation errors.

```
"

"'

"\"

"\x0"

"\u{}"

"\u{g0}"

"\u{00000}" # This has 5 digits, which is too many
```

## Related

- [Requirement-5: Character literals](Requirement-5-character-literals.md)
