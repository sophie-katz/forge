use std::{cell::RefCell, fmt, fs};

use crate::types::{ColumnNumber, LineNumber, SourceByteOffset, SourceId};

use super::error::ErrorSource;

/// The data loaded from a source file.
///
/// See [`SourceContext`] for more details.
struct SourceData {
    pub path: String,
    pub data: Vec<u8>,
    line_byte_offset_index: RefCell<Option<Vec<SourceByteOffset>>>,
}

impl SourceData {
    fn build_line_byte_offset_index(&self) {
        let mut line_byte_offset_index = Vec::new();

        let mut byte_offset = 0;

        while byte_offset < self.data.len() {
            if self.data[byte_offset] == b'\n' {
                if byte_offset + 1 < self.data.len() {
                    line_byte_offset_index.push(byte_offset + 1);
                }
            } else if self.data[byte_offset] == b'\r' {
                if byte_offset + 1 < self.data.len() && self.data[byte_offset + 1] == b'\n' {
                    if byte_offset + 2 < self.data.len() {
                        line_byte_offset_index.push(byte_offset + 2);
                    }

                    byte_offset += 1;
                } else if byte_offset + 1 < self.data.len() {
                    line_byte_offset_index.push(byte_offset + 1);
                }
            }

            byte_offset += 1;
        }

        *self.line_byte_offset_index.borrow_mut() = Some(line_byte_offset_index);
    }

    fn get_line_byte_offset(
        &self,
        line_number: LineNumber,
    ) -> Result<Option<SourceByteOffset>, ErrorSource> {
        if !self.line_byte_offset_index.borrow().is_some() {
            self.build_line_byte_offset_index();
        }

        if line_number < 1 {
            return Err(ErrorSource::LineNumberCannotBeZero);
        }

        if line_number == 1 {
            Ok(Some(0))
        } else if line_number - 2
            < self.line_byte_offset_index.borrow().as_ref().unwrap().len() as LineNumber
        {
            Ok(Some(
                self.line_byte_offset_index.borrow().as_ref().unwrap()[(line_number - 2) as usize],
            ))
        } else {
            Ok(None)
        }
    }
}

/// File data is stored in a [`SourceContext`], but always accessed through a [`SourceRef`].
///
/// This makes memory management much simpler when dealing with multiple source files loaded in
/// memory. See [`SourceContext`] for more details.
///
/// **Warning:** comparing source refs is done essentially by instance address. If the same file on
/// the filesystem is added to a source context twice, the source refs will not be equal.
#[derive(Clone)]
pub struct SourceRef<'source_context> {
    context: &'source_context SourceContext,
    id: SourceId,
}

impl<'source_context> SourceRef<'source_context> {
    /// Get the path of the source file on the filesystem.
    ///
    /// **Note:** This may be `"--"` if the source code was not read from a file.
    pub fn path(&self) -> &str {
        &self.context.sources[self.id].path
    }

    /// The data of the source file in bytes.
    pub fn data(&self) -> &[u8] {
        &self.context.sources[self.id].data
    }

    /// Get the location of the first byte of a line.
    ///
    /// It should be safe to assume that this is not in the middle of a grapheme cluster since it
    /// should be either the start of the file or the byte just after a newline.
    ///
    /// This is `O(1)` since these values are indexed during file loading.
    pub fn get_first_location_of_line(
        &self,
        line_number: LineNumber,
    ) -> Result<Option<SourceLocation>, ErrorSource> {
        if let Some(byte_offset) =
            self.context.sources[self.id].get_line_byte_offset(line_number)?
        {
            Ok(Some(SourceLocation {
                source: self.clone(),
                byte_offset,
                line_number,
                column_number: 1,
            }))
        } else {
            Ok(None)
        }
    }
}

impl<'source_context> fmt::Debug for SourceRef<'source_context> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("SourceRef")
            .field("id", &self.id)
            .field("path", &self.path())
            .finish()
    }
}

impl<'source_context> fmt::Display for SourceRef<'source_context> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.path())
    }
}

