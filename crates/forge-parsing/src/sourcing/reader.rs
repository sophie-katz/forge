use std::{cell::RefCell, str};

use unicode_segmentation::{GraphemeCursor, GraphemeIncomplete};

use super::{domain::SourceLocation, error::ErrorSource};

/// A saved state of a [`SourceReader`] which can be used to backtrack to previous locations safely.
#[derive(Clone)]
pub struct SourceReaderState<'source_context> {
    current_location: SourceLocation<'source_context>,
}

/// A reader that reads source code as a stream of grapheme clusters.
///
/// It also automatically converts DOS newlines to UNIX newlines opaquely to the caller.
///
/// # Example
///
/// ```
/// use forge_parsing::sourcing::{SourceContext, SourceLocation, SourceReader};
///
/// let mut source_context = SourceContext::default();
///
/// let source = source_context.add_from_string("--", "a\nb\n".to_owned()).unwrap();
///
/// let mut reader = SourceReader::new(SourceLocation::new_at_start(source));
///
/// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "a");
/// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "\n");
/// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "b");
/// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "\n");
/// assert!(reader.read_next_grapheme_cluster().is_err());
/// ```
pub struct SourceReader<'source_context> {
    state: RefCell<SourceReaderState<'source_context>>,
}

impl<'source_context> SourceReader<'source_context> {
    /// Creates a new [`SourceReader`] with the given initial location.
    pub fn new(initial_location: SourceLocation<'source_context>) -> Self {
        SourceReader {
            state: RefCell::new(SourceReaderState {
                current_location: initial_location,
            }),
        }
    }

    /// Checks if there are any more bytes to be read.
    ///
    /// **Note:** these bytes do not necessarily need to be complete grapheme clusters.
    pub fn is_more(&self) -> bool {
        let current_location = &self.state.borrow().current_location;

        current_location.byte_offset < current_location.source.data().len()
    }

    /// Peeks the next grapheme cluster without advancing the reader.
    ///
    /// # Example
    ///
    /// ```
    /// use forge_parsing::sourcing::{SourceContext, SourceLocation, SourceReader};
    ///
    /// let mut source_context = SourceContext::default();
    ///
    /// let source = source_context.add_from_string("--", "a\nb\n".to_owned()).unwrap();
    ///
    /// let mut reader = SourceReader::new(SourceLocation::new_at_start(source));
    ///
    /// assert_eq!(reader.peek_next_grapheme_cluster().unwrap(), "a");
    /// assert_eq!(reader.peek_next_grapheme_cluster().unwrap(), "a");
    /// ```
    ///
    /// # Returns
    ///
    /// A grapheme cluster if one can be read.
    ///
    /// # Errors
    ///
    /// * [`ErrorSource::NoMoreToBeRead`] if there are no more bytes to be read.
    /// * [`ErrorSource::ByteOffsetInMiddleOfGraphemeCluster`] if the byte offset is in the middle
    ///   of a grapheme cluster.
    /// * [`ErrorSource::UnicodeSegmentation`] if there is an unhandled error while finding the
    ///   boundaries of the next grapheme cluster.
    /// * [`ErrorSource::Utf8`] if the grapheme cluster is not valid UTF-8.
    pub fn peek_next_grapheme_cluster(&self) -> Result<String, ErrorSource> {
        match self.peek_next_grapheme_cluster_without_converting_newlines() {
            Ok(grapheme_cluster) => {
                if grapheme_cluster == "\r" || grapheme_cluster == "\r\n" {
                    Ok("\n".to_owned())
                } else {
                    Ok(grapheme_cluster)
                }
            }
            Err(error) => Err(error),
        }
    }

