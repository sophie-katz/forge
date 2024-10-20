use crate::{AsTypeBaseMut, Type};

pub struct TypeBuilder<'source_context> {
    tree: Type<'source_context>,
}

impl<'source_context> TypeBuilder<'source_context> {
    pub fn r#const(mut self) -> Self {
        self.tree.as_type_base_mut().is_const = true;
        self
    }

    pub fn reference(mut self) -> Self {
        self.tree.as_type_base_mut().is_reference = true;
        self
    }

    pub fn optional(mut self) -> Self {
        self.tree.as_type_base_mut().is_optional = true;
        self
    }

    pub fn pointer(mut self) -> Self {
        self.tree.as_type_base_mut().pointer_level += 1;
        self
    }

    pub fn build(self) -> Type<'source_context> {
        self.tree
    }
}
