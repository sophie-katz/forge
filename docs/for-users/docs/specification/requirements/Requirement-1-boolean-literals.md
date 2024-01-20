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

# Requirement-1: Boolean literals

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

**Start date:** 2024-01-05
**First supported in Forge version:** 0.1.0

## Summary

Boolean literals are a way to represent truthy and falsy values in an explicit way.

## Syntax specification

```ebnf
boolean_literal ::= "true"
                  | "false"
                  ;
```

EBNF specification above is specified using the [W3C Extensible Markup Language (XML) 1.0 (Fifth Edition)](https://www.w3.org/TR/xml/#sec-notation).

## Behavior

`true` and `false` are not constrained to any particular bit width in terms of valuation. `true` always has the integer value of `1` and `false` always has the integer value of `0`.

## Examples

Here are some examples of usage.

### Valid

These examples are expected to compile successfully.

```
true

false
```
