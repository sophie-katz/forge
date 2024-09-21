use crate::{
    domain::ValueWidth,
    lexing::{reader::TokenReader, token::TokenValue},
    message::domain::MessageContext,
};

use super::{
    ast::{
        Declaration, FunctionArgument, FunctionName, OperatorBinary, OperatorUnary, Statement,
        Symbol, Type, TypeBasic, TypeFunction, Value,
    },
    fragments::{
        check_token, parse_first_of, parse_operation_binary, parse_operation_unary_postfix,
        parse_operation_unary_prefix, parse_or_error_and_return_none,
        parse_repeated_bound_or_error_and_return_none,
        parse_repeated_bound_separated_or_error_and_return_none,
        read_token_or_error_and_return_none, read_token_or_return_none,
    },
    matchers::{match_any_token, match_token_with_condition, match_token_with_value},
};

fn parse_type_basic_keyword<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    _message_context: &mut MessageContext<'source_context>,
) -> Option<TypeBasic> {
    // Get token or return none if there are no more
    let token = read_token_or_return_none(token_reader, match_any_token())?;

    // Match value and optionally output AST
    match token.value {
        TokenValue::KeywordBool => Some(TypeBasic::Bool),
        TokenValue::KeywordF32 => Some(TypeBasic::Float(ValueWidth::FixedByteSize(4))),
        TokenValue::KeywordF64 => Some(TypeBasic::Float(ValueWidth::FixedByteSize(8))),
        TokenValue::KeywordI16 => Some(TypeBasic::IntSigned(ValueWidth::FixedByteSize(2))),
        TokenValue::KeywordI32 => Some(TypeBasic::IntSigned(ValueWidth::FixedByteSize(4))),
        TokenValue::KeywordI64 => Some(TypeBasic::IntSigned(ValueWidth::FixedByteSize(8))),
        TokenValue::KeywordI8 => Some(TypeBasic::IntSigned(ValueWidth::FixedByteSize(1))),
        TokenValue::KeywordInt => Some(TypeBasic::IntSigned(ValueWidth::Auto)),
        TokenValue::KeywordISize => Some(TypeBasic::IntSigned(ValueWidth::PointerSize)),
        TokenValue::KeywordNever => Some(TypeBasic::Never),
        TokenValue::KeywordNull => Some(TypeBasic::Null),
        TokenValue::KeywordU16 => Some(TypeBasic::IntUnsigned(ValueWidth::FixedByteSize(2))),
        TokenValue::KeywordU32 => Some(TypeBasic::IntUnsigned(ValueWidth::FixedByteSize(4))),
        TokenValue::KeywordU64 => Some(TypeBasic::IntUnsigned(ValueWidth::FixedByteSize(8))),
        TokenValue::KeywordU8 => Some(TypeBasic::IntUnsigned(ValueWidth::FixedByteSize(1))),
        TokenValue::KeywordUInt => Some(TypeBasic::IntUnsigned(ValueWidth::Auto)),
        TokenValue::KeywordUSize => Some(TypeBasic::IntUnsigned(ValueWidth::PointerSize)),
        TokenValue::KeywordVoid => Some(TypeBasic::Void),
        _ => None,
    }
}

fn parse_type_basic_symbol<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    _message_context: &mut MessageContext<'source_context>,
) -> Option<TypeBasic> {
    // Get token or return none if there are no more
    let token =
        read_token_or_return_none(token_reader, match_token_with_value(&TokenValue::Symbol))?;

    // Output AST
    Some(TypeBasic::Symbol {
        name: Symbol {
            name: token.first.bytes_between(&token.last).to_vec(),
        },
    })
}

fn parse_type_basic<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<TypeBasic> {
    parse_first_of(
        token_reader,
        message_context,
        &[&parse_type_basic_keyword, &parse_type_basic_symbol],
    )
}

fn parse_type_function<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<TypeFunction> {
    // Parse arguments
    let arguments = parse_repeated_bound_separated_or_error_and_return_none(
        token_reader,
        message_context,
        "function type argument",
        |token_reader, message_context| {
            parse_type(token_reader, message_context).map(|r#type| FunctionArgument {
                name: None,
                r#type: Some(Box::new(r#type)),
            })
        },
        &TokenValue::LeftParenthesis,
        &TokenValue::Comma,
        &TokenValue::RightParenthesis,
    )?;

    // Read right arrow or error and return none
    read_token_or_error_and_return_none(
        token_reader,
        message_context,
        match_token_with_value(&TokenValue::RightArrow),
        "expected '->' after type function arguments".to_owned(),
    )?;

    // Parse return type
    let return_type = parse_or_error_and_return_none(
        token_reader,
        message_context,
        parse_type,
        "expected return type".to_owned(),
    )?;

    Some(TypeFunction {
        arguments,
        return_type: Some(Box::new(return_type)),
    })
}