    fn peek_next_grapheme_cluster_without_converting_newlines(
        &self,
    ) -> Result<String, ErrorSource> {
        if !self.is_more() {
            return Err(ErrorSource::NoMoreToBeRead);
        }

        let current_location = &self.state.borrow().current_location;

        let mut cursor = GraphemeCursor::new(
            current_location.byte_offset,
            current_location.source.data().len(),
            true,
        );

        // It's OK for us to use the unsafe `from_utf8_unchecked` here because we know that the
        // unicode segmentation library will enforce that the bytes are valid UTF-8 anyway.
        //
        // This has the advantage that `GraphemeCursor.next_boundary` only checks validity for the
        // bytes of the current grapheme cluster, and not for the entire source string like
        // `str::from_utf8` would.
        let next_boundary = unsafe {
            match cursor.next_boundary(str::from_utf8_unchecked(current_location.source.data()), 0)
            {
                Ok(Some(next_boundary)) => next_boundary,
                // This should have already been handled above by checking `self.is_more()`, but there's
                // no harm in also handling it here
                Ok(None) => return Err(ErrorSource::NoMoreToBeRead),
                Err(GraphemeIncomplete::NextChunk) | Err(GraphemeIncomplete::PreContext(_)) => {
                    return Err(ErrorSource::ByteOffsetInMiddleOfGraphemeCluster(
                        current_location.byte_offset,
                    ))
                }
                Err(error) => return Err(ErrorSource::UnicodeSegmentation(error)),
            }
        };

        assert!(current_location.byte_offset < next_boundary);

        Ok(str::from_utf8(
            &current_location.source.data()[current_location.byte_offset..next_boundary],
        )?
        .to_owned())
    }

    /// Reads the next grapheme cluster and advances the reader.
    ///
    /// See [`SourceReader::peek_next`] for more details - this function calls that one.
    ///
    /// # Example
    ///
    /// ```
    /// use forge_parsing::sourcing::{SourceContext, SourceLocation, SourceReader};
    ///
    /// let mut source_context = SourceContext::default();
    ///
    /// let source = source_context.add_from_string("--", "a\nb\n".to_owned()).unwrap();
    ///
    /// let mut reader = SourceReader::new(SourceLocation::new_at_start(source));
    ///
    /// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "a");
    /// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "\n");
    /// ```
    ///
    /// # Returns
    ///
    /// A grapheme cluster if one can be read.
    ///
    /// # Errors
    ///
    /// * [`ErrorSource::NoMoreToBeRead`] if there are no more bytes to be read.
    /// * [`ErrorSource::ByteOffsetInMiddleOfGraphemeCluster`] if the byte offset is in the middle
    ///   of a grapheme cluster.
    /// * [`ErrorSource::UnicodeSegmentation`] if there is an unhandled error while finding the
    ///   boundaries of the next grapheme cluster.
    /// * [`ErrorSource::Utf8`] if the grapheme cluster is not valid UTF-8.
    pub fn read_next_grapheme_cluster(&mut self) -> Result<String, ErrorSource> {
        let grapheme_cluster = self.peek_next_grapheme_cluster_without_converting_newlines()?;

        let current_location = &mut self.state.borrow_mut().current_location;

        current_location.byte_offset += grapheme_cluster.len();

        if grapheme_cluster == "\n" || grapheme_cluster == "\r\n" {
            current_location.line_number += 1;
            current_location.column_number = 1;

            Ok("\n".to_owned())
        } else if grapheme_cluster == "\r" {
            if self
                .peek_next_grapheme_cluster()
                .map_or(false, |value| value == "\n")
            {
                current_location.byte_offset += 1;
            }

            current_location.line_number += 1;
            current_location.column_number = 1;

            Ok("\n".to_owned())
        } else {
            current_location.column_number += 1;

            Ok(grapheme_cluster)
        }
    }

    /// Gets the current location of the reader in the source.
    ///
    /// The values in the location are guaranteed to be valid within the source.
    pub fn current_location(&self) -> SourceLocation<'source_context> {
        self.state.borrow().current_location.clone()
    }

    /// Saves the current state of the reader to be restored later on.
    ///
    /// # Example
    ///
    /// ```
    /// use forge_parsing::sourcing::{SourceContext, SourceLocation, SourceReader};
    ///
    /// let mut source_context = SourceContext::default();
    ///
    /// let source = source_context.add_from_string("--", "a\nb\n".to_owned()).unwrap();
    ///
    /// let mut reader = SourceReader::new(SourceLocation::new_at_start(source));
    ///
    /// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "a");
    ///
    /// let state = reader.save_state();
    ///
    /// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "\n");
    /// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "b");
    ///
    /// reader.restore_state(state);
    ///
    /// assert_eq!(reader.read_next_grapheme_cluster().unwrap(), "\n");
    /// ```
    pub fn save_state(&self) -> SourceReaderState<'source_context> {
        self.state.borrow().clone()
    }

    /// Restores the state of the reader to a previously saved state.
    ///
    /// See [`SourceReader::save_state`] for an example of usage.
    pub fn restore_state(&mut self, state: SourceReaderState<'source_context>) {
        *self.state.borrow_mut() = state;
    }
}

