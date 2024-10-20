use crate::{AsTreeBase, TreeBase, Value};

#[derive(Debug, Clone, PartialEq)]
pub struct StatementValue<'source_context> {
    base: TreeBase<'source_context>,
    value: Box<Value<'source_context>>,
}

impl<'source_context> StatementValue<'source_context> {
    pub fn value(&self) -> &Value<'source_context> {
        &self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for StatementValue<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct StatementBlock<'source_context> {
    base: TreeBase<'source_context>,
    statements: Vec<Statement<'source_context>>,
}

impl<'source_context> StatementBlock<'source_context> {
    pub fn iter_statements(&self) -> impl Iterator<Item = &Statement<'source_context>> {
        self.statements.iter()
    }
}

impl<'source_context> AsTreeBase<'source_context> for StatementBlock<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct StatementIf<'source_context> {
    base: TreeBase<'source_context>,
    condition: Box<Value<'source_context>>,
    then: Box<Statement<'source_context>>,
    r#else: Option<Box<Statement<'source_context>>>,
}

impl<'source_context> StatementIf<'source_context> {
    pub fn condition(&self) -> &Value<'source_context> {
        &self.condition
    }

    pub fn then(&self) -> &Statement<'source_context> {
        &self.then
    }

    pub fn r#else(&self) -> Option<&Statement<'source_context>> {
        self.r#else.as_deref()
    }
}

impl<'source_context> AsTreeBase<'source_context> for StatementIf<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct StatementWhile<'source_context> {
    base: TreeBase<'source_context>,
    condition: Box<Value<'source_context>>,
    body: Option<Box<Statement<'source_context>>>,
}

impl<'source_context> StatementWhile<'source_context> {
    pub fn condition(&self) -> &Value<'source_context> {
        &self.condition
    }

    pub fn body(&self) -> Option<&Statement<'source_context>> {
        self.body.as_deref()
    }
}

impl<'source_context> AsTreeBase<'source_context> for StatementWhile<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct StatementContinue<'source_context> {
    base: TreeBase<'source_context>,
}

impl<'source_context> AsTreeBase<'source_context> for StatementContinue<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct StatementBreak<'source_context> {
    base: TreeBase<'source_context>,
}

impl<'source_context> AsTreeBase<'source_context> for StatementBreak<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct StatementReturn<'source_context> {
    base: TreeBase<'source_context>,
    value: Box<Value<'source_context>>,
}

impl<'source_context> StatementReturn<'source_context> {
    pub fn value(&self) -> &Value<'source_context> {
        &self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for StatementReturn<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Statement<'source_context> {
    Value(StatementValue<'source_context>),
    Block(StatementBlock<'source_context>),
    If(StatementIf<'source_context>),
    While(StatementWhile<'source_context>),
    Continue(StatementContinue<'source_context>),
    Break(StatementBreak<'source_context>),
    Return(StatementReturn<'source_context>),
}

impl<'source_context> AsTreeBase<'source_context> for Statement<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        match self {
            Self::Value(value) => value.as_tree_base(),
            Self::Block(block) => block.as_tree_base(),
            Self::If(r#if) => r#if.as_tree_base(),
            Self::While(r#while) => r#while.as_tree_base(),
            Self::Continue(r#continue) => r#continue.as_tree_base(),
            Self::Break(r#break) => r#break.as_tree_base(),
            Self::Return(r#return) => r#return.as_tree_base(),
        }
    }
}