fn parse_type<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Type> {
    parse_first_of(
        token_reader,
        message_context,
        &[
            &|token_reader, message_context| {
                Some(Type::Function(parse_type_function(
                    token_reader,
                    message_context,
                )?))
            },
            &|token_reader, message_context| {
                Some(Type::Basic(parse_type_basic(
                    token_reader,
                    message_context,
                )?))
            },
        ],
    )
}

fn parse_value_bool<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    _message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    // Get token or return none if there are no more
    let token = read_token_or_return_none(token_reader, match_any_token())?;

    // Match value and optionally output AST
    match token.value {
        TokenValue::Bool(value) => Some(Value::Bool(value)),
        _ => None,
    }
}

fn parse_value_int_unsigned<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    _message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    // Get token or return none if there are no more
    let token = read_token_or_return_none(token_reader, match_any_token())?;

    // Match value and optionally output AST
    match &token.value {
        TokenValue::IntUnsigned(value) => Some(Value::IntUnsigned(value.clone())),
        _ => None,
    }
}

// fn parse_value_int_signed<'source_context>(
//     token_reader: &mut TokenReader<'source_context>,
//     _message_context: &mut MessageContext<'source_context>,
// ) -> Option<Value> {
//     // Get token or return none if there are no more
//     let token = read_token_or_return_none(token_reader, match_any_token())?;

//     // Match value and optionally output AST
//     match &token.value {
//         TokenValue::IntSigned(value) => Some(Value::IntSigned(value.clone())),
//         _ => None,
//     }
// }

fn parse_value_float<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    _message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    // Get token or return none if there are no more
    let token = read_token_or_return_none(token_reader, match_any_token())?;

    // Match value and optionally output AST
    match &token.value {
        TokenValue::Float(value) => Some(Value::Float(value.clone())),
        _ => None,
    }
}

fn parse_value_term_keyword<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    _message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    // Get token or return none if there are no more
    let token = read_token_or_return_none(token_reader, match_any_token())?;

    // Match value and optionally output AST
    match &token.value {
        TokenValue::KeywordNull => Some(Value::Null),
        _ => None,
    }
}

fn parse_value_symbol<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    _message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    // Get token or return none if there are no more
    let token =
        read_token_or_return_none(token_reader, match_token_with_value(&TokenValue::Symbol))?;

    // Output AST
    Some(Value::Symbol {
        name: Symbol {
            name: token.first.bytes_between(&token.last).to_vec(),
        },
    })
}

fn parse_value_term<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_first_of(
        token_reader,
        message_context,
        &[
            &parse_value_bool,
            &parse_value_int_unsigned,
            // &parse_value_int_signed,
            &parse_value_float,
            &parse_value_term_keyword,
            &parse_value_symbol,
        ],
    )
}

fn parse_value_call<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    // Parse callee
    let callee = parse_value_term(token_reader, message_context)?;

    // If there is no call, just return the callee
    if !check_token(
        token_reader,
        match_token_with_value(&TokenValue::LeftParenthesis),
    ) {
        return Some(callee);
    }

    let arguments = parse_repeated_bound_separated_or_error_and_return_none(
        token_reader,
        message_context,
        "argument",
        parse_value,
        &TokenValue::LeftParenthesis,
        &TokenValue::Comma,
        &TokenValue::RightParenthesis,
    )?;

    Some(Value::Call {
        callee: Box::new(callee),
        arguments,
    })
}

fn parse_value_incrementing<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_unary_postfix(
        token_reader,
        message_context,
        &[
            (&TokenValue::Increment, OperatorUnary::Increment),
            (&TokenValue::Decrement, OperatorUnary::Decrement),
        ],
        parse_value_call,
    )
}

fn parse_value_exponentiation<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[(&TokenValue::Exponentiate, OperatorBinary::Exponentiate)],
        parse_value_incrementing,
        parse_value_exponentiation,
    )
}

fn parse_value_negation<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_unary_prefix(
        token_reader,
        message_context,
        &[
            (&TokenValue::Subtract, OperatorUnary::Negate),
            (&TokenValue::Add, OperatorUnary::Positive),
            (&TokenValue::BitNegate, OperatorUnary::BitNegate),
        ],
        parse_value_exponentiation,
        parse_value_negation,
    )
}

