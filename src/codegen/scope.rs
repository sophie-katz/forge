use std::collections::HashMap;

use crate::parsing::ast::Symbol;

use super::error::ErrorCodeGeneration;

pub struct Scope<Variable> {
    variables: HashMap<Symbol, Variable>,
}

impl<Variable> Scope<Variable> {
    pub fn new() -> Self {
        Self {
            variables: HashMap::new(),
        }
    }

    fn declare_variable(
        &mut self,
        symbol: Symbol,
        variable: Variable,
    ) -> Result<(), ErrorCodeGeneration> {
        match self.variables.try_insert(symbol, variable) {
            Ok(_) => Ok(()),
            Err(error) => Err(ErrorCodeGeneration::VariableAlreadyDeclared(
                error.entry.key().to_string(),
            )),
        }
    }

    fn get_variable(&self, symbol: &Symbol) -> Result<&Variable, ErrorCodeGeneration> {
        self.variables
            .get(symbol)
            .ok_or_else(|| ErrorCodeGeneration::UndeclaredVariable(symbol.to_string()))
    }
}
