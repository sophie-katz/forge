//! Functions for creating token matching conditions.

use crate::lexing::token::{Token, TokenValue};

pub fn match_any_token<'source_context>() -> impl Fn(&Token<'source_context>) -> bool {
    |_| true
}

pub fn match_token_with_condition<'source_context>(
    condition: impl Fn(&TokenValue) -> bool,
) -> impl Fn(&Token<'source_context>) -> bool {
    move |token: &Token<'source_context>| condition(&token.value)
}

pub fn match_token_with_value<'source_context, 'token_value>(
    value: &'token_value TokenValue,
) -> impl 'token_value + Fn(&Token<'source_context>) -> bool {
    move |token: &Token<'source_context>| token.value == *value
}

#[cfg(test)]
mod tests {
    use crate::source::domain::SourceContext;

    use super::*;

    #[test]
    fn test_match_any_token() {
        let source_context = SourceContext::default();

        let token = Token::new_test(TokenValue::Symbol, &source_context);
        assert!(match_any_token()(&token));

        let token = Token::new_test(TokenValue::BitOr, &source_context);
        assert!(match_any_token()(&token));
    }

    #[test]
    fn test_match_token_with_condition() {
        let source_context = SourceContext::default();

        let token = Token::new_test(TokenValue::Symbol, &source_context);
        assert!(match_token_with_condition(|token_value| {
            assert_eq!(*token_value, TokenValue::Symbol);
            true
        })(&token));

        let token = Token::new_test(TokenValue::BitOr, &source_context);
        assert!(!match_token_with_condition(|token_value| {
            assert_eq!(*token_value, TokenValue::BitOr);
            false
        })(&token));
    }

    #[test]
    fn test_match_token_with_value() {
        let source_context = SourceContext::default();

        let token = Token::new_test(TokenValue::Symbol, &source_context);
        assert!(match_token_with_value(&TokenValue::Symbol)(&token));

        let token = Token::new_test(TokenValue::BitOr, &source_context);
        assert!(match_token_with_value(&TokenValue::BitOr)(&token));
        assert!(!match_token_with_value(&TokenValue::Symbol)(&token));
    }
}