fn parse_value_multiplication<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[
            (&TokenValue::Multiply, OperatorBinary::Multiply),
            (&TokenValue::Divide, OperatorBinary::Divide),
            (&TokenValue::Modulate, OperatorBinary::Modulate),
        ],
        parse_value_negation,
        parse_value_multiplication,
    )
}

fn parse_value_addition<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[
            (&TokenValue::Add, OperatorBinary::Add),
            (&TokenValue::Subtract, OperatorBinary::Subtract),
        ],
        parse_value_multiplication,
        parse_value_addition,
    )
}

fn parse_value_bit_shift<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[
            (&TokenValue::BitShiftLeft, OperatorBinary::BitShiftLeft),
            (&TokenValue::BitShiftRight, OperatorBinary::BitShiftRight),
        ],
        parse_value_addition,
        parse_value_bit_shift,
    )
}

fn parse_value_bit_and<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[(&TokenValue::BitAnd, OperatorBinary::BitAnd)],
        parse_value_bit_shift,
        parse_value_bit_and,
    )
}

fn parse_value_bit_xor<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[(&TokenValue::BitXor, OperatorBinary::BitXor)],
        parse_value_bit_and,
        parse_value_bit_xor,
    )
}

fn parse_value_bit_or<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[(&TokenValue::BitOr, OperatorBinary::BitOr)],
        parse_value_bit_xor,
        parse_value_bit_or,
    )
}

fn parse_value_compare<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[
            (&TokenValue::Equals, OperatorBinary::Equals),
            (&TokenValue::NotEquals, OperatorBinary::NotEquals),
            (&TokenValue::LessThan, OperatorBinary::LessThan),
            (
                &TokenValue::LessThanOrEqualTo,
                OperatorBinary::LessThanOrEqualTo,
            ),
            (&TokenValue::GreaterThan, OperatorBinary::GreaterThan),
            (
                &TokenValue::GreaterThanOrEqualTo,
                OperatorBinary::GreaterThanOrEqualTo,
            ),
        ],
        parse_value_bit_or,
        parse_value_compare,
    )
}

fn parse_value_bool_not<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_unary_prefix(
        token_reader,
        message_context,
        &[(&TokenValue::BoolNot, OperatorUnary::BoolNot)],
        parse_value_compare,
        parse_value_bool_not,
    )
}

fn parse_value_bool_and<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[(&TokenValue::BoolAnd, OperatorBinary::BoolAnd)],
        parse_value_bool_not,
        parse_value_bool_and,
    )
}

fn parse_value_bool_or<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[(&TokenValue::BoolOr, OperatorBinary::BoolOr)],
        parse_value_bool_and,
        parse_value_bool_or,
    )
}

fn parse_value_casting<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    let value = parse_value_bool_or(token_reader, message_context)?;

    let token = match token_reader.peek_next() {
        Some(token) => token,
        None => return Some(value),
    };

    match token.value {
        TokenValue::KeywordAs => {
            token_reader.read_next();

            let r#type = parse_or_error_and_return_none(
                token_reader,
                message_context,
                parse_type,
                "expected type after 'as'".to_owned(),
            )?;

            return Some(Value::Cast {
                value: Box::new(value),
                r#type: Box::new(r#type),
            });
        }
        _ => Some(value),
    }
}

fn parse_value_assign<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_operation_binary(
        token_reader,
        message_context,
        &[
            (&TokenValue::Assign, OperatorBinary::Assign),
            (&TokenValue::BitAndAssign, OperatorBinary::BitAndAssign),
            (&TokenValue::BitOrAssign, OperatorBinary::BitOrAssign),
            (&TokenValue::BitXorAssign, OperatorBinary::BitXorAssign),
            (
                &TokenValue::BitShiftLeftAssign,
                OperatorBinary::BitShiftLeftAssign,
            ),
            (
                &TokenValue::BitShiftRightAssign,
                OperatorBinary::BitShiftRightAssign,
            ),
            (&TokenValue::BoolAndAssign, OperatorBinary::BoolAndAssign),
            (&TokenValue::BoolOrAssign, OperatorBinary::BoolOrAssign),
            (&TokenValue::AddAssign, OperatorBinary::AddAssign),
            (&TokenValue::SubtractAssign, OperatorBinary::SubtractAssign),
            (&TokenValue::MultiplyAssign, OperatorBinary::MultiplyAssign),
            (
                &TokenValue::ExponentiateAssign,
                OperatorBinary::ExponentiateAssign,
            ),
            (&TokenValue::ModulateAssign, OperatorBinary::ModulateAssign),
        ],
        parse_value_casting,
        parse_value_assign,
    )
}

