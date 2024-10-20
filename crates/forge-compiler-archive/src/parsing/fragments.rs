//! Parser fragments that can be reused.

use crate::{
    lexing::{
        reader::TokenReader,
        token::{Token, TokenValue},
    },
    message::domain::{Message, MessageContext, Severity},
};

use super::{
    ast::{OperatorBinary, OperatorUnary, Value},
    matchers::match_token_with_value,
};

pub fn check_token<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    matcher: impl Fn(&Token<'source_context>) -> bool,
) -> bool {
    // Check if the next token matches
    match token_reader.peek_next() {
        Some(token) if matcher(token) => true,
        _ => false,
    }
}

pub fn read_token_or_return_none<'source_context, 'token_reader>(
    token_reader: &'token_reader mut TokenReader<'source_context>,
    matcher: impl Fn(&Token<'source_context>) -> bool,
) -> Option<&'token_reader Token<'source_context>> {
    // Get token or return none if there are no more
    match token_reader.read_next() {
        Some(token) if matcher(token) => Some(token),
        _ => None,
    }
}

pub fn read_token_or_error_and_return_none<'source_context, 'token_reader>(
    token_reader: &'token_reader mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
    matcher: impl Fn(&Token<'source_context>) -> bool,
    message_text: String,
) -> Option<&'token_reader Token<'source_context>> {
    // Get token or return none if there are no more
    match token_reader.read_next() {
        Some(token) if matcher(token) => Some(token),
        Some(token) => {
            message_context.add(Message::new_from_range(
                Severity::Error,
                token.first.clone(),
                token.last.clone(),
                message_text,
            ));
            None
        }
        None => {
            message_context.add(Message::new(
                Severity::Error,
                "unexpected end of file".to_owned(),
            ));
            None
        }
    }
}

pub fn parse_or_error_and_return_none<'source_context, Output>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
    parser: impl Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Output>,
    message_text: String,
) -> Option<Output> {
    // Save state
    let state_start = token_reader.save_state();

    // Parse or return none
    match parser(token_reader, message_context) {
        Some(output) => Some(output),
        None => {
            token_reader.restore_state(state_start.clone());

            if let Some(token) = token_reader.peek_next() {
                message_context.add(Message::new_from_range(
                    Severity::Error,
                    token.first.clone(),
                    token.last.clone(),
                    message_text,
                ));
            } else {
                message_context.add(Message::new(Severity::Error, message_text));
            }

            None
        }
    }
}

pub fn parse_first_of<'source_context, Output>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
    parsers: &[&dyn Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Output>],
) -> Option<Output> {
    // Save state
    let state_start = token_reader.save_state();

    // Try to parse each parser
    for parser in parsers {
        if let Some(output) = parser(token_reader, message_context) {
            return Some(output);
        }

        // Reset and try next parser
        token_reader.restore_state(state_start.clone());
    }

    // Reset and return none
    token_reader.restore_state(state_start);

    None
}

pub fn parse_repeated_bound_separated_or_error_and_return_none<'source_context, Output>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
    item_name: &str,
    parser: impl Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Output>,
    opener: &TokenValue,
    separator: &TokenValue,
    closer: &TokenValue,
) -> Option<Vec<Output>> {
    // Read opener or return none
    read_token_or_return_none(token_reader, match_token_with_value(opener))?;

    let mut items = Vec::new();

    // Return an empty vector if we have an empty sequence
    if check_token(token_reader, match_token_with_value(closer)) {
        token_reader.read_next();
        return Some(items);
    }

    while token_reader.is_more() {
        // Parse item or error and return none
        let item = parse_or_error_and_return_none(
            token_reader,
            message_context,
            &parser,
            format!("expected {item_name}"),
        )?;

        items.push(item);

        // Check if we have a closer
        if check_token(token_reader, match_token_with_value(closer)) {
            break;
        }

        // Consume separator or error and return none
        read_token_or_error_and_return_none(
            token_reader,
            message_context,
            match_token_with_value(separator),
            format!("expected {separator}"),
        )?;

        // Check if we have a closer after final separator
        if check_token(token_reader, match_token_with_value(closer)) {
            break;
        }
    }

    // Consume closer or error and return none
    read_token_or_error_and_return_none(
        token_reader,
        message_context,
        match_token_with_value(closer),
        format!("expected closing {closer} after opening {opener}"),
    )?;

    Some(items)
}

