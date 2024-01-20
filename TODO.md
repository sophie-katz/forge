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

- [ ] Standardize message types
    - [ ] Add in error codes
- [ ] Create placeholder requirements for missing features
- [ ] Add in verification logic
- [ ] Add in a way to verify that AST is simplified and can be passed directly to the code generator

# Low priority

- [ ] Look over LLVM codegen and see if there's anything there that should be added to the language
- [ ] Use `elif` instead of `else if`
- [ ] Add in test matrix to dockerfile so we can get better arch/OS support
- [ ] Create a shared compilation function to use for the three now-missing CLI commands

# Backlog

- [x] Add in a feature not yet implemented error and a specific error message to indicate that a requirement or proposal is not yet implemented
    - [ ] Ideally reference a documentation URL
- [ ] Add in token ranges for AST nodes
- [ ] When printing errors for AST nodes
    - [ ] Print the token range if possible
    - [ ] Otherwise, print the token tree up until the first token range if one exists
- [ ] Add in memory leak detection into automated tests.
- [ ] Add formatter and linter if possible
- [ ] Enforce all errors start with a capital letter and do not end with punctuation
- [ ] Make sure all paths are absolute so we don't accidentally open a file twice
- [ ] Use GLib 2.0 destructs better
- [ ] Reorganize cli and config to be better
- [ ] Use `G_GNUC_WARN_UNUSED_RESULT`
    - [ ] Look into other attributes to use
- [ ] Add in "operator overloading" for values being passed into functions

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