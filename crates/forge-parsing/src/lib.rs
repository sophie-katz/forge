//! # Forge Parsing library
//!
//! Language-agnostic parsing library that is used for Forge. You can see it in use in the
//! `forge-compiler` crate.

#![warn(missing_docs)]

pub mod lexing;
pub mod parsing;
pub mod sourcing;
mod types;

pub use types::*;
