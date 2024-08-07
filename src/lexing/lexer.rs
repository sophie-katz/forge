use crate::{
    message::domain::{Message, MessageContext, Severity},
    source::{
        domain::{SourceLocation, SourceRef},
        reader::SourceReader,
    },
};

use super::{
    numbers::{parse_float, parse_int},
    token::{Token, TokenValue},
};

fn lex_next_symbol<'source_context>(
    reader: &mut SourceReader<'source_context>,
) -> Token<'source_context> {
    let first = reader.save_state();

    while let Some(byte) = reader.peek_next() {
        match byte {
            b'a'..=b'z' | b'A'..=b'Z' | b'_' | 0x80..=u8::MAX | b'0'..=b'9' => {
                reader.read_next();
            }

            _ => break,
        }
    }

    let bytes_between = first.bytes_between(reader.current_location());

    // Handle keywords with symbols being a fallback
    if bytes_between == [b'a', b's'] {
        Token::new(TokenValue::KeywordAs, first, reader.current_location())
    } else if bytes_between == [b'b', b'o', b'o', b'l'] {
        Token::new(TokenValue::KeywordBool, first, reader.current_location())
    } else if bytes_between == [b'c', b'o', b'n', b's', b't'] {
        Token::new(TokenValue::KeywordConst, first, reader.current_location())
    } else if bytes_between == [b'e', b'n', b'u', b'm'] {
        Token::new(TokenValue::KeywordEnum, first, reader.current_location())
    } else if bytes_between == [b'e', b'x', b'p', b'l', b'i', b'c', b'i', b't'] {
        Token::new(
            TokenValue::KeywordExplicit,
            first,
            reader.current_location(),
        )
    } else if bytes_between == [b'f', b'3', b'2'] {
        Token::new(TokenValue::KeywordF32, first, reader.current_location())
    } else if bytes_between == [b'f', b'6', b'4'] {
        Token::new(TokenValue::KeywordF64, first, reader.current_location())
    } else if bytes_between == [b'f', b'n'] {
        Token::new(TokenValue::KeywordFn, first, reader.current_location())
    } else if bytes_between == [b'i', b'1', b'6'] {
        Token::new(TokenValue::KeywordI16, first, reader.current_location())
    } else if bytes_between == [b'i', b'3', b'2'] {
        Token::new(TokenValue::KeywordI32, first, reader.current_location())
    } else if bytes_between == [b'i', b'6', b'4'] {
        Token::new(TokenValue::KeywordI64, first, reader.current_location())
    } else if bytes_between == [b'i', b'8'] {
        Token::new(TokenValue::KeywordI8, first, reader.current_location())
    } else if bytes_between == [b'i', b'n', b't'] {
        Token::new(TokenValue::KeywordInt, first, reader.current_location())
    } else if bytes_between == [b'i', b's', b'i', b'z', b'e'] {
        Token::new(TokenValue::KeywordISize, first, reader.current_location())
    } else if bytes_between == [b'l', b'e', b't'] {
        Token::new(TokenValue::KeywordLet, first, reader.current_location())
    } else if bytes_between == [b'n', b'e', b'v', b'e', b'r'] {
        Token::new(TokenValue::KeywordNever, first, reader.current_location())
    } else if bytes_between == [b'n', b'u', b'l', b'l'] {
        Token::new(TokenValue::KeywordNull, first, reader.current_location())
    } else if bytes_between == [b'p', b'r', b'o', b'p'] {
        Token::new(TokenValue::KeywordProp, first, reader.current_location())
    } else if bytes_between == [b'r', b'e', b't', b'u', b'r', b'n'] {
        Token::new(TokenValue::KeywordReturn, first, reader.current_location())
    } else if bytes_between == [b'n', b'e', b'v', b'e', b'r'] {
        Token::new(TokenValue::KeywordNever, first, reader.current_location())
    } else if bytes_between == [b's', b'e', b'l', b'f'] {
        Token::new(TokenValue::KeywordSelf, first, reader.current_location())
    } else if bytes_between == [b't', b'y', b'p', b'e'] {
        Token::new(TokenValue::KeywordType, first, reader.current_location())
    } else if bytes_between == [b'u', b'1', b'6'] {
        Token::new(TokenValue::KeywordU16, first, reader.current_location())
    } else if bytes_between == [b'u', b'3', b'2'] {
        Token::new(TokenValue::KeywordU32, first, reader.current_location())
    } else if bytes_between == [b'u', b'6', b'4'] {
        Token::new(TokenValue::KeywordU64, first, reader.current_location())
    } else if bytes_between == [b'u', b'8'] {
        Token::new(TokenValue::KeywordU8, first, reader.current_location())
    } else if bytes_between == [b'u', b'i', b'n', b't'] {
        Token::new(TokenValue::KeywordUInt, first, reader.current_location())
    } else if bytes_between == [b'u', b's', b'i', b'z', b'e'] {
        Token::new(TokenValue::KeywordUSize, first, reader.current_location())
    } else if bytes_between == [b'v', b'o', b'i', b'd'] {
        Token::new(TokenValue::KeywordVoid, first, reader.current_location())
    } else if bytes_between == [b't', b'r', b'u', b'e'] {
        Token::new(TokenValue::Bool(true), first, reader.current_location())
    } else if bytes_between == [b'f', b'a', b'l', b's', b'e'] {
        Token::new(TokenValue::Bool(false), first, reader.current_location())
    } else {
        Token::new(TokenValue::Symbol, first, reader.current_location())
    }
}

