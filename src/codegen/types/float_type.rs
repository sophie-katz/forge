use inkwell::types::FloatType;

use crate::{domain::ValueWidth, parsing::ast::TypeBasic};

use super::super::{context::CodeGenerationContext, error::ErrorCodeGeneration};

pub enum CodeGeneratorFloatType<'input> {
    FromByteCount(usize),
    FromValueWidth(&'input ValueWidth),
    FromTypeBasic(&'input TypeBasic),
}

impl<'input> CodeGeneratorFloatType<'input> {
    pub fn generate<'llvm_context>(
        self,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<FloatType<'llvm_context>, ErrorCodeGeneration> {
        match self {
            Self::FromByteCount(bytes) => {
                Self::generate_from_byte_count(bytes, code_generation_context)
            }
            Self::FromValueWidth(value_width) => {
                Self::generate_from_value_width(value_width, code_generation_context)
            }
            Self::FromTypeBasic(r#type) => {
                Self::generate_from_type_basic(r#type, code_generation_context)
            }
        }
    }

    fn generate_from_byte_count<'llvm_context>(
        bytes: usize,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<FloatType<'llvm_context>, ErrorCodeGeneration> {
        match bytes {
            4 => Ok(code_generation_context.llvm_context.f32_type()),
            8 => Ok(code_generation_context.llvm_context.f64_type()),
            _ => Err(ErrorCodeGeneration::UnsupportedByteWidth(bytes)),
        }
    }

    fn generate_from_value_width<'llvm_context>(
        value_width: &'input ValueWidth,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<FloatType<'llvm_context>, ErrorCodeGeneration> {
        match value_width {
            ValueWidth::FixedByteSize(bytes) => {
                Self::generate_from_byte_count(*bytes, code_generation_context)
            }
            ValueWidth::PointerSize => Err(ErrorCodeGeneration::UnsupportedAst {
                description: "pointer size for float".to_owned(),
            }),
            ValueWidth::Auto => Err(ErrorCodeGeneration::UnsupportedAst {
                description: "auto width for float".to_owned(),
            }),
        }
    }

    fn generate_from_type_basic<'llvm_context>(
        r#type: &'input TypeBasic,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<FloatType<'llvm_context>, ErrorCodeGeneration> {
        match r#type {
            TypeBasic::Float(value_width) => {
                Self::generate_from_value_width(value_width, code_generation_context)
            }
            _ => Err(ErrorCodeGeneration::UnsupportedAst {
                description: "unsupported type".to_owned(),
            }),
        }
    }
}