impl<'source_context> PartialEq for SourceRef<'source_context> {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

impl<'source_context> Eq for SourceRef<'source_context> {}

/// A context for managing source files.
///
/// One or more source files may be loaded using this context and then safely accessed after the
/// fact with [`SourceRef`] instances. Unloading source files is not supported, so [`SourceRef`]
/// instances all have the same lifespan as the [`SourceContext`].
///
/// # Example
///
/// ```no_run
/// use forge_parsing::sourcing::SourceContext;
///
/// let mut source_context = SourceContext::default();
///
/// let source_ref = source_context.add_from_file("path/to/file.txt").unwrap();
///
/// // You can now use `source_ref` to access the source file.
/// ```
#[derive(Default)]
pub struct SourceContext {
    sources: Vec<SourceData>,
    source_id_counter: SourceId,
}

impl SourceContext {
    /// Add a new source from raw byte data.
    ///
    /// # Example
    ///
    /// ```
    /// use forge_parsing::sourcing::SourceContext;
    ///
    /// let mut source_context = SourceContext::default();
    ///
    /// let source_ref = source_context.add_raw("path/to/file.txt", b"Hello, world!".to_vec()).unwrap();
    ///
    /// // You can now use `source_ref` to access the source file.
    /// ```
    ///
    /// # Arguments
    ///
    /// * `path` - The path of the source file on the filesystem. This may be `"--"` if the source
    ///            data was not read from a file.
    /// * `data` - The source data in bytes. This must be valid UTF-8 and is checked during reading.
    ///
    /// # Returns
    ///
    /// A [`SourceRef`] which may be used to access the source data. This can be thought of as a
    /// memory safe handle and is the only way to get access to the source data going forward.
    ///
    /// # Errors
    ///
    /// * [`ErrorSource::EmptyPath`] if `path` is an empty string.
    pub fn add_raw(&mut self, path: &str, data: Vec<u8>) -> Result<SourceRef, ErrorSource> {
        if path.is_empty() {
            return Err(ErrorSource::EmptyPath);
        }

        let id = self.next_id();

        self.sources.push(SourceData {
            path: path.to_owned(),
            data,
            line_byte_offset_index: RefCell::new(None),
        });

        Ok(SourceRef { context: self, id })
    }

    /// Add a new source from string data.
    ///
    /// **Note:** the string is consumed so no data is copied.
    ///
    /// # Example
    ///
    /// ```
    /// use forge_parsing::sourcing::SourceContext;
    ///
    /// let mut source_context = SourceContext::default();
    ///
    /// let source_ref = source_context.add_from_string("path/to/file.txt", "Hello, world!".to_owned()).unwrap();
    ///
    /// // You can now use `source_ref` to access the source file.
    /// ```
    ///
    /// # Arguments
    ///
    /// * `path` - The path of the source file on the filesystem. This may be `"--"` if the source
    ///            data was not read from a file.
    /// * `data` - The source data as a string.
    ///
    /// # Returns
    ///
    /// A [`SourceRef`] which may be used to access the source data. This can be thought of as a
    /// memory safe handle and is the only way to get access to the source data going forward.
    ///
    /// # Errors
    ///
    /// * [`ErrorSource::EmptyPath`] if `path` is an empty string.
    pub fn add_from_string(&mut self, path: &str, data: String) -> Result<SourceRef, ErrorSource> {
        self.add_raw(path, data.into_bytes())
    }

    /// Add a new source from a file on the filesystem.
    ///
    /// **Note:** the file is read in its entirety into memory during this function call. This makes
    /// the rest of the code simpler.
    ///
    /// # Example
    ///
    /// ```no_run
    /// use forge_parsing::sourcing::SourceContext;
    ///
    /// let mut source_context = SourceContext::default();
    ///
    /// let source_ref = source_context.add_from_file("path/to/file.txt").unwrap();
    ///
    /// // You can now use `source_ref` to access the source file.
    /// ```
    ///
    /// # Arguments
    ///
    /// * `path` - The path of the source file on the filesystem.
    ///
    /// # Returns
    ///
    /// A [`SourceRef`] which may be used to access the source data. This can be thought of as a
    /// memory safe handle and is the only way to get access to the source data going forward.
    ///
    /// # Errors
    ///
    /// * [`ErrorSource::EmptyPath`] if `path` is an empty string.
    /// * [`ErrorSource::Io`] if there was an IO or filesystem error reading the file.
    pub fn add_from_file(&mut self, path: &str) -> Result<SourceRef, ErrorSource> {
        let data = fs::read(path)?;
        self.add_raw(path, data)
    }

    fn next_id(&mut self) -> SourceId {
        let id = self.source_id_counter;
        self.source_id_counter += 1;
        id
    }
}

/// A referenced location with in a source.
///
/// **Warning:** locations from different sources are compared by their offsets only. Locations from
/// different files will be compared without error.
#[derive(Debug, Clone)]
pub struct SourceLocation<'source_context> {
    /// A reference to the source file for convenience.
    pub source: SourceRef<'source_context>,

    /// The offset of the location in bytes.
    ///
    /// **Warning:** if this byte offset is in the middle of a grapheme cluster, various functions
    /// may return errors or panic.
    pub byte_offset: SourceByteOffset,

    /// The line number of the location, starting at 1.
    pub line_number: LineNumber,

    /// The column number of the location, starting at 1.
    ///
    /// This is measured in grapheme clusters, not bytes.
    pub column_number: ColumnNumber,
}

