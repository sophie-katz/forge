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

# Naming convention

- [Types](#types)
- [Enum values](#enum-values)
- [Macros](#macros)
- [Global variables and macros](#global-variables-and-macros)
- [Private symbols](#private-symbols)
- [Exceptions to the rules](#exceptions-to-the-rules)
- [A note about C++](#a-note-about-c)
- [Function arguments](#function-arguments)
- [Local variables](#local-variables)

These naming conventions should be used by all symbols in the codebase.

These are general guidelines, but there are exceptions described below:

- All symbols should be prefixed with `frg_`.
- All symbols should be in `snake_case`.
- **All symbols should be descriptive:**
  - Favor longer and more descriptive names over abbreviations.
  - We all have intellisense so abbreviations just make the code less readable with limited benefit.
- **All symbols should be prefixed with their module name:**
  - If a file is in the `forge/ast/**` directory, then the module name is `ast`.
  - So all symbols declared within the `forge/ast/**` directory would have the prefix `frg_ast_`.
  - There are no nested modules, so symbols declared within `forge/ast/kind_info.h` would not have the prefix `frg_ast_kind_info_`.
  - Symbols that only contain the module name are usually not allowed; for example a symbol like `frg_ast` is not allowed.
- **All symbols should be in the singular form**
  - Instead of writing a symbol like `nodes_add`, write `node_add`.
  - This also helps to group related symbols together better.

## Types

Types follow the general guidelines except that they are always suffixed with `_t`. For example:

```c
typedef float frg_f32_t;
```

This is also true of structures and enums:

```c
typedef struct {
    ...
} frg_ast_node_t;

typedef enum {

} frg_ast_kind_t;
```

Symbols that are only the module name and the `_t` suffix are usually not allowed; for example a symbol like `frg_ast_t` is not allowed.

## Enum values

Enum values are always in `SCREAMING_SNAKE_CASE` and begin with the enum name missing the `_t` suffix. For example:

```c
typedef enum {
    FRG_AST_KIND_VALUE_BOOL,
    FRG_AST_KIND_VALUE_INT,
    ...
} frg_ast_kind_t;
```

## Macros

Macros are always in `SCREAMING_SNAKE_CASE` and begin with the module name. For example:

```c
#define FRG_AST_KIND_COUNT ...
```

This does not include functional macros, which follow the same rules as functions:

```c
#define frg_assert(condition) ...
```

## Global variables and macros

Global variables and macros that reference them are always in `SCREAMING_SNAKE_CASE` and begin with the prefix `FRG_<MODULE NAME>_GLOBAL_`. For example:

```c
frg_ast_kind_info_t FRG_AST_GLOBAL_KIND_INFO_TABLE[FRG_AST_KIND_COUNT];
```

## Function arguments

All function arguments are, by default, assumed to either:

- Be passed into the function as constant.
- Be passed into the function as mutable, but the function takes ownership.

If an argument is passed in as a mutable reference, meaning that its value may be modified by the function but it is not taking ownership, then the argument should be prefixed with `mut_`. For example:

```c
void frg_print_something(
    frg_stream_output_t* mut_output,
);
```

If an argument is only used as an additional output for the function, then it should be prefixed with `out_`. For example:

```c
bool frg_parse_character(
    frg_message_buffer_t* mut_message_buffer,
    frg_character_t* out_value,
    frg_parsing_token_reader_t* mut_reader
);
```

### Ordering

Function arguments should be ordered by:

1. Output arguments prefixed with `out_`.
2. Mutable arguments prefixed with `mut_`.
3. Input arguments: values on which the function operates.
4. Configuration arguments: values that change the behavior of the function.

## Private symbols

C does not have the concept of private symbols, so a `_` prefix can be used instead. For example:

```c
GList* _frg_ast_list_clone(GList* list);
```

## Exceptions to the rules

Some symbols can omit the module name prefix if they are ubiquitous to their usage.

- Symbols that are not in a module, such as those defined in `assert.h` can lack the module prefix.
- `frg_parse` is an obvious part of the compilation process that it can safely lack the module name `parsing`.
- etc.

## A note about C++

There is some part of the codebase that is written in C++ (code generation using LLVM). C++ code must follow these same rules for consistency.

## Local variables

While there are intentionally no guidelines for local variables to aid in faster development, it is suggested that variable names be descriptive.

- A variable named `index` provides much for context than a variable named `i`.
- A variable named `current_ast_node` provides much more context than `node`.
