use crate::sourcing::{ErrorSource, SourceLocation, SourceReader};

use super::Token;

/// A context that can be used for lexing.
///
/// It provides easy access to:
/// * Read grapheme clusters
/// * Construct tokens
/// * Skip grapheme clusters
/// * Etc.
///
/// # Example
///
/// ```
/// # use forge_parsing::{
/// #     lexing::{LexingContext, Token},
/// #     sourcing::{SourceContext, SourceLocation, SourceReader},
/// # };
/// #
/// # type TokenValue = String;
/// #
/// # let mut source_context = SourceContext::default();
/// #
/// # let source = source_context
/// #     .add_from_string("--", "abcdef".to_owned())
/// #     .unwrap();
/// #
/// // Create reader
/// let mut reader = SourceReader::new(SourceLocation::new_at_start(source));
///
/// // Create a vector to store constructed tokens
/// let mut tokens = Vec::new();
///
/// // Create the lexing context itself
/// let mut lexing_context = LexingContext::<TokenValue>::new(&mut reader, &mut tokens);
/// ```
pub struct LexingContext<'source_context, 'source_reader, 'tokens, TokenValue> {
    reader: &'source_reader mut SourceReader<'source_context>,
    tokens: &'tokens mut Vec<Token<'source_context, TokenValue>>,
    token_first: SourceLocation<'source_context>,
    token_last: SourceLocation<'source_context>,
    last_grapheme_cluster: Option<String>,
}

impl<'source_context, 'source_reader, 'tokens, TokenValue>
    LexingContext<'source_context, 'source_reader, 'tokens, TokenValue>
{
    /// # Arguments
    ///
    /// * `reader` - The source reader from which to read grapheme clusters
    /// * `tokens` - The vector to store constructed tokens to
    pub fn new(
        reader: &'source_reader mut SourceReader<'source_context>,
        tokens: &'tokens mut Vec<Token<'source_context, TokenValue>>,
    ) -> Self {
        let current_location = reader.current_location();

        Self {
            reader,
            tokens,
            token_first: current_location.clone(),
            token_last: current_location,
            last_grapheme_cluster: None,
        }
    }

    /// Check if there are more grapheme clusters to read
    ///
    /// This will still return `true` if there is an incomplete grapheme cluster at the end of the
    /// source.
    pub fn is_more(&self) -> bool {
        self.reader.is_more()
    }

    /// Peek at the next grapheme cluster without advancing the lexers current position in the
    /// source.
    ///
    /// # Returns
    ///
    /// The string content of the grapheme cluster.
    ///
    /// # Errors
    ///
    /// See [`SourceReader::peek_next_grapheme_cluster`].
    pub fn peek_next_grapheme_cluster(&self) -> Result<String, ErrorSource> {
        self.reader.peek_next_grapheme_cluster()
    }

    /// Reads the next grapheme cluster and advances the lexers current position in the source.
    ///
    /// # Returns
    ///
    /// The string content of the grapheme cluster.
    ///
    /// # Errors
    ///
    /// See [`SourceReader::peek_next_grapheme_cluster`].
    pub fn read_next_grapheme_cluster(&mut self) -> Result<String, ErrorSource> {
        self.token_last = self.reader.current_location();

        match self.reader.read_next_grapheme_cluster() {
            Ok(grapheme_cluster) => {
                self.last_grapheme_cluster = Some(grapheme_cluster.clone());
                Ok(grapheme_cluster)
            }
            Err(error) => Err(error),
        }
    }

    /// Returns `true` if any grapheme clusters have been read since beginning to lex or since and
    /// the last token was popped.
    ///
    /// If this is `false`, popping a token with [`LexingContext::pop_token`] will do nothing.
    pub fn has_token(&self) -> bool {
        self.token_first.byte_offset < self.reader.current_location().byte_offset
    }

    /// Get the current location in the source.
    pub fn current_location(&self) -> SourceLocation<'source_context> {
        self.reader.current_location()
    }

    /// Pop a token with the given value, if possible.
    ///
    /// # Returns
    ///
    /// Whether or not a token was popped. This is equivalent to [`LexingContext::has_token`].
    pub fn pop_token(&mut self, value: TokenValue) -> bool {
        if self.has_token() {
            let token = Token::new(value, self.token_first.clone(), self.token_last.clone());
            self.token_first = self.reader.current_location();
            self.token_last = self.reader.current_location();
            self.tokens.push(token);
            true
        } else {
            false
        }
    }

    /// Skips the current token.
    ///
    /// This is equivalent to popping the token without pushing it to the storage vector.
    ///
    /// # Returns
    ///
    /// Whether or not a token was popped. This is equivalent to [`LexingContext::has_token`].
    pub fn skip_token(&mut self) -> bool {
        if self.has_token() {
            self.token_first = self.reader.current_location();
            self.token_last = self.reader.current_location();
            true
        } else {
            false
        }
    }

    /// Get the last grapheme cluster that was read.
    ///
    /// This is helpful for context for the current grapheme cluster.
    pub fn last_grapheme_cluster(&self) -> Option<&str> {
        self.last_grapheme_cluster.as_deref()
    }
}

