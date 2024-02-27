<!--
Copyright (c) 2023-2024 Sophie Katz

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

# High priority

- [ ] Add reference types (for internal use)
  - [ ] Create unary type operators
- [ ] Add `void` type
- [ ] Add stack allocation for structs
- [ ] Use `elif` instead of `else if`
- [ ] Add in heap allocation with `new` and `free`
- [ ] Add in array types
- [ ] Add in allocation for arrays
  - [ ] Stack
  - [ ] Heap
- [ ] Add in casting
- [ ] Add in verification logic
  - [ ] Verify that AST is well-formed
  - [x] Add in a way to verify that AST is simplified and can be passed directly to the code generator
  - [ ] Type verification
  - [ ] Constant/mutable verification
  - [ ] Scope verification

# Low priority

- [ ] Look over LLVM codegen and see if there's anything there that should be added to the language
- [ ] Add in test matrix to dockerfile so we can get better arch/OS support **(requires GitHub teams)**

# Backlog

- [x] Add in a feature not yet implemented error and a specific error message to indicate that a requirement or proposal is not yet implemented
  - [ ] Ideally reference a documentation URL
- [ ] When printing errors for AST nodes
  - [x] Print the token range if possible
  - [ ] Otherwise, print the token tree up until the first token range if one exists
- [ ] Make sure all paths are absolute so we don't accidentally open a file twice
- [ ] Reorganize cli and config to be better
- [ ] Use `G_GNUC_WARN_UNUSED_RESULT`
  - [ ] Look into other attributes to use
  - [ ] Output arguments
  - [ ] Unused arguments
- [ ] Add in "operator overloading" for values being passed into functions
- [ ] Make statement and declaration blocks parse more similarly
- [ ] Cross-link documentation to code
- [ ] Use specified streams instead of stdout and stderr to test CLI in integration tests

# Completed

- [x] Organize developer docs
- [x] Write a developer guide for how to make changes to the AST
- [x] Create a plan for how to introduce more advanced testing to the compiler
- [x] Create a plan for how to handle feature flagging **(skipped)**
- [x] Do a review of error handling
- [x] Add code coverage metrics.
- [x] Do a unit test review to see where the most risk of failure is that needs to be tested.
- [x] Write at least one parser integration test.
- [x] Write at least one AST algorithm integration test.
- [x] Add in token reader for lexical casts with locations.
- [x] Add in a message buffer so we can test for errors.
- [x] Add in token ranges
- [x] Add a debug buffer to help with testing
- [x] Add in line printing for errors
- [x] Make errors look like they do in the error message plan
- [x] Add opt-out unicode support to CLI
- [x] Create stream for better testing
- [x] Move `stream.h` into `streams/utilities.h`
- [x] Have AST constructors return their specific type
- [x] Write at least one code generator integration test.
  - [x] See if we can do LLVM IR comparison easier to help with testing
  - [x] Maybe parse LLVM IR from text?
- [x] Implement integer literals of all sizes and signednesses
- [x] Implement boolean literals
- [x] Implement floating point literals of all sizes and signednesses
- [x] Standardize message types
  - [x] Add in error codes
- [x] Add in token ranges for AST nodes
- [x] Create placeholder requirements for missing features
- [x] Add in formatting for at least types
- [x] Enforce all errors start with a capital letter and do not end with punctuation
- [x] Add in memory leak detection into automated tests.
- [x] Refactor code to meet new conventions
  - [x] Declaration style
  - [x] Destructors with single pointers
  - [x] Method functions
  - [x] Private fields
  - [x] Getters and setters
    - [x] Usage
    - [x] Declaration style
  - [x] Recursive structs
  - [x] Forward declarations
    - [x] Circular file dependencies
    - [x] Circular struct dependencies
  - [x] Abbreviations
  - [x] Module name prefixes
  - [x] No nested modules
  - [x] Symbols not just the module name
  - [x] Singular form
  - [x] Global variables
  - [x] Mutable and output function arguments
  - [x] Function argument order
  - [x] Private symbols
  - [x] Match implementation code to headers
    - [x] Make sure all `frg_free` calls are for single pointers
  - [x] Make sure all private symbols have a good reason to be
- [x] Add formatter and linter if possible
- [x] Rename `error.h` to `assert.h`
- [x] Create and conform to naming guidelines
- [x] Find a better way to organize code so that not everything is in the AST module
- [x] Break up AST switch statements
  - [x] Visitor acceptors
  - [x] Type resolvers
  - [x] Formatters
- [x] Create stricter types for IR
- [x] Create a shared compilation function to use for the three now-missing CLI commands
- [x] Create tests for AST kind info coverage
- [x] Create tests for visitor AST kind coverage