pub fn parse_repeated_bound_or_error_and_return_none<'source_context, Output>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
    item_name: &str,
    parser: impl Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Output>,
    opener: &TokenValue,
    closer: &TokenValue,
) -> Option<Vec<Output>> {
    // Read opener or return none
    read_token_or_return_none(token_reader, match_token_with_value(opener))?;

    let mut items = Vec::new();

    // Return an empty vector if we have an empty sequence
    if check_token(token_reader, match_token_with_value(closer)) {
        return Some(items);
    }

    while token_reader.is_more() {
        // Parse item or error and return none
        let item = parse_or_error_and_return_none(
            token_reader,
            message_context,
            &parser,
            format!("expected {item_name}"),
        )?;

        items.push(item);

        // Check if we have a closer
        if check_token(token_reader, match_token_with_value(closer)) {
            break;
        }
    }

    // Consume closer or error and return none
    read_token_or_error_and_return_none(
        token_reader,
        message_context,
        match_token_with_value(closer),
        format!("expected closing {closer} after opening {opener}"),
    )?;

    Some(items)
}

pub fn parse_operation_unary_prefix<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
    operator_token_pairs: &[(&TokenValue, OperatorUnary)],
    parser_fallback: impl Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Value>,
    parser_argument: impl Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Value>,
) -> Option<Value> {
    let token = match token_reader.peek_next() {
        Some(token) => token,
        None => return None,
    };

    for (operator_token, operator) in operator_token_pairs {
        if token.value == **operator_token {
            token_reader.read_next();

            let argument = parse_or_error_and_return_none(
                token_reader,
                message_context,
                parser_argument,
                format!("expected argument of {operator_token}"),
            )?;

            return Some(Value::UnaryOperation {
                operator: operator.clone(),
                argument: Box::new(argument),
            });
        }
    }

    parser_fallback(token_reader, message_context)
}

pub fn parse_operation_unary_postfix<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
    operator_token_pairs: &[(&TokenValue, OperatorUnary)],
    parser_argument: impl Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Value>,
) -> Option<Value> {
    let argument = parser_argument(token_reader, message_context)?;

    let token = match token_reader.peek_next() {
        Some(token) => token,
        None => return Some(argument),
    };

    for (operator_token, operator) in operator_token_pairs {
        if token.value == **operator_token {
            token_reader.read_next();

            return Some(Value::UnaryOperation {
                operator: operator.clone(),
                argument: Box::new(argument),
            });
        }
    }

    Some(argument)
}

pub fn parse_operation_binary<'source_context>(
    token_reader: &mut TokenReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
    operator_token_pairs: &[(&TokenValue, OperatorBinary)],
    parser_left: impl Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Value>,
    parser_right: impl Fn(
        &mut TokenReader<'source_context>,
        &mut MessageContext<'source_context>,
    ) -> Option<Value>,
) -> Option<Value> {
    let left = parser_left(token_reader, message_context)?;

    let token = match token_reader.peek_next() {
        Some(token) => token,
        None => return Some(left),
    };

    for (operator_token, operator) in operator_token_pairs {
        if token.value == **operator_token {
            token_reader.read_next();

            let right = parse_or_error_and_return_none(
                token_reader,
                message_context,
                parser_right,
                format!("expected right-hand argument of {operator_token}"),
            )?;

            return Some(Value::BinaryOperation {
                operator: operator.clone(),
                left: Box::new(left),
                right: Box::new(right),
            });
        }
    }

    Some(left)
}

#[cfg(test)]
mod tests {
    use crate::{
        parsing::{
            ast::Symbol,
            matchers::{match_any_token, match_token_with_condition},
        },
        source::domain::SourceContext,
    };

    use super::*;

    #[test]
    fn test_check_token_no_more() {
        let mut token_reader = TokenReader::new(vec![]);

        assert!(!check_token(&mut token_reader, match_any_token()));
    }

