<!--
Copyright (c) 2023 Sophie Katz

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

# FR-7: Operators

This requirement is part of the [Forge language specification](../how-to-use.md). It uses v0 of the requirement template.

It provides an exhaustive list of operators in the language and their behavior.

## Description

These are the operators in the language:
- [`.`](#access)
- [`()`](#call)
- [`~`](#bitwise-not)
- [`&`](#bitwise-and)
- [`|`](#bitwise-or)
- [`^`](#bitwise-exclusive-or)
- [`<<`](#bitwise-left-shift)
- [`>>`](#bitwise-right-shift)
- [`-`](#arithmetic-negation)
- [`+`](#arithmetic-addition)
- [`-`](#arithmetic-subtraction)
- [`*`](#arithmetic-multiplication)
- [`/`](#arithmetic-division)
- [`//`](#arithmetic-floor-division)
- [`%`](#arithmetic-modulus)
- [`**`](#arithmetic-exponentiation)
- [`==`](#arithmetic-equal-to)
- [`!=`](#arithmetic-not-equal-to)
- [`<`](#arithmetic-less-than)
- [`<=`](#arithmetic-less-than-or-equal-to)
- [`>`](#arithmetic-greater-than)
- [`>=`](#arithmetic-greater-than-or-equal-to)
- [`!`](#logical-not)
- [`&&`](#logical-and)
- [`||`](#logical-or)
- [`=`](#assignment)
- [`&=`](#bitwise-and-then-assign)
- [`|=`](#bitwise-or-then-assign)
- [`^=`](#bitwise-exclusive-or-then-assign)
- [`<<=`](#bitwise-left-shift-then-assign)
- [`>>=`](#bitwise-right-shift-then-assign)
- [`+=`](#arithmetic-addition-then-assign)
- [`++`](#arithmetic-increment)
- [`-=`](#arithmetic-subtraction-then-assign)
- [`--`](#arithmetic-decrement)
- [`*=`](#arithmetic-multiplication-then-assign)
- [`/=`](#arithmetic-division-then-assign)
- [`//=`](#arithmetic-floor-division-then-assign)
- [`%=`](#arithmetic-modulus-then-assign)
- [`**=`](#arithmetic-exponentiation-then-assign)
- [`&&=`](#logical-and-then-assign)
- [`||=`](#logical-or-then-assign)

### Access: `A.B`

`B` must be a symbol that is a member of `A`, which must have a structure or interface type. It evaluates into a reference to that member of `A`.

### Call `A(B, C, ...)`

`A` must have a functional type. This expression evaluates by calling the function with `B`, `C`, ... as arguments.

### Bitwise not `~A`

`A` must have an integral type. This expression evaluates by performing a bitwise not on `A`. It can also be overloaded by interfaces.

### Bitwise and `A & B`

`A` must have an integral type. This expression evaluates by performing a bitwise and on `A` and `B`. It can also be overloaded by interfaces.

### Bitwise or `A | B`

`A` must have an integral type. This expression evaluates by performing a bitwise or on `A` and `B`. It can also be overloaded by interfaces.

### Bitwise exclusive or `A ^ B`

`A` must have an integral type. This expression evaluates by performing a bitwise exclusive or on `A` and `B`. It can also be overloaded by interfaces.

### Bitwise left shift `A << B`

`A` must have an integral type. This expression evaluates by performing a bitwise left shift on `A` and `B`. It can also be overloaded by interfaces.

### Bitwise right shift `A >> B`

`A` must have an integral type. This expression evaluates by performing a bitwise right shift on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic negation `-A`

`A` must have an integral or floating point type. This expression evaluates by performing an arithmetic negation on `A`. It can also be overloaded by interfaces.

### Arithmetic addition `A + B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic addition on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic subtraction `A - B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic subtraction on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic multiplication `A * B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic multiplication on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic division `A / B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic division on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic floor division `A // B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic floor division on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic modulus `A % B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic modulus on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic exponentiation `A ** B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic exponentiation on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic equal to `A == B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic equal to on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic not equal to `A != B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic not equal to on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic less than `A < B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic less than on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic less than or equal to `A <= B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic less than or equal to on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic greater than `A > B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic greater than on `A` and `B`. It can also be overloaded by interfaces.

### Arithmetic greater than or equal to `A >= B`

`A` and `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic greater than or equal to on `A` and `B`. It can also be overloaded by interfaces.

### Logical not `!A`

`A` must have a boolean type. This expression evaluates by performing a logical not on `A`. It can also be overloaded by interfaces.

### Logical and `A && B`

`A` and `B` must have a boolean type. This expression evaluates by performing a logical and on `A` and `B`. It can also be overloaded by interfaces.

### Logical or `A || B`

`A` and `B` must have a boolean type. This expression evaluates by performing a logical or on `A` and `B`. It can also be overloaded by interfaces.

### Assignment `A = B`

`A` must be a reference to a variable or member of a structure or interface. `B` must be assignable to `A`. This expression evaluates by assigning `B` to `A`.

### Bitwise and then assign `A &= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral type. This expression evaluates by performing a bitwise and on `A` and `B`, then assigning the result to `A`.

### Bitwise or then assign `A |= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral type. This expression evaluates by performing a bitwise or on `A` and `B`, then assigning the result to `A`.

### Bitwise exclusive or then assign `A ^= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral type. This expression evaluates by performing a bitwise exclusive or on `A` and `B`, then assigning the result to `A`.

### Bitwise left shift then assign `A <<= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral type. This expression evaluates by performing a bitwise left shift on `A` and `B`, then assigning the result to `A`.

### Bitwise right shift then assign `A >>= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral type. This expression evaluates by performing a bitwise right shift on `A` and `B`, then assigning the result to `A`.

### Arithmetic addition then assign `A += B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic addition on `A` and `B`, then assigning the result to `A`.

### Arithmetic increment `A++`

`A` must be a reference to a variable or member of a structure or interface. `A` must have an integral or floating point type. This expression evaluates by performing an arithmetic increment on `A`.

### Arithmetic subtraction then assign `A -= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic subtraction on `A` and `B`, then assigning the result to `A`.

### Arithmetic decrement `A--`

`A` must be a reference to a variable or member of a structure or interface. `A` must have an integral or floating point type. This expression evaluates by performing an arithmetic decrement on `A`.

### Arithmetic multiplication then assign `A *= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic multiplication on `A` and `B`, then assigning the result to `A`.

### Arithmetic division then assign `A /= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic division on `A` and `B`, then assigning the result to `A`.

### Arithmetic floor division then assign `A //= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic floor division on `A` and `B`, then assigning the result to `A`.

### Arithmetic modulus then assign `A %= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic modulus on `A` and `B`, then assigning the result to `A`.

### Arithmetic exponentiation then assign `A **= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have an integral or floating point type. This expression evaluates by performing an arithmetic exponentiation on `A` and `B`, then assigning the result to `A`.

### Logical and then assign `A &&= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have a boolean type. This expression evaluates by performing a logical and on `A` and `B`, then assigning the result to `A`.

### Logical or then assign `A ||= B`

`A` must be a reference to a variable or member of a structure or interface. `B` must have a boolean type. This expression evaluates by performing a logical or on `A` and `B`, then assigning the result to `A`.

## Related

- [FR-8: Operator precedence](./FR-8-operator-precedence.md)
