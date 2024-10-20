use crate::{AsTreeBase, Statement, TreeBase, Type, Value};

#[derive(Debug, Clone, PartialEq)]
pub struct DeclarationVariable<'source_context> {
    base: TreeBase<'source_context>,
    name: String,
    r#type: Option<Box<Type<'source_context>>>,
    value: Option<Box<Value<'source_context>>>,
}

impl<'source_context> DeclarationVariable<'source_context> {
    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn r#type(&self) -> Option<&Type<'source_context>> {
        self.r#type.as_deref()
    }

    pub fn value(&self) -> Option<&Value<'source_context>> {
        self.value.as_deref()
    }
}

impl<'source_context> AsTreeBase<'source_context> for DeclarationVariable<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct DeclarationFunction<'source_context> {
    base: TreeBase<'source_context>,
    name: String,
    return_type: Option<Box<Type<'source_context>>>,
    arguments: Vec<DeclarationVariable<'source_context>>,
    body: Option<Box<Statement<'source_context>>>,
}

impl<'source_context> DeclarationFunction<'source_context> {
    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn return_type(&self) -> Option<&Type<'source_context>> {
        self.return_type.as_deref()
    }

    pub fn iter_arguments(&self) -> impl Iterator<Item = &DeclarationVariable<'source_context>> {
        self.arguments.iter()
    }

    pub fn body(&self) -> Option<&Statement<'source_context>> {
        self.body.as_deref()
    }
}

impl<'source_context> AsTreeBase<'source_context> for DeclarationFunction<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct DeclarationType<'source_context> {
    base: TreeBase<'source_context>,
    name: String,
    r#type: Option<Box<Type<'source_context>>>,
    is_explicit: bool,
}

impl<'source_context> DeclarationType<'source_context> {
    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn r#type(&self) -> Option<&Type<'source_context>> {
        self.r#type.as_deref()
    }

    pub fn is_explicit(&self) -> bool {
        self.is_explicit
    }
}

impl<'source_context> AsTreeBase<'source_context> for DeclarationType<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct DeclarationStruct<'source_context> {
    base: TreeBase<'source_context>,
    name: String,
    implements: Vec<Type<'source_context>>,
    members: Vec<Declaration<'source_context>>,
}

impl<'source_context> DeclarationStruct<'source_context> {
    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn iter_implements(&self) -> impl Iterator<Item = &Type<'source_context>> {
        self.implements.iter()
    }

    pub fn iter_members(&self) -> impl Iterator<Item = &Declaration<'source_context>> {
        self.members.iter()
    }
}

impl<'source_context> AsTreeBase<'source_context> for DeclarationStruct<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct DeclarationInterface<'source_context> {
    base: TreeBase<'source_context>,
    name: String,
    implements: Vec<Type<'source_context>>,
    members: Vec<Declaration<'source_context>>,
}

impl<'source_context> DeclarationInterface<'source_context> {
    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn iter_implements(&self) -> impl Iterator<Item = &Type<'source_context>> {
        self.implements.iter()
    }

    pub fn iter_members(&self) -> impl Iterator<Item = &Declaration<'source_context>> {
        self.members.iter()
    }
}

impl<'source_context> AsTreeBase<'source_context> for DeclarationInterface<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Declaration<'source_context> {
    Variable(DeclarationVariable<'source_context>),
    Function(DeclarationFunction<'source_context>),
    Type(DeclarationType<'source_context>),
    Struct(DeclarationStruct<'source_context>),
    Interface(DeclarationInterface<'source_context>),
}

impl<'source_context> AsTreeBase<'source_context> for Declaration<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        match self {
            Self::Variable(value) => value.as_tree_base(),
            Self::Function(value) => value.as_tree_base(),
            Self::Type(value) => value.as_tree_base(),
            Self::Struct(value) => value.as_tree_base(),
            Self::Interface(value) => value.as_tree_base(),
        }
    }
}