impl<'source_context> SourceLocation<'source_context> {
    /// Create a new source location.
    ///
    /// **Warning:** you are responsible for making sure the values are accurate. They are not
    /// checked anywhere. It is much safer to use [`SourceLocation::new_at_start`] to create a
    /// location at the start of a source.
    ///
    /// # Arguments
    ///
    /// * `source` - The source file.
    /// * `byte_offset` - The offset of the location in bytes.
    /// * `line_number` - The line number of the location, starting at 1.
    /// * `column_number` - The column number of the location, starting at 1. This is measured in
    ///                     grapheme clusters, not bytes.
    ///
    /// # Errors
    ///
    /// * [`ErrorSource::LineNumberCannotBeZero`] if `line_number` is less than 1.
    /// * [`ErrorSource::ColumnNumberCannotBeZero`] if `column_number` is less than 1.
    pub fn new(
        source: SourceRef<'source_context>,
        byte_offset: SourceByteOffset,
        line_number: LineNumber,
        column_number: ColumnNumber,
    ) -> Result<Self, ErrorSource> {
        if line_number < 1 {
            return Err(ErrorSource::LineNumberCannotBeZero);
        }

        if column_number < 1 {
            return Err(ErrorSource::ColumnNumberCannotBeZero);
        }

        Ok(Self {
            source,
            byte_offset,
            line_number,
            column_number,
        })
    }

    /// Create a new source location at the start of a source.
    pub fn new_at_start(source: SourceRef<'source_context>) -> Self {
        Self {
            source,
            byte_offset: 0,
            line_number: 1,
            column_number: 1,
        }
    }

    /// Creates a source location which may be used for testing.
    ///
    /// This is essentially a shortcut to make test code simpler.
    #[cfg(test)]
    pub fn new_test(source: &'source_context SourceContext) -> Self {
        Self {
            source: SourceRef {
                context: source,
                id: 0,
            },
            byte_offset: 0,
            line_number: 1,
            column_number: 1,
        }
    }

    /// Gets a string between two source locations. The end is inclusive.
    ///
    /// It does not matter which comes first - it will work either way.
    ///
    /// **Warning:** if the bytes between the two locations are not valid UTF-8, this replace any
    /// invalid characters with an error character. See [`String::from_utf8_lossy`] for more
    /// details.
    pub fn string_between(&self, other: &Self) -> String {
        String::from_utf8_lossy(self.bytes_between(other)).to_string()
    }

    /// Gets a slice of the bytes between two source locations.
    ///
    /// It does not matter which comes first - it will work either way.
    fn bytes_between(&self, other: &Self) -> &[u8] {
        if self.byte_offset < other.byte_offset {
            &self.source.data()[self.byte_offset..other.byte_offset + 1]
        } else {
            &self.source.data()[other.byte_offset..self.byte_offset + 1]
        }
    }
}

impl<'source> fmt::Display for SourceLocation<'source> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{}:{}+{}",
            self.line_number, self.column_number, self.byte_offset
        )
    }
}

impl<'source> PartialEq for SourceLocation<'source> {
    fn eq(&self, other: &Self) -> bool {
        self.byte_offset == other.byte_offset
    }
}

impl<'source> Eq for SourceLocation<'source> {}

// We allow this because it's just a little bit less code to write
#[allow(clippy::non_canonical_partial_ord_impl)]
impl<'source> PartialOrd for SourceLocation<'source> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        self.byte_offset.partial_cmp(&other.byte_offset)
    }
}

impl<'source> Ord for SourceLocation<'source> {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.byte_offset.cmp(&other.byte_offset)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_source_data_build_line_byte_offset_index_unix() {
        let source = SourceData {
            path: "--".to_owned(),
            data: vec![b'a', b'\n', b'b', b'c', b'\n', b'd', b'\n'],
            line_byte_offset_index: RefCell::new(None),
        };

        assert!(source.get_line_byte_offset(0).is_err());
        assert_eq!(source.get_line_byte_offset(1).unwrap(), Some(0));
        assert_eq!(source.get_line_byte_offset(2).unwrap(), Some(2));
        assert_eq!(source.get_line_byte_offset(3).unwrap(), Some(5));
        assert_eq!(source.get_line_byte_offset(4).unwrap(), None);
    }

    #[test]
    fn test_source_data_build_line_byte_offset_index_dos() {
        let source = SourceData {
            path: "--".to_owned(),
            data: vec![
                b'a', b'\r', b'\n', b'b', b'c', b'\r', b'\n', b'd', b'\r', b'\n',
            ],
            line_byte_offset_index: RefCell::new(None),
        };

        assert!(source.get_line_byte_offset(0).is_err());
        assert_eq!(source.get_line_byte_offset(1).unwrap(), Some(0));
        assert_eq!(source.get_line_byte_offset(2).unwrap(), Some(3));
        assert_eq!(source.get_line_byte_offset(3).unwrap(), Some(7));
        assert_eq!(source.get_line_byte_offset(4).unwrap(), None);
    }
}
