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

<!--
Follow any instructions prefixed with 'CHANGE:' and then remove the comments (including this one).
-->

# Requirement-13: Numeric containing types

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

**Start date:** 2024-01-20

**Fully supported as of Forge version:** 0.1.0

## Summary

When two numeric types are passed to binary operators, the result must be a type that is capable of holding the result of the operation without truncating.

## Description

This behavior applies only to integer and floating point types. Let `A` and `B` be two numeric types. Let `R` be a type that is capable of holding either value without truncating:

- If `A` and `B` are both unsigned integers:

  - If either of `A` or `B` are 64 bits, then `R` is `u64`

  - Otherwise, `R` is `u32`

- If `A` and `B` are both signed integers:

  - If either of `A` or `B` are 64 bits, then `R` is `i64`

  - Otherwise, `R` is `i32`

- If `A` is signed and `B` is unsigned:

  - If both are 64 bits, then `R` is `i64` but it is recommended that a truncation warning is emitted

  - If `B` is 32 bits, then `R` is `i64`

  - If `A` is 64 bits and `B` is 16 bits or less, then `R` is `i64`

  - If `A` is 32 bits or less and `B` is 16 bits or less, then `R` is `i32`

- If `A` is unsigned and `B` is signed, see the previous case but swap `A` and `B`

- If `A` and `B` are both floating points:

  - If either of `A` or `B` are 64 bits, then `R` is `f64`

  - Otherwise, `R` is `f32`

- If `A` is floating point and `B` is an integer, then `R` is `f64`

- If `A` is an integer and `B` is floating point, then `R` is `f64`
