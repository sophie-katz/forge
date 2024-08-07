use std::fmt;

use chumsky::chain::Chain;

use crate::source::domain::SourceLocation;

#[derive(Clone, Debug, PartialEq)]
pub enum TokenValue {
    // Keywords
    KeywordAs,
    KeywordBool,
    KeywordConst,
    KeywordEnum,
    KeywordExplicit,
    KeywordF32,
    KeywordF64,
    KeywordFn,
    KeywordI16,
    KeywordI32,
    KeywordI64,
    KeywordI8,
    KeywordInt,
    KeywordISize,
    KeywordLet,
    KeywordNever,
    KeywordNull,
    KeywordProp,
    KeywordReturn,
    KeywordSelf,
    KeywordType,
    KeywordU16,
    KeywordU32,
    KeywordU64,
    KeywordU8,
    KeywordUInt,
    KeywordUSize,
    KeywordVoid,

    // Literals
    Bool(bool),
    UnsignedInt { value: u64, width: u32 },
    SignedInt { value: i64, width: u32 },
    Float { value: f64, width: u32 },
    Character { value: Vec<u8>, byte_flag: bool },
    String { value: Vec<u8>, byte_flag: bool },

    // Symbols
    Symbol,

    // Bookends
    LeftParenthesis,
    RightParenthesis,
    LeftCurlyBrace,
    RightCurlyBrace,
    LeftSquareBracket,
    RightSquareBracket,

    // Punctuation
    Period,
    Comma,
    Colon,
    Semicolon,
    RightArrow,
    Ellipsis,

