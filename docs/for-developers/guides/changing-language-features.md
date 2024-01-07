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

# Changing language features

Changing even simple language features often results in many changes in many places in the codebase. These are all of the places that you may have to update:

- [AST](#ast)
    - [ID enum](#id-enum)
    - [Structs](#structs)
    - [Debug printing](#debug-printing)
    - [Type resolution](#type-resolution)
- [Code generation](#code-generation)
- [Parsing](#parsing)
    - [Lexer](#lexer)
    - [Parser](#parser)

## AST

### ID enum

The ID enumerator is the list of all the different types of AST nodes that can exist. This is defined in the [`forge/common/enums.h`](../../../forge-core/include/forge/common/enums.h) file. See the `frg_ast_id_t` enumerator for details.

### Structs

The structures are stored in [`forge/ast/ast.h`](../../../forge-core/include/forge/ast/ast.h). Make sure to update any relevant functions as well as the structs.

### Debug printing

Debug printing is implemented in [`forge/ast/debug.h`](../../../forge-core/include/forge/ast/debug.h).

### Type resolution

Debug printing is implemented in [`forge/ast/type_resolution.h`](../../../forge-core/include/forge/ast/type_resolution.h).

## Code generation

The generators are defined in [`forge/codegen/generators.hpp`](../../../forge-core/lib/forge/codegen/generators.hpp).

## Parsing

### Lexer

The lexer is defined in [`forge/parsing/forge.l`](../../../forge-core/lib/forge/parsing/forge.l).

### Parser

The parser is defined in [`forge/parsing/forge.y`](../../../forge-core/lib/forge/parsing/forge.y).