fn lex_next_number<'source_context>(
    reader: &mut SourceReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Token<'source_context> {
    let first = reader.current_location().clone();
    let mut last: u8 = 0;
    let mut found_period = false;
    let mut found_minus = false;

    while let Some(byte) = reader.peek_next() {
        match byte {
            b'a'..=b'z' | b'A'..=b'Z' | b'_' | b'0'..=b'9' => {
                reader.read_next();
            }

            b'.' => {
                if found_period {
                    break;
                } else {
                    reader.read_next();
                    found_period = true;
                }
            }

            b'-' => {
                if found_minus {
                    break;
                } else if last == b'e' {
                    reader.read_next();
                    found_minus = true;
                } else {
                    break;
                }
            }

            _ => break,
        }

        last = byte;
    }

    if found_period {
        if let Some(token_value) = parse_float(&first, reader.current_location(), message_context) {
            Token::new(token_value, first, reader.current_location())
        } else {
            Token::new(
                TokenValue::Float {
                    value: 0.0,
                    width: 8,
                },
                first,
                reader.current_location(),
            )
        }
    } else {
        if let Some(token_value) = parse_int(&first, reader.current_location(), message_context) {
            Token::new(token_value, first, reader.current_location())
        } else {
            Token::new(
                TokenValue::UnsignedInt { value: 0, width: 4 },
                first,
                reader.current_location(),
            )
        }
    }
}

fn lex_next_character<'source_context>(
    reader: &mut SourceReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Token<'source_context> {
    todo!()
}

fn lex_next_string<'source_context>(
    reader: &mut SourceReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Token<'source_context> {
    todo!()
}

