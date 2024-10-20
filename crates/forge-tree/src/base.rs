use forge_parsing::sourcing::SourceLocation;

/// Shared fields across all tree types.
#[derive(Debug, Clone, PartialEq)]
pub struct TreeBase<'source_context> {
    first_location: SourceLocation<'source_context>,
    last_location: SourceLocation<'source_context>,
}

impl<'source_context> TreeBase<'source_context> {
    pub(crate) fn new(
        first_location: SourceLocation<'source_context>,
        last_location: SourceLocation<'source_context>,
    ) -> Self {
        Self {
            first_location,
            last_location,
        }
    }

    pub fn first_location(&self) -> &SourceLocation<'source_context> {
        &self.first_location
    }

    pub fn last_location(&self) -> &SourceLocation<'source_context> {
        &self.last_location
    }
}

/// A trait for any types that contain a tree base.
pub trait AsTreeBase<'source_context> {
    /// Exposes the base fields.
    ///
    /// This must be implemented for the base field accessors to work as expected.
    fn as_tree_base(&self) -> &TreeBase<'source_context>;
}

/// Shared fields across all type types.
#[derive(Debug, Clone, PartialEq)]
pub struct TypeBase<'source_context> {
    base: TreeBase<'source_context>,
    is_const: bool,
    is_reference: bool,
    is_optional: bool,
    pointer_level: u32,
}

impl<'source_context> TypeBase<'source_context> {
    pub(crate) fn new(
        first_location: SourceLocation<'source_context>,
        last_location: SourceLocation<'source_context>,
    ) -> Self {
        Self {
            base: TreeBase::new(first_location, last_location),
            is_const: false,
            is_reference: false,
            is_optional: false,
            pointer_level: 0,
        }
    }

    pub fn is_const(&self) -> bool {
        self.is_const
    }

    pub fn is_reference(&self) -> bool {
        self.is_reference
    }

    pub fn is_optional(&self) -> bool {
        self.is_optional
    }

    pub fn pointer_level(&self) -> u32 {
        self.pointer_level
    }
}

impl<'source_context> AsTreeBase<'source_context> for TypeBase<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

/// A trait for any types that contain a type base.
pub trait AsTypeBase<'source_context>: AsTreeBase<'source_context> {
    /// Exposes the base fields.
    ///
    /// This must be implemented for the base field accessors to work as expected.
    fn as_type_base(&self) -> &TypeBase<'source_context>;
}

/// A trait for any types that contain a type base.
pub trait AsTypeBaseMut<'source_context>: Sized {
    /// Exposes the base fields.
    ///
    /// This must be implemented for the base field accessors to work as expected.
    fn as_type_base_mut(&mut self) -> &mut TypeBase<'source_context>;

    fn r#const(mut self) -> Self {
        self.as_type_base_mut().is_const = true;
        self
    }

    fn reference(mut self) -> Self {
        self.as_type_base_mut().is_reference = true;
        self
    }

    fn optional(mut self) -> Self {
        self.as_type_base_mut().is_optional = true;
        self
    }

    fn pointer(mut self) -> Self {
        self.as_type_base_mut().pointer_level += 1;
        self
    }
}
