use crate::domain::{ValueFloat, ValueIntUnsigned, ValueWidth};

#[derive(Clone, Debug, PartialEq)]
pub struct Symbol {
    pub name: Vec<u8>,
}

#[derive(Clone, Debug, PartialEq)]
pub struct FunctionArgument {
    pub name: Option<Symbol>,
    pub r#type: Option<Box<Type>>,
}

#[derive(Clone, Debug, PartialEq)]
pub struct TypeFunction {
    pub arguments: Vec<FunctionArgument>,
    pub return_type: Option<Box<Type>>,
}

#[derive(Clone, Debug, PartialEq)]
pub enum Type {
    // Basic
    Bool,
    IntUnsigned(ValueWidth),
    IntSigned(ValueWidth),
    Float(ValueWidth),
    Void,
    Never,
    Null,

    // Symbols
    Symbol { name: Symbol },

    // Functions
    Function(TypeFunction),
}

#[derive(Clone, Debug, PartialEq)]
pub enum OperatorUnary {
    BitNegate,
    BoolNot,
    Negate,
    Positive,
    Increment,
    Decrement,
}

#[derive(Clone, Debug, PartialEq)]
pub enum OperatorBinary {
    BitAnd,
    BitOr,
    BitXor,
    BitShiftLeft,
    BitShiftRight,
    BoolAnd,
    BoolOr,
    Add,
    Subtract,
    Multiply,
    Divide,
    Exponentiate,
    Modulate,
    LessThan,
    LessThanOrEqualTo,
    GreaterThan,
    GreaterThanOrEqualTo,
    Equals,
    NotEquals,
    Assign,
    BitAndAssign,
    BitOrAssign,
    BitXorAssign,
    BitShiftLeftAssign,
    BitShiftRightAssign,
    BoolAndAssign,
    BoolOrAssign,
    AddAssign,
    SubtractAssign,
    MultiplyAssign,
    ExponentiateAssign,
    ModulateAssign,
}

#[derive(Clone, Debug, PartialEq)]
pub enum Operator {
    // Unary(OperatorUnary),
    // Binary(OperatorBinary),
}

#[derive(Clone, Debug, PartialEq)]
pub enum Value {
    // Literals
    Bool(bool),
    IntUnsigned(ValueIntUnsigned),
    // IntSigned(ValueIntSigned),
    Float(ValueFloat),
    // Character {
    //     value: Vec<u8>,
    //     byte_flag: bool,
    // },
    // String {
    //     value: Vec<u8>,
    //     byte_flag: bool,
    // },
    Null,

    // Symbols
    Symbol {
        name: Symbol,
    },

    // Casting
    Cast {
        value: Box<Value>,
        r#type: Box<Type>,
    },

    // Calling
    Call {
        callee: Box<Value>,
        arguments: Vec<Value>,
    },

    // Operators
    UnaryOperation {
        operator: OperatorUnary,
        argument: Box<Value>,
    },
    BinaryOperation {
        operator: OperatorBinary,
        left: Box<Value>,
        right: Box<Value>,
    },
}

#[derive(Clone, Debug, PartialEq)]
pub enum Statement {
    Value(Box<Value>),
    Block { statements: Vec<Statement> },
}

#[derive(Clone, Debug, PartialEq)]
pub enum FunctionName {
    Symbol(Symbol),
    // Operator(Operator),
}

#[derive(Clone, Debug, PartialEq)]
pub struct EnumEntry {
    pub name: Symbol,
    pub value: Option<Box<Value>>,
}

#[derive(Clone, Debug, PartialEq)]
pub enum Declaration {
    Variable {
        name: Symbol,
        r#type: Option<Box<Type>>,
        value: Box<Value>,
        is_const: bool,
    },

    Function {
        name: FunctionName,
        r#type: TypeFunction,
        body: Option<Box<Statement>>,
        is_prop: bool,
    },
    // Type {
    //     name: Symbol,
    //     base_type: Option<Box<Type>>,
    //     is_explicit: bool,
    //     members: Vec<Declaration>,
    // },

    // Enum {
    //     name: Symbol,
    //     entry_type: Option<Box<Type>>,
    //     entries: Vec<EnumEntry>,
    // },
}
