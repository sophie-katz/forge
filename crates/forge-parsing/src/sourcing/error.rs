use thiserror::Error;

/// A general-purpose error type used when reading source code.
#[derive(Debug, Error)]
pub enum ErrorSource {
    /// A wrapper around `io::Error`.
    #[error("io error: {0}")]
    Io(#[from] std::io::Error),

    /// A wrapper around `unicode_segmentation::GraphemeIncomplete`.
    ///
    /// This error may be returned when invalid UTF-8 is encountered but should never be encountered
    /// during normal use.
    #[error("unicode segmentation error")]
    UnicodeSegmentation(unicode_segmentation::GraphemeIncomplete),

    /// A wrapper around `str::Utf8Error`.
    ///
    /// This error may be returned when invalid UTF-8 is encountered but should never be encountered
    /// during normal use.
    #[error("UTF-8 error: {0}")]
    Utf8(#[from] std::str::Utf8Error),

    /// Paths to files cannot be empty strings.
    ///
    /// Use `"--"` instead to indicate that the source code wasn't read from a file on the
    /// filesystem.
    #[error("empty path")]
    EmptyPath,

    /// Line numbers start at 1 and not 0.
    #[error("line number cannot be zero")]
    LineNumberCannotBeZero,

    /// Column numbers start at 1 and not 0.
    #[error("column number cannot be zero")]
    ColumnNumberCannotBeZero,

    /// An error that is returned when you try to read past the end of the source code.
    ///
    /// This is a common error that occurs during normal use.
    #[error("no more to be read")]
    NoMoreToBeRead,

    /// An error that indicates that a byte offset is in the middle of a grapheme cluster.
    #[error("byte offset {0} is in the middle of a grapheme cluster")]
    ByteOffsetInMiddleOfGraphemeCluster(usize),
}
