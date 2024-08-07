use crate::{
    message::domain::{Message, MessageContext, Severity},
    source::domain::SourceLocation,
};

use super::token::TokenValue;

pub fn parse_int<'source_context>(
    first: &SourceLocation<'source_context>,
    last: &SourceLocation<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<TokenValue> {
    match first.string_between(last).parse::<u64>() {
        Ok(value) => Some(TokenValue::UnsignedInt { value, width: 4 }),
        Err(err) => {
            message_context.add(Message::new_from_range(
                Severity::Error,
                first.clone(),
                last.clone(),
                format!("unable to parse integer literal: {}", err),
            ));
            None
        }
    }
}

pub fn parse_float<'source_context>(
    first: &SourceLocation<'source_context>,
    last: &SourceLocation<'source_context>,
    message_context: &mut MessageContext<'source_context>,
) -> Option<TokenValue> {
    match first.string_between(last).parse::<f64>() {
        Ok(value) => Some(TokenValue::Float { value, width: 8 }),
        Err(err) => {
            message_context.add(Message::new_from_range(
                Severity::Error,
                first.clone(),
                last.clone(),
                format!("unable to parse float literal: {}", err),
            ));
            None
        }
    }
}
