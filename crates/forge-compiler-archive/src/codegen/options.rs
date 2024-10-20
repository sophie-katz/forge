use inkwell::{
    targets::{CodeModel, RelocMode, TargetMachine, TargetTriple},
    OptimizationLevel,
};

pub struct CodeGenerationOptions {
    pub target_triple: TargetTriple,
    pub cpu: String,
    pub features: String,
    pub optimization_level: OptimizationLevel,
    pub reloc_mode: RelocMode,
    pub code_model: CodeModel,
}

impl CodeGenerationOptions {
    pub fn new() -> Self {
        Self {
            target_triple: TargetMachine::get_default_triple(),
            cpu: TargetMachine::get_host_cpu_name().to_string(),
            features: TargetMachine::get_host_cpu_features().to_string(),
            optimization_level: OptimizationLevel::Default,
            reloc_mode: RelocMode::Default,
            code_model: CodeModel::Default,
        }
    }

    pub fn with_target_triple(mut self, target_triple: TargetTriple) -> Self {
        self.target_triple = target_triple;
        self
    }

    pub fn with_cpu(mut self, cpu: String) -> Self {
        self.cpu = cpu;
        self
    }

    pub fn with_features(mut self, features: String) -> Self {
        self.features = features;
        self
    }

    pub fn with_optimization_level(mut self, optimization_level: OptimizationLevel) -> Self {
        self.optimization_level = optimization_level;
        self
    }

    pub fn with_reloc_mode(mut self, reloc_mode: RelocMode) -> Self {
        self.reloc_mode = reloc_mode;
        self
    }

    pub fn with_code_model(mut self, code_model: CodeModel) -> Self {
        self.code_model = code_model;
        self
    }
}
