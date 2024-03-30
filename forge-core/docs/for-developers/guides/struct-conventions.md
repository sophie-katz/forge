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

# Struct conventions

- [Constructors and destructors](#constructors-and-destructors)
- ["Method" functions](#method-functions)
- [Private fields](#private-fields)
- [Getters and setters](#getters-and-setters)
- [Recursive structs](#recursive-structs)
- [Forward declarations](#forward-declarations)
  - [Circular file dependencies](#circular-file-dependencies)
  - [Circular struct dependencies](#circular-struct-dependencies)
- [Non-owned fields](#non-owned-fields)
- [Opaque structs](#opaque-structs)

Structures should use this declaration style:

```c
typedef struct {
    ...
} frg_struct_name_t;
```

See [Naming convention](naming-convention.md) for details on how to name structs.

## Constructors and destructors

All structs (if they will be stored on the heap) need constructors and destructors. They should all be structured like this:

```c
typedef struct {
    ...
} frg_a_t;

frg_a_t* frg_a_new();

void frg_a_destroy(frg_a_t* a);
```

## "Method" functions

Functions that operate on values of the type of a given struct should be prefixed with the struct's name:

```c
typedef struct {
    ...
} frg_a_t;

void frg_a_do_something(frg_a_t* a);
```

Arguments should be made `const` when they do not modify the value. Although this type rule can be easily broken in C, it's still helpful for self-documenting code.

## Private fields

If a given field may be accessed directly, it should have no prefix.

If a given field should only be accessed through ["method" functions](#method-functions), it should be prefixed with an `_`.

## Getters and setters

Getters and setters should be avoided at all costs. Just use [public fields](#private-fields). The only case when they should be used is for [recursive structs](#recursive-structs) and [circular struct dependencies](#circular-struct-dependencies).

They should be declared like this:

```c
typedef struct {
    int x;
    frg_b_t* y;
} frg_a_t;

int frg_a_get_x(const frg_a_t* a);

void frg_a_set_x(frg_a_t* mut_a, int x);

frg_b_t* frg_a_get_y(frg_a_t* mut_a);

void frg_a_set_y(frg_a_t* mut_a, frg_b_t* y);
```

Notice that `a` is `const` in the getter for `x` because it's being returned by value, but is mutable in the getter for `y` because the return value can be modified.

## Recursive structs

If a struct needs to contain itself as a member, it should use this declaration style:

```c
typedef struct frg_struct_name_t {
    struct frg_struct_name_t *recursive;
    ...
} frg_struct_name_t;
```

In this case, make sure to provide [getters and setters](#getters-and-setters) if needed to ensure that recursive members are not accessed directly with the wrong types:

```c
frg_struct_name_t* frg_struct_name_recursive_get(
    frg_struct_name_t* struct_name
);

void frg_struct_name_recursive_set(
    frg_struct_name_t* struct_name,
    frg_struct_name_t* recursive
);
```

Otherwise, `a->b` would have the type `struct frg_b_t*` which wouldn't match `frg_b_t*` for all other uses of the property.

## Forward declarations

### Circular file dependencies

If there is a circular dependency between struct `frg_a_t` defined in file `a.h` and struct `frg_b_t` defined in file `b.h`, then one of the files should be split into `a.h` and `a_forward.h`. `a_forward.h` should just contain the `frg_a_t` struct declaration and nothing else. Then `b.h` can include `a_forward.h` and `a.h` can include `b.h`.

For example, this could be `a.h` **before the fix:**

```c
#pragma once

#include <forge/b.h>

typedef struct {
    ...
} frg_a_t;

frg_a_t* frg_a_new();

void frg_a_destroy(frg_a_t* a);

void frg_a_do_something(frg_a_t* a, const frg_b_t* b);

...
```

And this could be `b.h` **before the fix:**

```c
#pragma once

#include <forge/a.h>

typedef struct {
    frg_a_t* a;
} frg_b_t;

...
```

To fix this, we would split `a.h` into `a_forward.h`:

```c
#pragma once

typedef struct {
    ...
} frg_a_t;
```

And then `a.h` would become:

```c
#pragma once

#include <forge/a_forward.h>
#include <forge/b.h>

frg_a_t* frg_a_new();

void frg_a_destroy(frg_a_t* a);

void frg_a_do_something(frg_a_t* a, const frg_b_t* b);

...
```

And `b.h` would become:

```c
#pragma once

#include <forge/a_forward.h>

typedef struct {
    frg_a_t* a;
} frg_b_t;

...
```

## Circular struct dependencies

If you have two structs that each depend on each other, then you should declare them in the same `*_forward.h` file like so:

```c
struct frg_a_t;
struct frg_b_t;

typedef struct {
    struct frg_b_t* _b;
} frg_a_t;

typedef struct {
    struct frg_a_t* _a;
} frg_b_t;
```

In this case, make sure to provide [getters and setters](#getters-and-setters) if needed to ensure that these members are not accessed directly with the wrong types:

```c
frg_b_t* frg_a_b_get(frg_a_t* a);
void frg_a_b_set(frg_a_t* a, frg_b_t* b);
```

Otherwise, `a->b` would have the type `struct frg_b_t*` which wouldn't match `frg_b_t*` for all other uses of the property.

# Non-owned fields

If a given field is not owned by the struct, it should be prefixed with `mut_`.

# Opaque structs

If you need to hide the implementation of a struct, you should use an opaque struct.

```c
typedef struct frg_struct_name_t frg_struct_name_t;
```
