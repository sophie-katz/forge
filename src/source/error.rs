use thiserror::Error;

#[derive(Debug, Error)]
pub enum SourceError {
    #[error("empty path")]
    EmptyPath,
    #[error("line number cannot be zero")]
    LineNumberCannotBeZero,
}
