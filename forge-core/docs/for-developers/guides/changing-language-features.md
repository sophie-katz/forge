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

# Changing language features

Changing even simple language features often results in many changes in many places in the codebase. These are all of the places that you may have to update:

- [Testing](#testing)
- [AST](#ast)
  - [Node kind enumerator](#node-kind-enumerator)
  - [Structs](#structs)
  - [Node kind info](#node-kind-info)
- [Verification](#verification)
- [Code generation](#code-generation)
- [Parsing](#parsing)
  - [Lexer](#lexer)
  - [Parser](#parser)

## Testing

Compilation tests must be written for any new language features. These are defined in [`forge/tests/compilation/`](../../../tests/compilation/).

> [!IMPORTANT]  
> Tests should always be written first!

## AST

### Node kind enumerator

The node kind enumerator is the list of all the different kinds of AST nodes that can exist. This is defined in the [`forge/enums.h`](../../../include/forge/enums.h) file. See the `frg_ast_node_kind_t` enumerator for details.

### Structs

The structures are stored in [`forge/ast/node.h`](../../../include/forge/ast/node.h). Make sure to update any relevant functions as well as the structs.

### Node kind info

Nodes kinds have metadata associated with them, which is defined in [`forge/ast/node_kind_info.h`](../../../include/forge/ast/node_kind_info.h).

This may involve changing functions associated with the relevant node kinds, which may live in other modules.

## Verification

Verification code is defined in [`forge/verification/`](../../../lib/forge/verification/).

## Code generation

The generators are defined in [`forge/codegen/generators.hpp`](../../../lib/forge/codegen/generators.hpp).

## Parsing

### Lexer

The lexer is defined in [`forge/parsing/forge.l`](../../../lib/forge/parsing/forge.l).

### Parser

The parser is defined in [`forge/parsing/forge.y`](../../../lib/forge/parsing/forge.y).
