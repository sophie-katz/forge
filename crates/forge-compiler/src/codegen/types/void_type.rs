use inkwell::types::VoidType;

use crate::{
    codegen::{context::CodeGenerationContext, error::ErrorCodeGeneration},
    parsing::ast::TypeBasic,
};

pub enum CodeGeneratorVoidType<'input> {
    Default,
    FromTypeBasic(&'input TypeBasic),
}

impl<'input> CodeGeneratorVoidType<'input> {
    pub fn generate<'llvm_context>(
        self,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<VoidType<'llvm_context>, ErrorCodeGeneration> {
        match self {
            Self::Default => Self::generate_default(code_generation_context),
            Self::FromTypeBasic(r#type) => {
                Self::generate_from_type_basic(r#type, code_generation_context)
            }
        }
    }

    fn generate_default<'llvm_context>(
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<VoidType<'llvm_context>, ErrorCodeGeneration> {
        Ok(code_generation_context.llvm_context.void_type())
    }

    fn generate_from_type_basic<'llvm_context>(
        r#type: &'input TypeBasic,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<VoidType<'llvm_context>, ErrorCodeGeneration> {
        match r#type {
            TypeBasic::Void | TypeBasic::Never => Self::generate_default(code_generation_context),
            _ => Err(ErrorCodeGeneration::UnsupportedAst {
                description: "unsupported type".to_owned(),
            }),
        }
    }
}
