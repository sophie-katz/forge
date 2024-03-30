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

This should be a basic milestone for simplified Forge.

```
fn and(a: bool, b: bool) -> bool {
    return a && b;
}

fn add(a: i32, b: i32) -> i32 {
    return a + b;
}

fn fadd(a: f32, b: i32) -> f32 {
    return a + b;
}

fn add_pointer(a: *i32, b: i32) -> void {
    *a = *a + b;
}

ty A {
    a: i32,
    b: f64
}

let a = const A {
    a: 1,
    b: 2.0
};

fn max(a: i64, b: i64) -> i64 {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

fn power(base: i32, exponent: i32) {
    let result = 1;
    let i = 0;

    while (i < exponent) {
        result = result * base;
        i = i + 1;
    }

    return result;
}

fn fib(n: i32) -> i32 {
    if (n > 2) {
        return fib(n - 1) + fib(n - 2);
    } elif (n == 1) {
        return 1;
    } else {
        return 1;
    }
}

fn hello_world_array() -> [12: u8] {
    return [72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100, 0];
}

fn hello_world_pointer() -> *u8 {
    let result: *u8 = new [12: u8];

    *result = 72;
    *(result + 1) = 101;
    *(result + 2) = 108;
    *(result + 3) = 108;
    *(result + 4) = 111;
    *(result + 5) = 32;
    *(result + 6) = 87;
    *(result + 7) = 111;
    *(result + 8) = 114;
    *(result + 9) = 108;
    *(result + 10) = 100;
    *(result + 11) = 0;

    return result;
}

fn free_hello_world_pointer(pointer: *u8) -> void {
    free pointer;
}

fn casting() -> void {
    let a: [u8: 100];

    let a_as_A = &a as *A;
}
```