fn lex_next_token<'source_context>(
    reader: &mut SourceReader<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<Token<'source_context>> {
    let first = reader.current_location().clone();

    let byte = match reader.peek_next() {
        Some(byte) => byte,
        None => return None,
    };

    match byte {
        // Whitespace
        b' ' | b'\t' | b'\n' => {
            reader.read_next();
            None
        }

        // Symbols
        b'a'..=b'z' | b'A'..=b'Z' | b'_' | 0x80..=u8::MAX => Some(lex_next_symbol(reader)),

        // Numbers
        b'0'..=b'9' => Some(lex_next_number(reader, message_context)),

        // Characters
        b'\'' => Some(lex_next_character(reader, message_context)),

        // Strings
        b'"' => Some(lex_next_string(reader, message_context)),

        // Bookends
        b'(' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::LeftParenthesis,
                first,
                reader.current_location(),
            ))
        }
        b')' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::RightParenthesis,
                first,
                reader.current_location(),
            ))
        }
        b'{' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::LeftCurlyBrace,
                first,
                reader.current_location(),
            ))
        }
        b'}' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::RightCurlyBrace,
                first,
                reader.current_location(),
            ))
        }
        b'[' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::LeftSquareBracket,
                first,
                reader.current_location(),
            ))
        }
        b']' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::RightSquareBracket,
                first,
                reader.current_location(),
            ))
        }

        // Punctuation
        b'.' => {
            reader.read_next();

            if let Some(b'.') = reader.peek_next() {
                reader.read_next();

                if let Some(b'.') = reader.peek_next() {
                    reader.read_next();

                    Some(Token::new(
                        TokenValue::Ellipsis,
                        first,
                        reader.current_location(),
                    ))
                } else {
                    let mut message = Message::new_from_range(
                        Severity::Error,
                        first.clone(),
                        reader.current_location().clone(),
                        "'..' is not valid punctuation".to_owned(),
                    );

                    message.add_child(Message::new(
                        Severity::Suggestion,
                        "did you mean either '.' or '...'?".to_owned(),
                    ));

                    message_context.add(message);

                    None
                }
            } else {
                Some(Token::new(
                    TokenValue::Period,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b',' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::Comma,
                first,
                reader.current_location(),
            ))
        }
        b':' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::Colon,
                first,
                reader.current_location(),
            ))
        }
        b';' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::Semicolon,
                first,
                reader.current_location(),
            ))
        }

        // Operators
        b'~' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::BitNegate,
                first,
                reader.current_location(),
            ))
        }
        b'&' => {
            reader.read_next();

            if let Some(b'&') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::BoolAnd,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::BitAnd,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'|' => {
            reader.read_next();

            if let Some(b'|') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::BoolAnd,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::BitAnd,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'^' => {
            reader.read_next();
            Some(Token::new(
                TokenValue::BitXor,
                first,
                reader.current_location(),
            ))
        }
        b'<' => {
            reader.read_next();

            if let Some(b'<') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::BitShiftLeft,
                    first,
                    reader.current_location(),
                ))
            } else if let Some(b'=') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::LessThanOrEqualTo,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::LessThan,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'>' => {
            reader.read_next();

            if let Some(b'>') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::BitShiftRight,
                    first,
                    reader.current_location(),
                ))
            } else if let Some(b'=') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::GreaterThanOrEqualTo,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::GreaterThan,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'!' => {
            reader.read_next();

            if let Some(b'=') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::NotEquals,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::BoolNot,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'-' => {
            reader.read_next();

            if let Some(b'>') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::RightArrow,
                    first,
                    reader.current_location(),
                ))
            } else if let Some(b'-') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::Decrement,
                    first,
                    reader.current_location(),
                ))
            } else if let Some(b'=') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::SubtractAssign,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::Subtract,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'+' => {
            reader.read_next();

            if let Some(b'+') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::Increment,
                    first,
                    reader.current_location(),
                ))
            } else if let Some(b'=') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::AddAssign,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::Add,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'*' => {
            reader.read_next();

            if let Some(b'*') = reader.peek_next() {
                reader.read_next();

                if let Some(b'=') = reader.peek_next() {
                    reader.read_next();
                    Some(Token::new(
                        TokenValue::ExponentiateAssign,
                        first,
                        reader.current_location(),
                    ))
                } else {
                    Some(Token::new(
                        TokenValue::Exponentiate,
                        first,
                        reader.current_location(),
                    ))
                }
            } else if let Some(b'=') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::MultiplyAssign,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::Multiply,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'%' => {
            reader.read_next();

            if let Some(b'=') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::ModulateAssign,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::Modulate,
                    first,
                    reader.current_location(),
                ))
            }
        }
        b'=' => {
            reader.read_next();

            if let Some(b'=') = reader.peek_next() {
                reader.read_next();
                Some(Token::new(
                    TokenValue::Equals,
                    first,
                    reader.current_location(),
                ))
            } else {
                Some(Token::new(
                    TokenValue::Assign,
                    first,
                    reader.current_location(),
                ))
            }
        }

        // Error on unknown characters
        _ => {
            message_context.add(Message::new_from_location(
                Severity::Error,
                reader.current_location().clone(),
                "unexpected character".to_owned(),
            ));

            // Skip the character
            reader.read_next();

            None
        }
    }
}

pub fn lex<'source_context>(
    source: SourceRef<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Vec<Token<'source_context>> {
    let mut tokens = Vec::new();
    let mut reader = SourceReader::new(SourceLocation::new(source));

    while reader.is_more() {
        let before = reader.current_location().clone();

        if let Some(token) = lex_next_token(&mut reader, message_context) {
            tokens.push(token);
        }

        if reader.current_location().offset == before.offset {
            panic!("lexer did not advance cursor at {:?}", before);
        }
    }

    tokens
}

