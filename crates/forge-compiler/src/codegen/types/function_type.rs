use inkwell::types::{BasicMetadataTypeEnum, FunctionType};

use crate::{
    codegen::{context::CodeGenerationContext, error::ErrorCodeGeneration},
    parsing::ast::{FunctionArgument, Type, TypeBasic, TypeFunction},
};

use super::{
    float_type::CodeGeneratorFloatType, int_type::CodeGeneratorIntType,
    void_type::CodeGeneratorVoidType,
};

pub enum CodeGeneratorFunctionType<'input> {
    FromTypeFunction(&'input TypeFunction),
}

impl<'input> CodeGeneratorFunctionType<'input> {
    pub fn generate<'llvm_context>(
        self,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<FunctionType<'llvm_context>, ErrorCodeGeneration> {
        match self {
            Self::FromTypeFunction(r#type) => {
                Self::generate_from_type_function(r#type, code_generation_context)
            }
        }
    }

    fn generate_from_type_function<'llvm_context>(
        r#type: &'input TypeFunction,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<FunctionType<'llvm_context>, ErrorCodeGeneration> {
        let return_type = Self::get_return_type(&r#type.return_type)?;

        let arguments = Self::generate_basic_metadata_type_enum_vec_from_arguments(
            r#type.arguments.iter(),
            code_generation_context,
        )?;

        Self::generate_from_type_function_with_arguments(
            return_type,
            code_generation_context,
            &arguments,
        )
    }

    fn get_return_type(
        return_type: &'input Option<Box<Type>>,
    ) -> Result<&'input TypeBasic, ErrorCodeGeneration> {
        match return_type {
            Some(r#type) => match r#type.as_ref() {
                Type::Basic(r#type) => Ok(r#type),
                Type::Function(_) => Err(ErrorCodeGeneration::UnsupportedAst {
                    description: "functions as first-class values".to_owned(),
                }),
            },
            None => Err(ErrorCodeGeneration::UnsupportedAst {
                description: "missing return type in function".to_owned(),
            }),
        }
    }

    fn generate_basic_metadata_type_enum_vec_from_arguments<'llvm_context>(
        arguments: impl Iterator<Item = &'input FunctionArgument>,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<Vec<BasicMetadataTypeEnum<'llvm_context>>, ErrorCodeGeneration> {
        arguments
            .map(|argument| {
                let r#type = Self::get_argument_type(argument)?;

                Self::generate_basic_metadata_type_enum_from_type_basic(
                    r#type,
                    code_generation_context,
                )
            })
            .collect::<Result<Vec<BasicMetadataTypeEnum<'llvm_context>>, ErrorCodeGeneration>>()
    }

    fn get_argument_type(
        argument: &'input FunctionArgument,
    ) -> Result<&'input TypeBasic, ErrorCodeGeneration> {
        match &argument.r#type {
            Some(r#type) => match r#type.as_ref() {
                Type::Basic(r#type) => Ok(r#type),
                Type::Function(_) => Err(ErrorCodeGeneration::UnsupportedAst {
                    description: "functions as first-class values".to_owned(),
                }),
            },
            None => Err(ErrorCodeGeneration::UnsupportedAst {
                description: "missing type in function argument".to_owned(),
            }),
        }
    }

    fn generate_basic_metadata_type_enum_from_type_basic<'llvm_context>(
        r#type: &'input TypeBasic,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
    ) -> Result<BasicMetadataTypeEnum<'llvm_context>, ErrorCodeGeneration> {
        match r#type {
            TypeBasic::Bool | TypeBasic::IntUnsigned(_) | TypeBasic::IntSigned(_) => {
                Ok(CodeGeneratorIntType::FromTypeBasic(r#type)
                    .generate(code_generation_context)?
                    .into())
            }
            TypeBasic::Float(_) => Ok(CodeGeneratorFloatType::FromTypeBasic(r#type)
                .generate(code_generation_context)?
                .into()),
            _ => {
                return Err(ErrorCodeGeneration::UnsupportedAst {
                    description: "unsupported type".to_owned(),
                })
            }
        }
    }

    fn generate_from_type_function_with_arguments<'llvm_context>(
        return_type: &'input TypeBasic,
        code_generation_context: &CodeGenerationContext<'llvm_context>,
        arguments: &[BasicMetadataTypeEnum<'llvm_context>],
    ) -> Result<FunctionType<'llvm_context>, ErrorCodeGeneration> {
        match return_type {
            TypeBasic::Bool | TypeBasic::IntSigned(_) | TypeBasic::IntUnsigned(_) => {
                Ok(CodeGeneratorIntType::FromTypeBasic(return_type)
                    .generate(code_generation_context)?
                    .fn_type(arguments, false))
            }
            TypeBasic::Float(_) => Ok(CodeGeneratorFloatType::FromTypeBasic(return_type)
                .generate(code_generation_context)?
                .fn_type(arguments, false)),
            TypeBasic::Void | TypeBasic::Never => {
                Ok(CodeGeneratorVoidType::FromTypeBasic(return_type)
                    .generate(code_generation_context)?
                    .fn_type(arguments, false))
            }
            _ => {
                return Err(ErrorCodeGeneration::UnsupportedAst {
                    description: "unsupported type".to_owned(),
                })
            }
        }
    }
}
