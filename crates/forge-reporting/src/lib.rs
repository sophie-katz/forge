//! # Forge Reporting library
//!
//! This is the library that contains the reporting system for the Forge language. It is used to
//! report errors and warnings throughout the Forge compiler.

#![warn(missing_docs)]

mod context;
mod rendering;
mod report;
mod unicode;

pub use context::ReportingContext;
pub use report::{Report, ReportCode, Severity};
