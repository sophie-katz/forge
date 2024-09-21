use inkwell::{
    builder::Builder,
    context::Context,
    module::Module,
    targets::{Target, TargetMachine},
};

use super::{error::ErrorCodeGeneration, options::CodeGenerationOptions};

pub struct CodeGenerationContext<'llvm_context> {
    pub llvm_context: &'llvm_context Context,
    pub module: Module<'llvm_context>,
    pub builder: Builder<'llvm_context>,
    pub options: CodeGenerationOptions,
    pub target: Target,
    pub target_machine: TargetMachine,
}

impl<'llvm_context> CodeGenerationContext<'llvm_context> {
    /// Create a new code generation context.
    ///
    /// # Example
    ///
    /// ```
    /// use inkwell::context::Context;
    ///
    /// let llvm_context = Context::create();
    /// let code_generation_context = CodeGenerationContext::new(&llvm_context, "my_module");
    /// ```
    pub fn new(
        context: &'llvm_context Context,
        module_name: &str,
        options: CodeGenerationOptions,
    ) -> Result<Self, ErrorCodeGeneration> {
        let module = context.create_module(module_name);
        let builder = context.create_builder();

        let target = Target::from_triple(&options.target_triple).map_err(|error| {
            ErrorCodeGeneration::UnsupportedTargetArchitecture(error.to_string())
        })?;

        let target_machine = target
            .create_target_machine(
                &options.target_triple,
                &options.cpu,
                &options.features,
                options.optimization_level,
                options.reloc_mode,
                options.code_model,
            )
            .ok_or_else(|| {
                ErrorCodeGeneration::UnsupportedTargetArchitecture(
                    "unable to create LLVM target machine".to_owned(),
                )
            })?;

        Ok(Self {
            llvm_context: context,
            module,
            builder,
            options,
            target,
            target_machine,
        })
    }

    pub fn pointer_width(&self) -> usize {
        self.target_machine
            .get_target_data()
            .get_pointer_byte_size(None) as usize
    }
}
