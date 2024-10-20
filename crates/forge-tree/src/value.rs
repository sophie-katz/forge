use crate::{AsTreeBase, TreeBase};

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralBool<'source_context> {
    base: TreeBase<'source_context>,
    value: bool,
}

impl<'source_context> ValueLiteralBool<'source_context> {
    pub fn value(&self) -> bool {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralBool<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralI8<'source_context> {
    base: TreeBase<'source_context>,
    value: i8,
}

impl<'source_context> ValueLiteralI8<'source_context> {
    pub fn value(&self) -> i8 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralI8<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralI16<'source_context> {
    base: TreeBase<'source_context>,
    value: i16,
}

impl<'source_context> ValueLiteralI16<'source_context> {
    pub fn value(&self) -> i16 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralI16<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralI32<'source_context> {
    base: TreeBase<'source_context>,
    value: i32,
}

impl<'source_context> ValueLiteralI32<'source_context> {
    pub fn value(&self) -> i32 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralI32<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralI64<'source_context> {
    base: TreeBase<'source_context>,
    value: i64,
}

impl<'source_context> ValueLiteralI64<'source_context> {
    pub fn value(&self) -> i64 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralI64<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralISize<'source_context> {
    base: TreeBase<'source_context>,
    value: i64,
}

impl<'source_context> ValueLiteralISize<'source_context> {
    pub fn value(&self) -> i64 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralISize<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralU8<'source_context> {
    base: TreeBase<'source_context>,
    value: u8,
}

impl<'source_context> ValueLiteralU8<'source_context> {
    pub fn value(&self) -> u8 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralU8<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralU16<'source_context> {
    base: TreeBase<'source_context>,
    value: u16,
}

impl<'source_context> ValueLiteralU16<'source_context> {
    pub fn value(&self) -> u16 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralU16<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralU32<'source_context> {
    base: TreeBase<'source_context>,
    value: u32,
}

impl<'source_context> ValueLiteralU32<'source_context> {
    pub fn value(&self) -> u32 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralU32<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralU64<'source_context> {
    base: TreeBase<'source_context>,
    value: u64,
}

impl<'source_context> ValueLiteralU64<'source_context> {
    pub fn value(&self) -> u64 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralU64<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueLiteralUSize<'source_context> {
    base: TreeBase<'source_context>,
    value: u64,
}

impl<'source_context> ValueLiteralUSize<'source_context> {
    pub fn value(&self) -> u64 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueLiteralUSize<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueF32<'source_context> {
    base: TreeBase<'source_context>,
    value: f32,
}

impl<'source_context> ValueF32<'source_context> {
    pub fn value(&self) -> f32 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueF32<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueF64<'source_context> {
    base: TreeBase<'source_context>,
    value: f64,
}

impl<'source_context> ValueF64<'source_context> {
    pub fn value(&self) -> f64 {
        self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueF64<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueChar<'source_context> {
    base: TreeBase<'source_context>,
    value: String,
}

impl<'source_context> ValueChar<'source_context> {
    pub fn value(&self) -> &str {
        &self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueChar<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueString<'source_context> {
    base: TreeBase<'source_context>,
    value: String,
}

impl<'source_context> ValueString<'source_context> {
    pub fn value(&self) -> &str {
        &self.value
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueString<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueSymbol<'source_context> {
    base: TreeBase<'source_context>,
    name: String,
}

impl<'source_context> ValueSymbol<'source_context> {
    pub fn name(&self) -> &str {
        &self.name
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueSymbol<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum UnaryOperator {
    BitNegate,
    BoolNot,
    Negate,
    Increment,
    Decrement,
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueUnary<'source_context> {
    base: TreeBase<'source_context>,
    operator: UnaryOperator,
    operand: Box<Value<'source_context>>,
}

impl<'source_context> ValueUnary<'source_context> {
    pub fn operator(&self) -> UnaryOperator {
        self.operator
    }

    pub fn operand(&self) -> &Value<'source_context> {
        &self.operand
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueUnary<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum BinaryOperator {
    BitAnd,
    BitOr,
    BitXor,
    BitShiftLeft,
    BitShiftRight,
    BoolAnd,
    BoolOr,
    EqualTo,
    NotEqualTo,
    LessThan,
    LessThanOrEqualTo,
    GreaterThan,
    GreaterThanOrEqualTo,
    Add,
    Subtract,
    Multiply,
    Exponentiate,
    Divide,
    Modulo,
    Assign,
    BitAndAssign,
    BitOrAssign,
    BitXorAssign,
    BitShiftLeftAssign,
    BitShiftRightAssign,
    AddAssign,
    SubtractAssign,
    MultiplyAssign,
    ExponentiateAssign,
    DivideAssign,
    ModuloAssign,
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueBinary<'source_context> {
    base: TreeBase<'source_context>,
    left: Box<Value<'source_context>>,
    operator: BinaryOperator,
    right: Box<Value<'source_context>>,
}

impl<'source_context> ValueBinary<'source_context> {
    pub fn operator(&self) -> BinaryOperator {
        self.operator
    }

    pub fn left(&self) -> &Value<'source_context> {
        &self.left
    }

    pub fn right(&self) -> &Value<'source_context> {
        &self.right
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueBinary<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ValueCall<'source_context> {
    base: TreeBase<'source_context>,
    callee: Box<Value<'source_context>>,
    arguments: Vec<Value<'source_context>>,
}

impl<'source_context> ValueCall<'source_context> {
    pub fn callee(&self) -> &Value<'source_context> {
        &self.callee
    }

    pub fn iter_arguments(&self) -> impl Iterator<Item = &Value<'source_context>> {
        self.arguments.iter()
    }
}

impl<'source_context> AsTreeBase<'source_context> for ValueCall<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        &self.base
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Value<'source_context> {
    LiteralBool(ValueLiteralBool<'source_context>),
    LiteralI8(ValueLiteralI8<'source_context>),
    LiteralI16(ValueLiteralI16<'source_context>),
    LiteralI32(ValueLiteralI32<'source_context>),
    LiteralI64(ValueLiteralI64<'source_context>),
    LiteralISize(ValueLiteralISize<'source_context>),
    LiteralU8(ValueLiteralU8<'source_context>),
    LiteralU16(ValueLiteralU16<'source_context>),
    LiteralU32(ValueLiteralU32<'source_context>),
    LiteralU64(ValueLiteralU64<'source_context>),
    LiteralUSize(ValueLiteralUSize<'source_context>),
    F32(ValueF32<'source_context>),
    F64(ValueF64<'source_context>),
    Char(ValueChar<'source_context>),
    String(ValueString<'source_context>),
    Symbol(ValueSymbol<'source_context>),
    Unary(ValueUnary<'source_context>),
    Binary(ValueBinary<'source_context>),
    Call(ValueCall<'source_context>),
}

impl<'source_context> AsTreeBase<'source_context> for Value<'source_context> {
    fn as_tree_base(&self) -> &TreeBase<'source_context> {
        match self {
            Self::LiteralBool(value) => value.as_tree_base(),
            Self::LiteralI8(value) => value.as_tree_base(),
            Self::LiteralI16(value) => value.as_tree_base(),
            Self::LiteralI32(value) => value.as_tree_base(),
            Self::LiteralI64(value) => value.as_tree_base(),
            Self::LiteralISize(value) => value.as_tree_base(),
            Self::LiteralU8(value) => value.as_tree_base(),
            Self::LiteralU16(value) => value.as_tree_base(),
            Self::LiteralU32(value) => value.as_tree_base(),
            Self::LiteralU64(value) => value.as_tree_base(),
            Self::LiteralUSize(value) => value.as_tree_base(),
            Self::F32(value) => value.as_tree_base(),
            Self::F64(value) => value.as_tree_base(),
            Self::Char(value) => value.as_tree_base(),
            Self::String(value) => value.as_tree_base(),
            Self::Symbol(value) => value.as_tree_base(),
            Self::Unary(value) => value.as_tree_base(),
            Self::Binary(value) => value.as_tree_base(),
            Self::Call(value) => value.as_tree_base(),
        }
    }
}
