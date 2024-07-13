use std::{cell::RefCell, fmt::Debug};

use super::error::SourceError;

pub type SourceId = usize;
pub type SourceOffset = usize;
pub type LineNumber = u32;
pub type ColumnNumber = u32;

struct SourceData {
    pub path: String,
    pub data: Vec<u8>,
    line_offset_index: RefCell<Option<Vec<SourceOffset>>>,
}

impl SourceData {
    fn build_line_offset_index(&self) {
        let mut line_offset_index = Vec::new();

        let mut offset = 0;

        while offset < self.data.len() {
            if self.data[offset] == b'\n' {
                if offset + 1 < self.data.len() {
                    line_offset_index.push(offset + 1);
                }
            } else if self.data[offset] == b'\r' {
                if offset + 1 < self.data.len() && self.data[offset + 1] == b'\n' {
                    if offset + 2 < self.data.len() {
                        line_offset_index.push(offset + 2);
                    }

                    offset += 1;
                } else {
                    if offset + 1 < self.data.len() {
                        line_offset_index.push(offset + 1);
                    }
                }
            }

            offset += 1;
        }

        *self.line_offset_index.borrow_mut() = Some(line_offset_index);
    }

    fn get_line_offset(
        &self,
        line_number: LineNumber,
    ) -> Result<Option<SourceOffset>, SourceError> {
        if !self.line_offset_index.borrow().is_some() {
            self.build_line_offset_index();
        }

        if line_number < 1 {
            return Err(SourceError::LineNumberCannotBeZero);
        }

        if line_number == 1 {
            Ok(Some(0))
        } else if line_number - 2
            < self.line_offset_index.borrow().as_ref().unwrap().len() as LineNumber
        {
            Ok(Some(
                self.line_offset_index.borrow().as_ref().unwrap()[(line_number - 2) as usize],
            ))
        } else {
            Ok(None)
        }
    }
}

#[derive(Clone, Copy)]
pub struct SourceRef<'source_context> {
    context: &'source_context SourceContext,
    id: SourceId,
}

impl<'source_context> SourceRef<'source_context> {
    pub fn path(&self) -> &str {
        &self.context.sources[self.id].path
    }

    pub fn data(&self) -> &[u8] {
        &self.context.sources[self.id].data
    }

    pub fn get_first_location_of_line(
        &self,
        line_number: LineNumber,
    ) -> Result<Option<SourceLocation>, SourceError> {
        if let Some(offset) = self.context.sources[self.id].get_line_offset(line_number)? {
            Ok(Some(SourceLocation {
                source: *self,
                offset,
                line_number,
                column_number: 1,
            }))
        } else {
            Ok(None)
        }
    }
}

impl<'source_context> Debug for SourceRef<'source_context> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // TODO: See if this is best practice or not
        f.debug_struct("SourceRef")
            .field("id", &self.id)
            .field("path()", &self.path())
            .finish()
    }
}

impl<'source_context> PartialEq for SourceRef<'source_context> {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

impl<'source_context> Eq for SourceRef<'source_context> {}

pub struct SourceContext {
    sources: Vec<SourceData>,
    source_id_counter: SourceId,
}

impl SourceContext {
    pub fn new() -> Self {
        Self {
            sources: Vec::new(),
            source_id_counter: 0,
        }
    }

    pub fn add_raw(&mut self, path: String, data: Vec<u8>) -> Result<SourceRef, SourceError> {
        if path.is_empty() {
            return Err(SourceError::EmptyPath);
        }

        let id = self.next_id();

        self.sources.push(SourceData {
            path,
            data,
            line_offset_index: RefCell::new(None),
        });

        Ok(SourceRef { context: self, id })
    }

    pub fn add_from_str(&mut self, path: String, data: &str) -> Result<SourceRef, SourceError> {
        self.add_raw(path, data.as_bytes().to_vec())
    }

    pub fn add_from_file(&mut self, _path: String) -> Result<SourceRef, SourceError> {
        todo!()
    }

    fn next_id(&mut self) -> SourceId {
        let id = self.source_id_counter;
        self.source_id_counter += 1;
        id
    }
}

/// A referenced location with in a source.
///
/// **Warning:** locations from different sources are compared by their offsets only.
#[derive(Debug, Clone, Copy)]
pub struct SourceLocation<'source_context> {
    pub source: SourceRef<'source_context>,
    pub offset: SourceOffset,
    pub line_number: LineNumber,
    pub column_number: ColumnNumber,
}

impl<'source_context> SourceLocation<'source_context> {
    pub fn new(source: SourceRef<'source_context>) -> Self {
        SourceLocation {
            source,
            offset: 0,
            line_number: 1,
            column_number: 1,
        }
    }
}

impl<'source> PartialEq for SourceLocation<'source> {
    fn eq(&self, other: &Self) -> bool {
        self.offset == other.offset
    }
}

impl<'source> Eq for SourceLocation<'source> {}

impl<'source> PartialOrd for SourceLocation<'source> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        self.offset.partial_cmp(&other.offset)
    }
}

impl<'source> Ord for SourceLocation<'source> {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.offset.cmp(&other.offset)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_source_data_build_line_offset_index_unix() {
        let source = SourceData {
            path: "--".to_owned(),
            data: vec![b'a', b'\n', b'b', b'c', b'\n', b'd', b'\n'],
            line_offset_index: RefCell::new(None),
        };

        assert!(source.get_line_offset(0).is_err());
        assert_eq!(source.get_line_offset(1).unwrap(), Some(0));
        assert_eq!(source.get_line_offset(2).unwrap(), Some(2));
        assert_eq!(source.get_line_offset(3).unwrap(), Some(5));
        assert_eq!(source.get_line_offset(4).unwrap(), None);
    }

    #[test]
    fn test_source_data_build_line_offset_index_dos() {
        let source = SourceData {
            path: "--".to_owned(),
            data: vec![
                b'a', b'\r', b'\n', b'b', b'c', b'\r', b'\n', b'd', b'\r', b'\n',
            ],
            line_offset_index: RefCell::new(None),
        };

        assert!(source.get_line_offset(0).is_err());
        assert_eq!(source.get_line_offset(1).unwrap(), Some(0));
        assert_eq!(source.get_line_offset(2).unwrap(), Some(3));
        assert_eq!(source.get_line_offset(3).unwrap(), Some(7));
        assert_eq!(source.get_line_offset(4).unwrap(), None);
    }
}
