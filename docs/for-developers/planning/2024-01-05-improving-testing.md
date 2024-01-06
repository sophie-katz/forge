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

# Goals

Right now, there is very minimal testing in the compiler. Most of it is done manually. This is good in that it speeds up initial development, but it is horrible for actually having a working compiler. Trust me I am aware of how bad practice it is. This is my plan to improve the situation.

## Unit testing

We're already using Unity for our unit tests, but there's nothing to ensure that pieces of the code are unit tested well or not. I would like to:

- Add code coverage metrics.
- Do a unit test review to see where the most risk of failure is that needs to be tested.

## Integration testing

The main types of integration testing I can think of are:

- **Parser testing:** Parse strings into AST and then compare against expected AST or expected errors.

- **AST algorithm testing:** Run various algorithms (like type resolution) on AST and compare against expected results.

- **Code generation testing:** Generate code from AST and then compare against expected output.

## Performance testing

Ideally, performance testing would tell us:

- Whether there are any memory leaks.
- How many system resources is being consumed during compilation.
- Where the bottlenecks are in the compiler.
- How performant the compiled code is.
    - Standard benchmarks might be a great place to start here.

## End-to-end testing

End-to-end testing should be run on the `forge` executable as a whole. It should take sample programs and compile them before comparing them to expected results. This is the most important type of testing because it ensures that the compiler is working as a whole.

## Fuzz testing

We'd need a Forge code fuzzer for this to work. It's definitely a further down the road type of thing.

## In-language testing

Once we get more code written in Forge, we can use that to test the compiler.

# Tasks

Turning the above goals into something actionable, here is an approximate todo list for improving testing:

- [ ] Add code coverage metrics.
- [ ] Do a unit test review to see where the most risk of failure is that needs to be tested.
- [ ] Write at least one parser integration test.
- [ ] Write at least one AST algorithm integration test.
- [ ] Write at least one code generator integration test.
- [ ] Add in memory leak detection into automated tests.

There's plenty more to do, but this would make a great start.