#[cfg(test)]
mod tests {
    use crate::sourcing::SourceContext;

    use super::*;

    #[test]
    fn simple() {
        let mut source_context = SourceContext::default();

        let source = source_context
            .add_from_string("--", "abcdef".to_owned())
            .unwrap();

        let mut reader = SourceReader::new(SourceLocation::new_at_start(source));

        let mut tokens = Vec::new();

        let mut lexing_context = LexingContext::<String>::new(&mut reader, &mut tokens);

        // Test initial state
        assert!(lexing_context.is_more());
        assert_eq!(lexing_context.peek_next_grapheme_cluster().unwrap(), "a");
        assert!(!lexing_context.has_token());

        // Read one character as a token
        assert_eq!(lexing_context.read_next_grapheme_cluster().unwrap(), "a");
        assert!(lexing_context.is_more());
        assert_eq!(lexing_context.peek_next_grapheme_cluster().unwrap(), "b");
        assert!(lexing_context.has_token());
        assert!(lexing_context.pop_token("a".to_owned()));
        assert!(!lexing_context.has_token());

        // Read two characters as a token
        assert_eq!(lexing_context.read_next_grapheme_cluster().unwrap(), "b");
        assert!(lexing_context.is_more());
        assert_eq!(lexing_context.read_next_grapheme_cluster().unwrap(), "c");
        assert!(lexing_context.is_more());
        assert_eq!(lexing_context.peek_next_grapheme_cluster().unwrap(), "d");
        assert!(lexing_context.has_token());
        assert!(lexing_context.pop_token("bc".to_owned()));
        assert!(!lexing_context.has_token());

        // Read the rest as a token
        assert_eq!(lexing_context.read_next_grapheme_cluster().unwrap(), "d");
        assert!(lexing_context.is_more());
        assert_eq!(lexing_context.read_next_grapheme_cluster().unwrap(), "e");
        assert!(lexing_context.is_more());
        assert_eq!(lexing_context.read_next_grapheme_cluster().unwrap(), "f");
        assert!(!lexing_context.is_more());
        assert!(lexing_context.peek_next_grapheme_cluster().is_err());
        assert!(lexing_context.has_token());
        assert!(lexing_context.pop_token("def".to_owned()));
        assert!(!lexing_context.has_token());

        // Check token values
        assert_eq!(tokens.len(), 3);
        assert_eq!(tokens[0].value, "a");
        assert_eq!(tokens[0].first.byte_offset, 0);
        assert_eq!(tokens[0].last.byte_offset, 0);
        assert_eq!(tokens[0].text(), "a");
        assert_eq!(tokens[1].value, "bc");
        assert_eq!(tokens[1].first.byte_offset, 1);
        assert_eq!(tokens[1].last.byte_offset, 2);
        assert_eq!(tokens[1].text(), "bc");
        assert_eq!(tokens[2].value, "def");
        assert_eq!(tokens[2].first.byte_offset, 3);
        assert_eq!(tokens[2].last.byte_offset, 5);
        assert_eq!(tokens[2].text(), "def");
    }
}