    // Operators
    BitNegate,
    BitAnd,
    BitOr,
    BitXor,
    BitShiftLeft,
    BitShiftRight,
    BoolNot,
    BoolAnd,
    BoolOr,
    Negate,
    Add,
    Increment,
    Subtract,
    Decrement,
    Multiply,
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

impl fmt::Display for TokenValue {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            TokenValue::KeywordAs => write!(f, "'as'"),
            TokenValue::KeywordBool => write!(f, "'bool'"),
            TokenValue::KeywordConst => write!(f, "'const'"),
            TokenValue::KeywordEnum => write!(f, "'enum'"),
            TokenValue::KeywordExplicit => write!(f, "'explicit'"),
            TokenValue::KeywordF32 => write!(f, "'f32'"),
            TokenValue::KeywordF64 => write!(f, "'f64'"),
            TokenValue::KeywordFn => write!(f, "'fn'"),
            TokenValue::KeywordI16 => write!(f, "'i16'"),
            TokenValue::KeywordI32 => write!(f, "'i32'"),
            TokenValue::KeywordI64 => write!(f, "'i64'"),
            TokenValue::KeywordI8 => write!(f, "'i8'"),
            TokenValue::KeywordInt => write!(f, "'int'"),
            TokenValue::KeywordISize => write!(f, "'isize'"),
            TokenValue::KeywordLet => write!(f, "'let'"),
            TokenValue::KeywordNever => write!(f, "'never'"),
            TokenValue::KeywordNull => write!(f, "'null'"),
            TokenValue::KeywordProp => write!(f, "'prop'"),
            TokenValue::KeywordReturn => write!(f, "'return'"),
            TokenValue::KeywordSelf => write!(f, "'self'"),
            TokenValue::KeywordType => write!(f, "'type'"),
            TokenValue::KeywordU16 => write!(f, "'u16'"),
            TokenValue::KeywordU32 => write!(f, "'u32'"),
            TokenValue::KeywordU64 => write!(f, "'u64'"),
            TokenValue::KeywordU8 => write!(f, "'u8'"),
            TokenValue::KeywordUInt => write!(f, "'uint'"),
            TokenValue::KeywordUSize => write!(f, "'usize'"),
            TokenValue::KeywordVoid => write!(f, "'void'"),
            TokenValue::Bool(value) => match value {
                true => write!(f, "true"),
                false => write!(f, "false"),
            },
            TokenValue::UnsignedInt { value, width } => write!(f, "{}u{}", value, width * 8),
            TokenValue::SignedInt { value, width } => write!(f, "{}i{}", value, width * 8),
            TokenValue::Float { value, width } => write!(f, "{}f{}", value, width * 8),
            TokenValue::Character { .. } => todo!(),
            TokenValue::String { .. } => todo!(),
            TokenValue::Symbol => write!(f, "symbol"),
            TokenValue::LeftParenthesis => write!(f, "'('"),
            TokenValue::RightParenthesis => write!(f, "')'"),
            TokenValue::LeftCurlyBrace => write!(f, "'{{'"),
            TokenValue::RightCurlyBrace => write!(f, "'}}'"),
            TokenValue::LeftSquareBracket => write!(f, "'['"),
            TokenValue::RightSquareBracket => write!(f, "']'"),
            TokenValue::Period => write!(f, "'.'"),
            TokenValue::Comma => write!(f, "','"),
            TokenValue::Colon => write!(f, "':'"),
            TokenValue::Semicolon => write!(f, "';'"),
            TokenValue::RightArrow => write!(f, "'->'"),
            TokenValue::Ellipsis => write!(f, "'...'"),
            TokenValue::BitNegate => write!(f, "'~'"),
            TokenValue::BitAnd => write!(f, "'&'"),
            TokenValue::BitOr => write!(f, "'|'"),
            TokenValue::BitXor => write!(f, "'^'"),
            TokenValue::BitShiftLeft => write!(f, "'<<'"),
            TokenValue::BitShiftRight => write!(f, "'>>'"),
            TokenValue::BoolNot => write!(f, "'!'"),
            TokenValue::BoolAnd => write!(f, "'&&'"),
            TokenValue::BoolOr => write!(f, "'||'"),
            TokenValue::Negate => write!(f, "'!'"),
            TokenValue::Add => write!(f, "'+'"),
            TokenValue::Increment => write!(f, "'++'"),
            TokenValue::Subtract => write!(f, "'-'"),
            TokenValue::Decrement => write!(f, "'--'"),
            TokenValue::Multiply => write!(f, "'*'"),
            TokenValue::Exponentiate => write!(f, "'**'"),
            TokenValue::Modulate => write!(f, "'%'"),
            TokenValue::LessThan => write!(f, "'<'"),
            TokenValue::LessThanOrEqualTo => write!(f, "'<='"),
            TokenValue::GreaterThan => write!(f, "'>'"),
            TokenValue::GreaterThanOrEqualTo => write!(f, "'>='"),
            TokenValue::Equals => write!(f, "'=='"),
            TokenValue::NotEquals => write!(f, "'!='"),
            TokenValue::Assign => write!(f, "'='"),
            TokenValue::BitAndAssign => write!(f, "'&='"),
            TokenValue::BitOrAssign => write!(f, "'|='"),
            TokenValue::BitXorAssign => write!(f, "'^='"),
            TokenValue::BitShiftLeftAssign => write!(f, "'<<='"),
            TokenValue::BitShiftRightAssign => write!(f, "'>>='"),
            TokenValue::BoolAndAssign => write!(f, "'&&='"),
            TokenValue::BoolOrAssign => write!(f, "'||='"),
            TokenValue::AddAssign => write!(f, "'+='"),
            TokenValue::SubtractAssign => write!(f, "'-='"),
            TokenValue::MultiplyAssign => write!(f, "'*='"),
            TokenValue::ExponentiateAssign => write!(f, "'**='"),
            TokenValue::ModulateAssign => write!(f, "'%='"),
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Token<'source_context> {
    pub value: TokenValue,
    pub first: SourceLocation<'source_context>,
    pub length: usize,
}

impl<'source_context> Token<'source_context> {
    pub fn new(
        value: TokenValue,
        first: SourceLocation<'source_context>,
        last: &SourceLocation<'source_context>,
    ) -> Self {
        Self {
            value,
            first,
            length: last.offset - first.offset,
        }
    }
}

impl fmt::Display for Token<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "[{: >4}:{: >2}+{: >5}+{: >2}] {}",
            self.first.line_number,
            self.first.column_number,
            self.first.offset,
            self.length,
            self.value
        )
    }
}
