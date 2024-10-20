//! Shared types used throughout Forge.

/// The id of a source, used to discriminate between loaded sources.
pub type SourceId = usize;

/// The byte offset in a source.
pub type SourceByteOffset = usize;

/// The line number in a source.
pub type LineNumber = u32;

/// The column number in a source.
pub type ColumnNumber = u32;
