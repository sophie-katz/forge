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

# Requirement-6: File naming

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

**Start date:** 2024-01-05
**Fully supported as of Forge version:** In progress

## Description

Forge source files should use the `*.frg` file extension to be recognized by the compiler. They should be named in `kebab-case` with an optional kebab case suffix after a `.` character.

## Syntax specification

```
[a-z][a-z0-9]*(-[a-z][a-z0-9]*)*(\.[a-z][a-z0-9]*(-[a-z][a-z0-9]*)*)?\.frg
```

Regular expression above is specified using the [PCRE2 syntax](https://www.pcre.org/current/doc/html/pcre2syntax.html).

## Examples

Here are some examples of valid file names.

### Valid

These examples are expected to be passed to the compiler successfully.

```
main.frg
lib.frg
my-cool-file.frg
lib.test.frg
```

### Invalid

These are examples of invalid usage of this feature and should cause compilation errors.

```
main.forge
main.frg.txt
Main.frg
MyCoolFile.frg
my_cool_file.frg
main
lib.test.test.frg
.frg
```
