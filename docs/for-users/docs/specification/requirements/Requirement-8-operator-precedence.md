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

# Requirement-8: Operator precedence

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

**Start date:** 2024-01-05
**Fully supported as of Forge version:** 0.1.0

It defines the precedence, or order of operations, for the operators in the language.

## Background

Forge's list of operators is relatively similar to Python's. For reference: https://docs.python.org/3/reference/expressions.html#operator-precedence. This order of operations is based heavily on Python.

## Description

Precedence | Operators
---------- | ---------
15         | `.`, `()`
14         | `++`, `--`
13         | `**`
12         | `-` (unary), `~`
11         | `*`, `/`, `//`, `%`
10         | `+`, `-` (binary)
9          | `<<`, `>>`
8          | `&`
7          | `^`
6          | <code>&#124;</code>
5          | `==`, `!=`, `<`, `<=`, `>`, `>=`
4          | `!`
3          | `&&`
2          | <code>&#124;&#124;</code>
1          | `=`, `&=`, <code>&#124;=</code>, `^=`, `<<=`, `>>=`, `+=`, `-=`, `*=`, `/=`, `//=`, `%=`, `**=`, `&&=`, <code>&#124;&#124;=</code>

>[!NOTE]  
> Higher precedence numbers are more tightly binding.

## Examples

Here are some examples of usage.

```
a.b() # a.b is resolved, then the result is called

a().b() # a() is resolved, the member b is found on that result, then the final result is called

x++ * 2 # the result is x multiplied by 2, then x is incremented after this expression is evaluated

x && y || z # the result is x and y, then that result or z
```

## Related

- [Requirement-7: Operators](./Requirement-7-operators.md)
