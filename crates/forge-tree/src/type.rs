use forge_parsing::sourcing::SourceLocation;

use crate::{AsTreeBase, AsTypeBase, AsTypeBaseMut, TreeBase, TypeBase};

#[derive(Debug, Clone, PartialEq)]
pub struct TypeBool<'source_context> {
    base: TypeBase<'source_context>,
}

impl<'source_context> TypeBool<'source_context> {
    pub fn new(
        first_location: SourceLocation<'source_context>,
        last_location: SourceLocation<'source_context>,
    ) -> TypeBool<'source_context> {
        TypeBool {
            base: TypeBase::new(first_location, last_location),
        }
    }
}

impl<'source_context> AsTreeBase<'source_context> for TypeBool<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        self.as_type_base().as_tree_base()
    }
}

impl<'source_context> AsTypeBase<'source_context> for TypeBool<'source_context> {
    fn as_type_base(&self) -> &TypeBase<'source_context> {
        &self.base
    }
}

impl<'source_context> AsTypeBaseMut<'source_context> for TypeBool<'source_context> {
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context> {
        &mut self.base
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum IntWidth {
    Fixed8,
    Fixed16,
    Fixed32,
    Fixed64,
    PointerSize,
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypeInt<'source_context> {
    base: TypeBase<'source_context>,
    width: IntWidth,
    signed: bool,
}

impl<'source_context> TypeInt<'source_context> {
    pub fn new(
        first_location: SourceLocation<'source_context>,
        last_location: SourceLocation<'source_context>,
        width: IntWidth,
        signed: bool,
    ) -> TypeInt<'source_context> {
        TypeInt {
            base: TypeBase::new(first_location, last_location),
            width,
            signed,
        }
    }

    pub fn width(&self) -> IntWidth {
        self.width
    }

    pub fn signed(&self) -> bool {
        self.signed
    }
}

impl<'source_context> AsTreeBase<'source_context> for TypeInt<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        self.as_type_base().as_tree_base()
    }
}

impl<'source_context> AsTypeBase<'source_context> for TypeInt<'source_context> {
    fn as_type_base(&self) -> &TypeBase<'source_context> {
        &self.base
    }
}

impl<'source_context> AsTypeBaseMut<'source_context> for TypeInt<'source_context> {
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context> {
        &mut self.base
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum FloatWidth {
    Fixed32,
    Fixed64,
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypeFloat<'source_context> {
    base: TypeBase<'source_context>,
    width: FloatWidth,
}

impl<'source_context> TypeFloat<'source_context> {
    pub fn new(
        first_location: SourceLocation<'source_context>,
        last_location: SourceLocation<'source_context>,
        width: FloatWidth,
    ) -> TypeFloat<'source_context> {
        TypeFloat {
            base: TypeBase::new(first_location, last_location),
            width,
        }
    }

    pub fn width(&self) -> FloatWidth {
        self.width
    }
}

impl<'source_context> AsTreeBase<'source_context> for TypeFloat<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        self.as_type_base().as_tree_base()
    }
}

impl<'source_context> AsTypeBase<'source_context> for TypeFloat<'source_context> {
    fn as_type_base(&self) -> &TypeBase<'source_context> {
        &self.base
    }
}

impl<'source_context> AsTypeBaseMut<'source_context> for TypeFloat<'source_context> {
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context> {
        &mut self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypeVoid<'source_context> {
    base: TypeBase<'source_context>,
}

impl<'source_context> AsTreeBase<'source_context> for TypeVoid<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        self.as_type_base().as_tree_base()
    }
}

impl<'source_context> AsTypeBase<'source_context> for TypeVoid<'source_context> {
    fn as_type_base(&self) -> &TypeBase<'source_context> {
        &self.base
    }
}

impl<'source_context> AsTypeBaseMut<'source_context> for TypeVoid<'source_context> {
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context> {
        &mut self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypeNever<'source_context> {
    base: TypeBase<'source_context>,
}

impl<'source_context> AsTreeBase<'source_context> for TypeNever<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        self.as_type_base().as_tree_base()
    }
}

impl<'source_context> AsTypeBase<'source_context> for TypeNever<'source_context> {
    fn as_type_base(&self) -> &TypeBase<'source_context> {
        &self.base
    }
}

impl<'source_context> AsTypeBaseMut<'source_context> for TypeNever<'source_context> {
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context> {
        &mut self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypeNull<'source_context> {
    base: TypeBase<'source_context>,
}

impl<'source_context> AsTreeBase<'source_context> for TypeNull<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        self.as_type_base().as_tree_base()
    }
}

impl<'source_context> AsTypeBase<'source_context> for TypeNull<'source_context> {
    fn as_type_base(&self) -> &TypeBase<'source_context> {
        &self.base
    }
}

impl<'source_context> AsTypeBaseMut<'source_context> for TypeNull<'source_context> {
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context> {
        &mut self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypeSymbol<'source_context> {
    base: TypeBase<'source_context>,
    name: String,
}

impl<'source_context> TypeSymbol<'source_context> {
    pub fn name(&self) -> &str {
        &self.name
    }
}

impl<'source_context> AsTreeBase<'source_context> for TypeSymbol<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        self.as_type_base().as_tree_base()
    }
}

impl<'source_context> AsTypeBase<'source_context> for TypeSymbol<'source_context> {
    fn as_type_base(&self) -> &TypeBase<'source_context> {
        &self.base
    }
}

impl<'source_context> AsTypeBaseMut<'source_context> for TypeSymbol<'source_context> {
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context> {
        &mut self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Type<'source_context> {
    Bool(TypeBool<'source_context>),
    Int(TypeInt<'source_context>),
    Float(TypeFloat<'source_context>),
    Void(TypeVoid<'source_context>),
    Never(TypeNever<'source_context>),
    Null(TypeNull<'source_context>),
    Symbol(TypeSymbol<'source_context>),
}

impl<'source_context> AsTreeBase<'source_context> for Type<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        self.as_type_base().as_tree_base()
    }
}

impl<'source_context> AsTypeBase<'source_context> for Type<'source_context> {
    fn as_type_base(&self) -> &TypeBase<'source_context> {
        match self {
            Self::Bool(inner) => inner.as_type_base(),
            Self::Int(inner) => inner.as_type_base(),
            Self::Float(inner) => inner.as_type_base(),
            Self::Void(inner) => inner.as_type_base(),
            Self::Never(inner) => inner.as_type_base(),
            Self::Null(inner) => inner.as_type_base(),
            Self::Symbol(inner) => inner.as_type_base(),
        }
    }
}

impl<'source_context> AsTypeBaseMut<'source_context> for Type<'source_context> {
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context> {
        match self {
            Self::Bool(inner) => inner.as_type_base_mut(),
            Self::Int(inner) => inner.as_type_base_mut(),
            Self::Float(inner) => inner.as_type_base_mut(),
            Self::Void(inner) => inner.as_type_base_mut(),
            Self::Never(inner) => inner.as_type_base_mut(),
            Self::Null(inner) => inner.as_type_base_mut(),
            Self::Symbol(inner) => inner.as_type_base_mut(),
        }
    }
}