fn parse_value<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Value> {
    parse_value_assign(token_reader, message_context)
}

fn parse_statement_value<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Statement> {
    let value = parse_value(token_reader, message_context)?;

    read_token_or_error_and_return_none(
        token_reader,
        message_context,
        match_token_with_value(&TokenValue::Semicolon),
        "expected ';' after value".to_owned(),
    )?;

    Some(Statement::Value(Box::new(value)))
}

fn parse_statement_block<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Statement> {
    let statements = parse_repeated_bound_or_error_and_return_none(
        token_reader,
        message_context,
        "statement",
        parse_statement,
        &TokenValue::LeftCurlyBrace,
        &TokenValue::RightCurlyBrace,
    )?;

    Some(Statement::Block { statements })
}

fn parse_statement<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Statement> {
    parse_first_of(
        token_reader,
        message_context,
        &[&parse_statement_block, &parse_statement_value],
    )
}

fn parse_declaration_variable<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Declaration> {
    // Read 'let' or 'const' or return none
    let is_const = read_token_or_return_none(
        token_reader,
        match_token_with_condition(|value| {
            *value == TokenValue::KeywordLet || *value == TokenValue::KeywordConst
        }),
    )?
    .value
        == TokenValue::KeywordConst;

    // Read variable name or return none
    let name =
        read_token_or_return_none(token_reader, match_token_with_value(&TokenValue::Symbol))?;
    let name = name.first.bytes_between(&name.last).to_vec();

    // Parse type
    let mut r#type = None;
    if check_token(token_reader, match_token_with_value(&TokenValue::Colon)) {
        token_reader.read_next();

        // Parse type
        r#type = Some(Box::new(parse_or_error_and_return_none(
            token_reader,
            message_context,
            parse_type,
            "expected type after ':'".to_owned(),
        )?));
    }

    // Read equals or return none
    read_token_or_error_and_return_none(
        token_reader,
        message_context,
        match_token_with_value(&TokenValue::Assign),
        "expected '=' in variable declaration".to_owned(),
    )?;

    // Parse value
    let value = parse_or_error_and_return_none(
        token_reader,
        message_context,
        parse_value,
        "expected value after variable type".to_owned(),
    )?;

    // Read semicolon or return none
    read_token_or_error_and_return_none(
        token_reader,
        message_context,
        match_token_with_value(&TokenValue::Semicolon),
        "expected ';' after variable declaration".to_owned(),
    )?;

    Some(Declaration::Variable {
        name: Symbol { name },
        r#type,
        value: Box::new(value),
        is_const,
    })
}

fn parse_declaration_function<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Declaration> {
    // Read 'fn' or return none
    read_token_or_return_none(token_reader, match_token_with_value(&TokenValue::KeywordFn))?;

    // Read variable name or return none
    let name =
        read_token_or_return_none(token_reader, match_token_with_value(&TokenValue::Symbol))?;
    let name = name.first.bytes_between(&name.last).to_vec();

    // Parse arguments
    parse_or_error_and_return_none(
        token_reader,
        message_context,
        |token_reader, message_context| {
            parse_repeated_bound_separated_or_error_and_return_none(
                token_reader,
                message_context,
                "function argument",
                |token_reader, message_context| {
                    parse_type(token_reader, message_context).map(|r#type| FunctionArgument {
                        name: None,
                        r#type: Some(Box::new(r#type)),
                    })
                },
                &TokenValue::LeftParenthesis,
                &TokenValue::Comma,
                &TokenValue::RightParenthesis,
            )
        },
        "expected function arguments".to_owned(),
    )?;

    // Read right arrow or error and return none
    read_token_or_error_and_return_none(
        token_reader,
        message_context,
        match_token_with_value(&TokenValue::RightArrow),
        "expected '->' after function arguments".to_owned(),
    )?;

    // Parse return type
    let return_type = parse_or_error_and_return_none(
        token_reader,
        message_context,
        parse_type,
        "expected return type".to_owned(),
    )?;

    // Parse body
    let body = parse_or_error_and_return_none(
        token_reader,
        message_context,
        |token_reader, message_context| {
            parse_statement_block(token_reader, message_context).map(Box::new)
        },
        "expected function body".to_owned(),
    )?;

    Some(Declaration::Function {
        name: FunctionName::Symbol(Symbol { name }),
        r#type: TypeFunction {
            arguments: vec![],
            return_type: Some(Box::new(return_type)),
        },
        body: Some(body),
        is_prop: false,
    })
}

