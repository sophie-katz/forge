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

# Better developer experience

Right now, it's really hard to write code in the Forge compiler. It feels like pulling your teeth out. What can be done to improve the developer experience?

**Problems:**

- [Creating error codes is hard](#simplifying-error-codes)
- [Doing the same repeated work for many AST kinds is hard](#repeated-work-for-ast-kinds)
- [Memory safety requires lots of cognitive overhead](#memory-safety)
- Lots of simple behavior changes require lots of code changes
- Difficult to consider edge cases
- Difficult to consider consequences of code changes

## Simplifying error codes

It's not that writing a particular error code is complicated, it's that often when writing new code you need to write a bunch of them. And it's hard to consider how to write error codes that will be generic enough that you don't have to write too many.

One solution would be to create a macro that aborts execution and can act as a placeholder for future errors. This would allow for faster development of code that can be handled before it gets to production.

## Repeated work for AST kinds

Providing a macro that aborts execution to act as a placeholder for future support might help.

## Memory safety

Adding in memory leak checking to automated tests would remove the pressure of having to focus on this during development as much.
