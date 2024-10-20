# Forge Tree

This crate provides the syntax tree for the Forge programming language. It is very language specific.

- [Guide](#guide)
- [Internal structure](#internal-structure)

## Guide

**TODO:** This section needs to be written.

## Internal structure

The internal structure of the library has some complexities, but they are there to achieve these goals:

- **Guaranteed well formed** - Modifying the tree in-place is not allowed and tree nodes can only be created by a builder which enforces a well formed structure by design.
- **Compile-time verification** - Nodes have generic metadata values attached to them so that you can encode different stages of verification and transformation directly in the type system.

See these files for an explanation of how this is implemented:

- [`tree_traits.rs`](./src/tree_traits.rs)
- [`base_structs.rs`](./src/base_structs.rs)
- [`tree_enums.rs`](./src/tree_enums.rs)