fn parse_declaration<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Declaration> {
    parse_first_of(
        token_reader,
        message_context,
        &[&parse_declaration_variable, &parse_declaration_function],
    )
}

pub fn parse<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Vec<Declaration>> {
    let mut declarations = vec![];

    while token_reader.is_more() {
        declarations.push(parse_or_error_and_return_none(
            token_reader,
            message_context,
            parse_declaration,
            "expected declaration".to_owned(),
        )?);
    }

    Some(declarations)
}

#[cfg(test)]
mod tests {
    use crate::{lexing::token::Token, source::domain::SourceContext};

    use super::*;

    #[test]
    fn test_type_bool_good() {
        let source_context = SourceContext::new();
        let mut token_reader = TokenReader::new(vec![Token::new_test(
            TokenValue::KeywordBool,
            &source_context,
        )]);
        let mut message_context = MessageContext::new();

        assert_eq!(
            parse_type_basic(&mut token_reader, &mut message_context),
            Some(TypeBasic::Bool)
        );

        assert_eq!(token_reader.is_more(), false);
        assert_eq!(message_context.error_count(), 0);
    }

    #[test]
    fn test_type_bool_bad() {
        let source_context = SourceContext::new();
        let mut token_reader = TokenReader::new(vec![Token::new_test(
            TokenValue::KeywordAs,
            &source_context,
        )]);
        let mut message_context = MessageContext::new();

        assert_eq!(
            parse_type_basic(&mut token_reader, &mut message_context),
            None
        );

        assert_eq!(message_context.error_count(), 0);
    }

    #[test]
    #[ignore]
    fn test_type_basic_symbol_good() {
        let source_context = SourceContext::new();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);
        let mut message_context = MessageContext::new();

        assert_eq!(
            parse_type_basic_symbol(&mut token_reader, &mut message_context),
            Some(TypeBasic::Symbol {
                name: Symbol { name: Vec::new() }
            })
        );

        assert_eq!(token_reader.is_more(), false);
        assert_eq!(message_context.error_count(), 0);
    }

    #[test]
    fn test_type_basic_symbol_bad() {
        let source_context = SourceContext::new();
        let mut token_reader = TokenReader::new(vec![Token::new_test(
            TokenValue::KeywordAs,
            &source_context,
        )]);
        let mut message_context = MessageContext::new();

        assert_eq!(
            parse_type_basic_symbol(&mut token_reader, &mut message_context),
            None
        );

        assert_eq!(message_context.error_count(), 0);
    }

    #[test]
    fn test_parse_value_call() {
        let source_context = SourceContext::new();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::Bool(true), &source_context),
            Token::new_test(TokenValue::LeftParenthesis, &source_context),
            Token::new_test(TokenValue::RightParenthesis, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        assert_eq!(
            parse_value_call(&mut token_reader, &mut message_context),
            Some(Value::Call {
                callee: Box::new(Value::Bool(true)),
                arguments: Vec::new()
            })
        );

        assert_eq!(token_reader.is_more(), false);
        assert_eq!(message_context.error_count(), 0);
    }

    #[test]
    fn test_parse_statement_value() {
        let source_context = SourceContext::new();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::Bool(true), &source_context),
            Token::new_test(TokenValue::Semicolon, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        assert_eq!(
            parse_statement_value(&mut token_reader, &mut message_context),
            Some(Statement::Value(Box::new(Value::Bool(true)),))
        );

        assert_eq!(token_reader.is_more(), false);
        assert_eq!(message_context.error_count(), 0);
    }

    #[test]
    #[ignore]
    fn test_parse_declaration_variable() {
        let source_context = SourceContext::new();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::KeywordLet, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Assign, &source_context),
            Token::new_test(TokenValue::Bool(true), &source_context),
            Token::new_test(TokenValue::Semicolon, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        assert_eq!(
            parse_declaration_variable(&mut token_reader, &mut message_context),
            Some(Declaration::Variable {
                name: Symbol { name: Vec::new() },
                r#type: None,
                value: Box::new(Value::Bool(true)),
                is_const: false
            })
        );

        assert_eq!(token_reader.is_more(), false);
        assert_eq!(message_context.error_count(), 0);
    }
}
