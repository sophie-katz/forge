use thiserror::Error;

#[derive(Debug, Error)]
pub enum ErrorCodeGeneration {
    #[error("unsupported target architecture: {0}")]
    UnsupportedTargetArchitecture(String),

    #[error("unsupported byte width: {0}")]
    UnsupportedByteWidth(usize),

    #[error("unsupported ast in code generation: {description}")]
    UnsupportedAst { description: String },

    #[error("undeclared variable: {0}")]
    UndeclaredVariable(String),

    #[error("variable already declared: {0}")]
    VariableAlreadyDeclared(String),
}
