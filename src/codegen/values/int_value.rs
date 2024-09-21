use inkwell::values::IntValue;

use crate::{
    codegen::{
        context::CodeGenerationContext, error::ErrorCodeGeneration,
        types::int_type::CodeGeneratorIntType,
    },
    domain::ValueIntUnsigned,
    parsing::ast::Value,
};

pub enum CodeGeneratorIntValue<'input> {
    FromValue(&'input Value),
}

impl<'input> CodeGeneratorIntValue<'input> {
    pub fn generate<'llvm_context>(
        self,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<IntValue<'llvm_context>, ErrorCodeGeneration> {
        match self {
            CodeGeneratorIntValue::FromValue(value) => {
                Self::generate_from_value(value, code_generation_context)
            }
        }
    }

    fn generate_from_value<'llvm_context>(
        value: &Value,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<IntValue<'llvm_context>, ErrorCodeGeneration> {
        match value {
            Value::Bool(value) => Self::generate_from_bool(*value, code_generation_context),
            Value::IntUnsigned(value_int_unsigned) => {
                Self::generate_from_value_int_unsigned(value_int_unsigned, code_generation_context)
            }
            _ => Err(ErrorCodeGeneration::UnsupportedAst {
                description: format!("expected integer, found {:?}", value),
            }),
        }
    }

    fn generate_from_bool<'llvm_context>(
        value: bool,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<IntValue<'llvm_context>, ErrorCodeGeneration> {
        Ok(code_generation_context
            .llvm_context
            .bool_type()
            .const_int(if value { 1 } else { 0 }, false))
    }

    fn generate_from_value_int_unsigned<'llvm_context>(
        value_int_unsigned: &ValueIntUnsigned,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<IntValue<'llvm_context>, ErrorCodeGeneration> {
        CodeGeneratorIntType::FromValueWidth(&value_int_unsigned.width)
            .generate(code_generation_context)
            .map(|r#type| r#type.const_int(value_int_unsigned.value, false))
    }
}
