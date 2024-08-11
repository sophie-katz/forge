use thiserror::Error;

#[derive(Debug, Error)]
pub enum ErrorSource {
    #[error("empty path")]
    EmptyPath,
    #[error("line number cannot be zero")]
    LineNumberCannotBeZero,
    #[error("io error: {0}")]
    Io(#[from] std::io::Error),
}
