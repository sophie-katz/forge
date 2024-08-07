# Forge

## Comments

```
// This is a comment

/* This is also a comment */
```

Keep things similar to C because that's what most people are familiar with.

## Literals

String and character literals can be prefixed with these flags in any order:

- `b` return the value as a byte array instead of a unicode string
- `r` do not escape backslashes (`$` can still be escaped with `$$`)
- `u` do not allow format strings

```
// Booleans
true
false

// Integers
5
0b101
0o5
0x5
1_000
5i64
5u8

// Floats
5.0
0b101.0
0o5.0
0x5.f
5.0e1
5.0e-1
0x5.fea
5_000.0
5.0f32
5.0f64

// Characters
'a'
'\n'
'\x1f'
'\u{2530}'
b'a'

// Strings
"hello, world"
b"hello, world"

// Multi-line strings
"all strings
are multiline by default"

// Custom quotes
#"this string can contain " characters "#
#some_symbol"this string can contain " characters "some_symbol#

// Raw strings
r"this string doesn't escape \ characters"

// Format strings
"${x} all strings are format strings by default"
u"use u to disable this"

// All at once
rbu#"this string does not allow formatting or escapes and is returned as a byte array.

it is multiline and can contain " characters."#
```

## Basic types

- `bool`
- `i8`, `i16`, `i32`, `i64`, `isize`
- `int` (auto-detect width to prevent overflow, defaults to `i32`)
- `u8`, `u16`, `u32`, `u64`, `usize`
- `uint` (auto-detect width to prevent overflow, defaults to `u32`)
- `f32`, `f64`
- `void`
- `never`
- `null`

## Casting

```
5 as float
```

## Alias types

You can declare types that are aliases for other types.

```
type ibyte = i8;
```

These are are implicitly castable between their base type. If you want to prevent this, use:

```
explicit type char = u32;
```

These are the alias types that are declared by default:

```
type ibyte = i8;
type byte = u8;
type float = f64;
explicit type char = u32;
```

## Enums

```
// These values do not have any internal representation and exist symbolically only
enum Color {
    Red,
    Green,
    Blue,
}

// You can access the value with:
Color.Red

// You can get an array of the items with:
Color.items

// These values have an internal representation
enum Color: string {
    Red = "red",
    Green = "green",
    Blue = "blue",
}

Color.Red == "red"
typeof Color.Red == Color
Color.Red.value == "red"
typeof Color.Red.value == string

Color.items == [Color.Red, Color.Green, Color.Blue]
Color.values == ["red", "green", "blue"]

// You can also have integer enums which support auto-incremented ids and arithmetic
enum Step: i32 {
    Start,
    Middle,
    End,
}

Step.Start == 0
Step.Start + 1 == Step.Middle
```

## Variables

```
let x = 5;

let y: string = "hello, world";

const z = 5;
```

## Global variables

```
const X = 5;

// This can be modified at runtime
let Y: string = "hello, world";
```

## Boolean

The boolean type can be represented like this, even though it's built in (note that it's lowercase):

```
enum bool {
    true,
    false
}

const true = bool.true;

const false = bool.false;
```

## Functions

```
fn add(x: int, y: int) -> int {
    return x + y;
}

// A missing return type means to return void
fn add(x: int, y: int) {
    return;
}
```

## Classes

```
type byte = u8;

// You can create bindings for an existing type to extend it
explicit type char = u32 -> {
    prop bytes(self) -> [4:byte] {
        return [self >> 24, self >> 16, self >> 8, self];
    }
};

'a'.bytes == [0, 0, 0, 97];

// You can create union types
type IntOrString = int | String;

let x: IntOrString = 5;
let y: IntOrString = "hello, world";

if x is int {
    ...
}

// You can create structures
type Point = {
    x: int;
    y: int;
};

let p: Point = { x: 5, y: 10 };
p.x == 5;

// And you can create structures with methods
type Rect = {
    x: int;
    y: int;
    width: int;
    height: int;

    prop area(self) -> int {
        return self.width * self.height;
    }
};

// And you can extend existing types using your own
type MyRect = Rect -> {
    prop perimeter(self) -> int {
        return self.width * 2 + self.height * 2;
    }
};

// You can deconstruct and reconstruct types
let r: Rect = { x: 2, y: 3, width: 5, height: 10 };

let p: Point = { ...r };

p.x == 2;

let r2: Rect = { ...p, width: 5, height: 10 };

let { height } = r2;

height == 10;

// You can overload operators
type Point = {
    x: int;
    y: int;

    fn "+" (self, other: Point) -> Point {
        return { x: self.x + other.x, y: self.y + other.y };
    }
};

// You can create constructs and destructors
type Point = {
    x: int;
    y: int;
};
```