#[cfg(test)]
mod tests {
    use super::super::domain::SourceContext;
    use super::*;

    #[test]
    fn test_empty_source() {
        let mut source_context = SourceContext::default();
        let source = source_context.add_from_string("--", "".to_owned()).unwrap();
        let mut reader = SourceReader::new(SourceLocation::new_at_start(source));

        assert!(!reader.is_more());
        assert!(reader.peek_next_grapheme_cluster().is_err());
        assert!(reader.read_next_grapheme_cluster().is_err());
        assert_eq!(reader.current_location().byte_offset, 0);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 1);
    }

    #[test]
    fn test_simple() {
        let mut source_context = SourceContext::default();
        let source = source_context
            .add_from_string("--", "ab".to_owned())
            .unwrap();
        let mut reader = SourceReader::new(SourceLocation::new_at_start(source));

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("a".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("a".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 1);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 2);

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("b".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("b".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 2);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 3);

        assert!(!reader.is_more());
        assert!(reader.peek_next_grapheme_cluster().is_err());
        assert!(reader.read_next_grapheme_cluster().is_err());
        assert_eq!(reader.current_location().byte_offset, 2);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 3);
    }

    #[test]
    fn test_unix_newline() {
        let mut source_context = SourceContext::default();
        let source = source_context
            .add_from_string("--", "a\nb\n".to_owned())
            .unwrap();
        let mut reader = SourceReader::new(SourceLocation::new_at_start(source));

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("a".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("a".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 1);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 2);

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("\n".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("\n".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 2);
        assert_eq!(reader.current_location().line_number, 2);
        assert_eq!(reader.current_location().column_number, 1);

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("b".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("b".to_owned())
        );

        assert_eq!(reader.current_location().byte_offset, 3);
        assert_eq!(reader.current_location().line_number, 2);
        assert_eq!(reader.current_location().column_number, 2);

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("\n".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("\n".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 4);
        assert_eq!(reader.current_location().line_number, 3);
        assert_eq!(reader.current_location().column_number, 1);

        assert!(!reader.is_more());
        assert!(reader.peek_next_grapheme_cluster().is_err());
        assert!(reader.read_next_grapheme_cluster().is_err());
        assert_eq!(reader.current_location().byte_offset, 4);
        assert_eq!(reader.current_location().line_number, 3);
        assert_eq!(reader.current_location().column_number, 1);
    }

    #[test]
    fn test_dos_newline() {
        let mut source_context = SourceContext::default();
        let source = source_context
            .add_from_string("--", "a\r\nb\r\n".to_owned())
            .unwrap();
        let mut reader = SourceReader::new(SourceLocation::new_at_start(source));

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("a".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("a".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 1);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 2);

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("\n".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("\n".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 3);
        assert_eq!(reader.current_location().line_number, 2);
        assert_eq!(reader.current_location().column_number, 1);

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("b".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("b".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 4);
        assert_eq!(reader.current_location().line_number, 2);
        assert_eq!(reader.current_location().column_number, 2);

        assert!(reader.is_more());
        assert_eq!(
            reader.peek_next_grapheme_cluster().ok(),
            Some("\n".to_owned())
        );
        assert_eq!(
            reader.read_next_grapheme_cluster().ok(),
            Some("\n".to_owned())
        );
        assert_eq!(reader.current_location().byte_offset, 6);
        assert_eq!(reader.current_location().line_number, 3);
        assert_eq!(reader.current_location().column_number, 1);

        assert!(!reader.is_more());
        assert!(reader.peek_next_grapheme_cluster().is_err());
        assert!(reader.read_next_grapheme_cluster().is_err());
        assert_eq!(reader.current_location().byte_offset, 6);
        assert_eq!(reader.current_location().line_number, 3);
        assert_eq!(reader.current_location().column_number, 1);
    }
}