#[cfg(test)]
mod tests {
    use termcolor::{ColorChoice, StandardStream};

    use crate::{message::formating::write_message_context, source::domain::SourceContext};

    use super::*;

    #[test]
    fn empty() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 0);
    }

    #[test]
    fn whitespace_only() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", " \t\n\r\n  ").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 0);
    }

    #[test]
    fn symbol_underscore() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "_").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Symbol);
        assert_eq!(tokens[0].length, 1);
    }

    #[test]
    fn symbol_simple() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context
            .add_from_str("--", "helloWorld_1234")
            .unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Symbol);
        assert_eq!(tokens[0].length, 15);
    }

    #[test]
    fn symbol_unicode() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source_text = "♥";

        assert_eq!(source_text.as_bytes(), [0xe2, 0x99, 0xa5]);

        let source = source_context.add_from_str("--", source_text).unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Symbol);
        assert_eq!(tokens[0].length, 3);
    }

    #[test]
    fn keyword_usize() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "usize").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::KeywordUSize);
        assert_eq!(tokens[0].length, 5);
    }

    #[test]
    fn keyword_true() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "true").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Bool(true));
        assert_eq!(tokens[0].length, 4);
    }

    #[test]
    fn int() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "53").unwrap();
        let tokens = lex(source, &mut message_context);

        // write_message_context(
        //     &mut StandardStream::stdout(ColorChoice::Auto),
        //     &message_context,
        // )
        // .unwrap();

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(
            tokens[0].value,
            TokenValue::UnsignedInt {
                value: 53,
                width: 4
            }
        );
        assert_eq!(tokens[0].length, 2);
    }

    #[test]
    fn float() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "5.3").unwrap();
        let tokens = lex(source, &mut message_context);

        // write_message_context(
        //     &mut StandardStream::stdout(ColorChoice::Auto),
        //     &message_context,
        // )
        // .unwrap();

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(
            tokens[0].value,
            TokenValue::Float {
                value: 5.3,
                width: 8
            }
        );
        assert_eq!(tokens[0].length, 3);
    }

    #[test]
    fn comma() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", ",").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Comma);
        assert_eq!(tokens[0].length, 1);
    }

    #[test]
    fn period() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", ".").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Period);
        assert_eq!(tokens[0].length, 1);
    }

    #[test]
    fn ellipsis() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "...").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 1);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Ellipsis);
        assert_eq!(tokens[0].length, 3);
    }

    #[test]
    fn two_periods() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "..").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 1);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 0);
    }

    #[test]
    fn invalid_character() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "`").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 1);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 0);
    }

    #[test]
    fn multiple() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "a+5:asdf").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 5);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Symbol);
        assert_eq!(tokens[0].length, 1);
        assert_eq!(tokens[1].first.offset, 1);
        assert_eq!(tokens[1].value, TokenValue::Add);
        assert_eq!(tokens[1].length, 1);
        assert_eq!(tokens[2].first.offset, 2);
        assert_eq!(
            tokens[2].value,
            TokenValue::UnsignedInt { value: 5, width: 4 }
        );
        assert_eq!(tokens[2].length, 1);
        assert_eq!(tokens[3].first.offset, 3);
        assert_eq!(tokens[3].value, TokenValue::Colon);
        assert_eq!(tokens[3].length, 1);
        assert_eq!(tokens[4].first.offset, 4);
        assert_eq!(tokens[4].value, TokenValue::Symbol);
        assert_eq!(tokens[4].length, 4);
    }

    #[test]
    fn greedy() {
        let mut source_context = SourceContext::new();
        let mut message_context = MessageContext::new();

        let source = source_context.add_from_str("--", "+++").unwrap();
        let tokens = lex(source, &mut message_context);

        assert_eq!(message_context.error_count(), 0);
        assert_eq!(message_context.warning_count(), 0);
        assert_eq!(tokens.len(), 2);
        assert_eq!(tokens[0].first.offset, 0);
        assert_eq!(tokens[0].value, TokenValue::Increment);
        assert_eq!(tokens[0].length, 2);
        assert_eq!(tokens[1].first.offset, 2);
        assert_eq!(tokens[1].value, TokenValue::Add);
        assert_eq!(tokens[1].length, 1);
    }
}
