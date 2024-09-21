use inkwell::types::IntType;

use crate::{domain::ValueWidth, parsing::ast::TypeBasic};

use super::super::{context::CodeGenerationContext, error::ErrorCodeGeneration};

pub enum CodeGeneratorIntType<'input> {
    FromByteCount(usize),
    FromValueWidth(&'input ValueWidth),
    FromTypeBasic(&'input TypeBasic),
}

impl<'input> CodeGeneratorIntType<'input> {
    pub fn generate<'llvm_context>(
        self,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<IntType<'llvm_context>, ErrorCodeGeneration> {
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
    ) -> Result<IntType<'llvm_context>, ErrorCodeGeneration> {
        match bytes {
            1 => Ok(code_generation_context.llvm_context.i8_type()),
            2 => Ok(code_generation_context.llvm_context.i16_type()),
            4 => Ok(code_generation_context.llvm_context.i32_type()),
            8 => Ok(code_generation_context.llvm_context.i64_type()),
            _ => Err(ErrorCodeGeneration::UnsupportedByteWidth(bytes)),
        }
    }

    fn generate_from_value_width<'llvm_context>(
        value_width: &'input ValueWidth,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<IntType<'llvm_context>, ErrorCodeGeneration> {
        match value_width {
            ValueWidth::FixedByteSize(bytes) => {
                Self::generate_from_byte_count(*bytes, code_generation_context)
            }
            ValueWidth::PointerSize => Self::generate_from_byte_count(
                code_generation_context.pointer_width(),
                code_generation_context,
            ),
            ValueWidth::Auto => Err(ErrorCodeGeneration::UnsupportedAst {
                description: "auto width for integer".to_owned(),
            }),
        }
    }

    fn generate_from_type_basic<'llvm_context>(
        r#type: &'input TypeBasic,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<IntType<'llvm_context>, ErrorCodeGeneration> {
        match r#type {
            TypeBasic::Bool => Ok(code_generation_context.llvm_context.bool_type()),
            TypeBasic::IntUnsigned(value_width) | TypeBasic::IntSigned(value_width) => {
                Self::generate_from_value_width(value_width, code_generation_context)
            }
            _ => Err(ErrorCodeGeneration::UnsupportedAst {
                description: "unsupported type".to_owned(),
            }),
        }
    }
}