    #[test]
    fn test_check_token_matching() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);

        assert!(check_token(&mut token_reader, match_any_token()));
    }

    #[test]
    fn test_check_token_not_matching() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);

        assert!(!check_token(
            &mut token_reader,
            match_token_with_condition(|_| false)
        ));
    }

    #[test]
    fn test_read_token_or_return_none_no_more() {
        let mut token_reader = TokenReader::new(vec![]);

        assert!(read_token_or_return_none(&mut token_reader, match_any_token()).is_none());
    }

    #[test]
    fn test_read_token_or_return_none_matching() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);

        assert!(read_token_or_return_none(&mut token_reader, match_any_token()).is_some());
    }

    #[test]
    fn test_read_token_or_return_none_not_matching() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);

        assert!(read_token_or_return_none(
            &mut token_reader,
            match_token_with_value(&TokenValue::Add)
        )
        .is_none());
    }

    #[test]
    fn read_token_or_error_and_return_none_no_more() {
        let mut token_reader = TokenReader::new(vec![]);
        let mut message_context = MessageContext::new();

        assert!(read_token_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            match_any_token(),
            "expected token".to_owned()
        )
        .is_none());

        assert_eq!(message_context.error_count(), 1);
    }

    #[test]
    fn read_token_or_error_and_return_none_matching() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);
        let mut message_context = MessageContext::new();

        assert!(read_token_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            match_any_token(),
            "expected token".to_owned()
        )
        .is_some());

        assert_eq!(message_context.error_count(), 0);
    }

    #[test]
    fn read_token_or_error_and_return_none_not_matching() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);
        let mut message_context = MessageContext::new();

        assert!(read_token_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            match_token_with_value(&TokenValue::Add),
            "expected token".to_owned()
        )
        .is_none());

        assert_eq!(message_context.error_count(), 1);
    }

    #[test]
    fn test_parse_or_error_and_return_none_none() {
        let mut token_reader = TokenReader::new(vec![]);
        let mut message_context = MessageContext::new();

        assert!(parse_or_error_and_return_none::<()>(
            &mut token_reader,
            &mut message_context,
            |_, _| None,
            "expected token".to_owned()
        )
        .is_none());

        assert_eq!(message_context.error_count(), 1);
    }

    #[test]
    fn test_parse_or_error_and_return_none_some() {
        let mut token_reader = TokenReader::new(vec![]);
        let mut message_context = MessageContext::new();

        assert!(parse_or_error_and_return_none::<()>(
            &mut token_reader,
            &mut message_context,
            |_, _| Some(()),
            "expected token".to_owned()
        )
        .is_some());

        assert_eq!(message_context.error_count(), 0);
    }

    #[test]
    fn test_parse_repeated_bound_separated_or_error_and_return_none_simple() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::LeftParenthesis, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Comma, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::RightParenthesis, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let items = parse_repeated_bound_separated_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            "item",
            |token_reader, _| {
                token_reader.read_next();
                Some(5)
            },
            &TokenValue::LeftParenthesis,
            &TokenValue::Comma,
            &TokenValue::RightParenthesis,
        );

        assert!(items.is_some());
        assert_eq!(items.unwrap().len(), 2);
    }

    #[test]
    fn test_parse_repeated_bound_separated_or_error_and_return_none_no_closer() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::LeftParenthesis, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Comma, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let items = parse_repeated_bound_separated_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            "item",
            |token_reader, _| {
                token_reader.read_next();
                Some(5)
            },
            &TokenValue::LeftParenthesis,
            &TokenValue::Comma,
            &TokenValue::RightParenthesis,
        );

        assert_eq!(message_context.error_count(), 1);
        assert!(items.is_none());
    }

    #[test]
    fn test_parse_repeated_bound_separated_or_error_and_return_none_no_opener() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Comma, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::RightParenthesis, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let items = parse_repeated_bound_separated_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            "item",
            |token_reader, _| {
                token_reader.read_next();
                Some(5)
            },
            &TokenValue::LeftParenthesis,
            &TokenValue::Comma,
            &TokenValue::RightParenthesis,
        );

        assert_eq!(message_context.error_count(), 0);
        assert!(items.is_none());
    }

    #[test]
    fn test_parse_repeated_bound_separated_or_error_and_return_none_no_separator() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::LeftParenthesis, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::RightParenthesis, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let items = parse_repeated_bound_separated_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            "item",
            |token_reader, _| {
                token_reader.read_next();
                Some(5)
            },
            &TokenValue::LeftParenthesis,
            &TokenValue::Comma,
            &TokenValue::RightParenthesis,
        );

        assert_eq!(message_context.error_count(), 1);
        assert!(items.is_none());
    }

    #[test]
    fn test_parse_repeated_bound_or_error_and_return_none_simple() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::LeftParenthesis, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::RightParenthesis, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let items = parse_repeated_bound_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            "item",
            |token_reader, _| {
                token_reader.read_next();
                Some(5)
            },
            &TokenValue::LeftParenthesis,
            &TokenValue::RightParenthesis,
        );

        assert!(items.is_some());
        assert_eq!(items.unwrap().len(), 2);
    }

    #[test]
    fn test_parse_repeated_bound_or_error_and_return_none_no_closer() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::LeftParenthesis, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let items = parse_repeated_bound_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            "item",
            |token_reader, _| {
                token_reader.read_next();
                Some(5)
            },
            &TokenValue::LeftParenthesis,
            &TokenValue::RightParenthesis,
        );

        assert_eq!(message_context.error_count(), 1);
        assert!(items.is_none());
    }

    #[test]
    fn test_parse_repeated_bound_or_error_and_return_none_no_opener() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::RightParenthesis, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let items = parse_repeated_bound_or_error_and_return_none(
            &mut token_reader,
            &mut message_context,
            "item",
            |token_reader, _| {
                token_reader.read_next();
                Some(5)
            },
            &TokenValue::LeftParenthesis,
            &TokenValue::RightParenthesis,
        );

        assert_eq!(message_context.error_count(), 0);
        assert!(items.is_none());
    }

    #[test]
    fn test_parse_operation_unary_prefix_operation() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::Subtract, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let value = parse_operation_unary_prefix(
            &mut token_reader,
            &mut message_context,
            &[(&TokenValue::Subtract, OperatorUnary::Negate)],
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
        );

        assert!(value.is_some());
        assert_eq!(
            value.unwrap(),
            Value::UnaryOperation {
                operator: OperatorUnary::Negate,
                argument: Box::new(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            }
        );
    }

    #[test]
    fn test_parse_operation_unary_prefix_fallback() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);
        let mut message_context = MessageContext::new();

        let value = parse_operation_unary_prefix(
            &mut token_reader,
            &mut message_context,
            &[(&TokenValue::Subtract, OperatorUnary::Negate)],
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
        );

        assert!(value.is_some());
        assert_eq!(
            value.unwrap(),
            Value::Symbol {
                name: Symbol {
                    name: "x".bytes().collect(),
                },
            }
        );
    }

    #[test]
    fn test_parse_operation_unary_postfix_operation() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Subtract, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let value = parse_operation_unary_postfix(
            &mut token_reader,
            &mut message_context,
            &[(&TokenValue::Subtract, OperatorUnary::Negate)],
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
        );

        assert!(value.is_some());
        assert_eq!(
            value.unwrap(),
            Value::UnaryOperation {
                operator: OperatorUnary::Negate,
                argument: Box::new(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            }
        );
    }

    #[test]
    fn test_parse_operation_unary_postfix_fallback() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);
        let mut message_context = MessageContext::new();

        let value = parse_operation_unary_postfix(
            &mut token_reader,
            &mut message_context,
            &[(&TokenValue::Subtract, OperatorUnary::Negate)],
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
        );

        assert!(value.is_some());
        assert_eq!(
            value.unwrap(),
            Value::Symbol {
                name: Symbol {
                    name: "x".bytes().collect(),
                },
            }
        );
    }

    #[test]
    fn test_parse_operation_binary_operation() {
        let source_context = SourceContext::default();
        let mut token_reader = TokenReader::new(vec![
            Token::new_test(TokenValue::Symbol, &source_context),
            Token::new_test(TokenValue::Add, &source_context),
            Token::new_test(TokenValue::Symbol, &source_context),
        ]);
        let mut message_context = MessageContext::new();

        let value = parse_operation_binary(
            &mut token_reader,
            &mut message_context,
            &[(&TokenValue::Add, OperatorBinary::Add)],
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
        );

        assert!(value.is_some());
        assert_eq!(
            value.unwrap(),
            Value::BinaryOperation {
                operator: OperatorBinary::Add,
                left: Box::new(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                }),
                right: Box::new(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                }),
            }
        );
    }

    #[test]
    fn test_parse_operation_binary_fallback() {
        let source_context = SourceContext::default();
        let mut token_reader =
            TokenReader::new(vec![Token::new_test(TokenValue::Symbol, &source_context)]);
        let mut message_context = MessageContext::new();

        let value = parse_operation_binary(
            &mut token_reader,
            &mut message_context,
            &[(&TokenValue::Add, OperatorBinary::Add)],
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
            |token_reader, _| {
                token_reader.read_next();
                Some(Value::Symbol {
                    name: Symbol {
                        name: "x".bytes().collect(),
                    },
                })
            },
        );

        assert!(value.is_some());
        assert_eq!(
            value.unwrap(),
            Value::Symbol {
                name: Symbol {
                    name: "x".bytes().collect(),
                },
            }
        );
    }
}
