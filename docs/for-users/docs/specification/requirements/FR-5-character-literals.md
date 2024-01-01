<!--
Copyright (c) 2024 Sophie Katz

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

<!--
Follow any instructions prefixed with 'CHANGE:' and then remove the comments (including this one).
-->

# FR-5: Character literals

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

Character literals which represent all possible character values.

## Background

This requirement is based off of [FR-5: String literals](FR-5-string-literals.md). Character literals are a specialization of string literals.

## Description

Character literals should be able represent all possible characters, including unicode characters and the null character (decimal 0).

Characters are stored as 4-byte unsigned integers.

## Syntax specification

```
'([^'\\]|\\.)+'
```

Regular expression above is specified using the [PCRE2 syntax](https://www.pcre.org/current/doc/html/pcre2syntax.html).

## Behavior

Character literals identical to string literals except that:

- They are prefixed and suffixed by `'` characters instead of `"` characters.

- They contain exactly one unicode code point.

## Examples

Here are some examples of usage.

### Valid

These examples are expected to compile successfully.

```
'a'

'\''

'\\'

'\x00'

'\u{0000}'
```

### Invalid

These are examples of invalid usage of this feature and should cause compilation errors.

```
'

''

'"

'\'

'\x0'

'\u{}'

'\u{g0}'

'\u{00000}' # This has 5 digits, which is too many
```

## Related

- [FR-4: String literals](FR-4-string-literals.md)
