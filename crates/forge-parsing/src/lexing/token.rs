use std::fmt;

use crate::sourcing::SourceLocation;

#[cfg(test)]
use crate::sourcing::SourceContext;

/// A token in the source code.
///
/// It contains a token value, which is used to determine the meaning of the token, and information
/// about from where in the source it comes. For example:
///
/// * `value` - `Symbol`
/// * `first` - `test.frg:1:1`
/// * `last` - `test.frg:1:5`
#[derive(Debug, Clone, PartialEq)]
pub struct Token<'source_context, TokenValue> {
    /// The value of the token which determines its meaning.
    pub value: TokenValue,

    /// The location of the first grapheme cluster of the token.
    pub first: SourceLocation<'source_context>,

    /// The location of the last grapheme cluster of the token.
    pub last: SourceLocation<'source_context>,
}

impl<'source_context, TokenValue> Token<'source_context, TokenValue> {
    /// # Arguments
    ///
    /// * `value` - The value of the token which determines its meaning
    /// * `first` - The location of the first grapheme cluster of the token
    /// * `last` - The location of the last grapheme cluster of the token
    pub fn new(
        value: TokenValue,
        first: SourceLocation<'source_context>,
        last: SourceLocation<'source_context>,
    ) -> Self {
        assert!(first.byte_offset <= last.byte_offset);
        Self { value, first, last }
    }

    #[cfg(test)]
    pub fn new_test(value: TokenValue, source_context: &'source_context SourceContext) -> Self {
        Self {
            value,
            first: SourceLocation::new_test(source_context),
            last: SourceLocation::new_test(source_context),
        }
    }

    /// Gets the text value of the token by concatenating the grapheme clusters within it.
    pub fn text(&self) -> String {
        self.first.string_between(&self.last)
    }
}

impl<'source_context, TokenValue: fmt::Display> fmt::Display
    for Token<'source_context, TokenValue>
{
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{: <12} {}",
            format!("[{}]", self.first).to_string(),
            self.value
        )
    }
}
