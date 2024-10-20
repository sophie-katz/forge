use super::domain::SourceLocation;

#[derive(Clone)]
pub struct SourceReaderState<'source_context> {
    current_location: SourceLocation<'source_context>,
}

pub struct SourceReader<'source_context> {
    state: SourceReaderState<'source_context>,
}

impl<'source_context> SourceReader<'source_context> {
    pub fn new(initial_location: SourceLocation<'source_context>) -> Self {
        SourceReader {
            state: SourceReaderState {
                current_location: initial_location,
            },
        }
    }

    pub fn is_more(&self) -> bool {
        self.state.current_location.offset < self.state.current_location.source.data().len()
    }

    pub fn peek_next(&self) -> Option<u8> {
        if self.is_more() {
            if self.state.current_location.source.data()[self.state.current_location.offset]
                == b'\r'
            {
                Some(b'\n')
            } else {
                Some(self.state.current_location.source.data()[self.state.current_location.offset])
            }
        } else {
            None
        }
    }

    pub fn read_next(&mut self) -> Option<u8> {
        if !self.is_more() {
            return None;
        }

        let byte = self.state.current_location.source.data()[self.state.current_location.offset];

        self.state.current_location.offset += 1;

        if byte == b'\n' {
            self.state.current_location.line_number += 1;
            self.state.current_location.column_number = 1;

            Some(b'\n')
        } else if byte == b'\r' {
            if self.peek_next() == Some(b'\n') {
                self.state.current_location.offset += 1;
            }

            self.state.current_location.line_number += 1;
            self.state.current_location.column_number = 1;

            Some(b'\n')
        } else {
            self.state.current_location.column_number += 1;

            Some(byte)
        }
    }

    pub fn current_location(&self) -> &SourceLocation<'source_context> {
        &self.state.current_location
    }

    pub fn save_state(&self) -> SourceReaderState<'source_context> {
        self.state.clone()
    }

    pub fn restore_state(&mut self, state: SourceReaderState<'source_context>) {
        self.state = state;
    }
}

#[cfg(test)]
mod tests {
    use super::super::domain::SourceContext;
    use super::*;

    #[test]
    fn test_empty_source() {
        let mut source_context = SourceContext::default();
        let source = source_context.add_from_str("--", "").unwrap();
        let mut reader = SourceReader::new(SourceLocation::new(source));

        assert_eq!(reader.is_more(), false);
        assert_eq!(reader.peek_next(), None);
        assert_eq!(reader.read_next(), None);
        assert_eq!(reader.current_location().offset, 0);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 1);
    }

    #[test]
    fn test_simple() {
        let mut source_context = SourceContext::default();
        let source = source_context.add_from_str("--", "ab").unwrap();
        let mut reader = SourceReader::new(SourceLocation::new(source));

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'a'));
        assert_eq!(reader.read_next(), Some(b'a'));
        assert_eq!(reader.current_location().offset, 1);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 2);

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'b'));
        assert_eq!(reader.read_next(), Some(b'b'));
        assert_eq!(reader.current_location().offset, 2);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 3);

        assert_eq!(reader.is_more(), false);
        assert_eq!(reader.peek_next(), None);
        assert_eq!(reader.read_next(), None);
        assert_eq!(reader.current_location().offset, 2);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 3);
    }

    #[test]
    fn test_unix_newline() {
        let mut source_context = SourceContext::default();
        let source = source_context.add_from_str("--", "a\nb\n").unwrap();
        let mut reader = SourceReader::new(SourceLocation::new(source));

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'a'));
        assert_eq!(reader.read_next(), Some(b'a'));
        assert_eq!(reader.current_location().offset, 1);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 2);

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'\n'));
        assert_eq!(reader.read_next(), Some(b'\n'));
        assert_eq!(reader.current_location().offset, 2);
        assert_eq!(reader.current_location().line_number, 2);
        assert_eq!(reader.current_location().column_number, 1);

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'b'));
        assert_eq!(reader.read_next(), Some(b'b'));

        assert_eq!(reader.current_location().offset, 3);
        assert_eq!(reader.current_location().line_number, 2);
        assert_eq!(reader.current_location().column_number, 2);

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'\n'));
        assert_eq!(reader.read_next(), Some(b'\n'));
        assert_eq!(reader.current_location().offset, 4);
        assert_eq!(reader.current_location().line_number, 3);
        assert_eq!(reader.current_location().column_number, 1);

        assert_eq!(reader.is_more(), false);
        assert_eq!(reader.peek_next(), None);
        assert_eq!(reader.read_next(), None);
        assert_eq!(reader.current_location().offset, 4);
        assert_eq!(reader.current_location().line_number, 3);
        assert_eq!(reader.current_location().column_number, 1);
    }

    #[test]
    fn test_dos_newline() {
        let mut source_context = SourceContext::default();
        let source = source_context.add_from_str("--", "a\r\nb\r\n").unwrap();
        let mut reader = SourceReader::new(SourceLocation::new(source));

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'a'));
        assert_eq!(reader.read_next(), Some(b'a'));
        assert_eq!(reader.current_location().offset, 1);
        assert_eq!(reader.current_location().line_number, 1);
        assert_eq!(reader.current_location().column_number, 2);

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'\n'));
        assert_eq!(reader.read_next(), Some(b'\n'));
        assert_eq!(reader.current_location().offset, 3);
        assert_eq!(reader.current_location().line_number, 2);
        assert_eq!(reader.current_location().column_number, 1);

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'b'));
        assert_eq!(reader.read_next(), Some(b'b'));
        assert_eq!(reader.current_location().offset, 4);
        assert_eq!(reader.current_location().line_number, 2);
        assert_eq!(reader.current_location().column_number, 2);

        assert_eq!(reader.is_more(), true);
        assert_eq!(reader.peek_next(), Some(b'\n'));
        assert_eq!(reader.read_next(), Some(b'\n'));
        assert_eq!(reader.current_location().offset, 6);
        assert_eq!(reader.current_location().line_number, 3);
        assert_eq!(reader.current_location().column_number, 1);

        assert_eq!(reader.is_more(), false);
        assert_eq!(reader.peek_next(), None);
        assert_eq!(reader.read_next(), None);
        assert_eq!(reader.current_location().offset, 6);
        assert_eq!(reader.current_location().line_number, 3);
        assert_eq!(reader.current_location().column_number, 1);
    }
}
