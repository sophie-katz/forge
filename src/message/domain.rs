use std::{cmp::Ordering, collections::HashMap};

use sorted_vec::SortedVec;

use crate::source::domain::{ColumnNumber, LineNumber, SourceLocation, SourceOffset, SourceRef};

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Severity {
    FatalError,
    Error,
    Warning,
    Info,
    Fix,
    Suggestion,
    Note,
    Debug,
}

impl Severity {
    pub fn is_error(&self) -> bool {
        match self {
            Severity::FatalError | Severity::Error => true,
            _ => false,
        }
    }

    pub fn is_warning(&self) -> bool {
        match self {
            Severity::Warning => true,
            _ => false,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Message<'source_context> {
    severity: Severity,
    source: Option<SourceRef<'source_context>>,
    first_location_line_number: Option<LineNumber>,
    first_location_column_number: Option<ColumnNumber>,
    last_location_line_number: Option<LineNumber>,
    last_location_column_number: Option<ColumnNumber>,
    text: String,
    children: Vec<Message<'source_context>>,
}

impl<'source_context> Message<'source_context> {
    pub fn new(severity: Severity, text: String) -> Self {
        Message {
            severity,
            source: None,
            first_location_line_number: None,
            first_location_column_number: None,
            last_location_line_number: None,
            last_location_column_number: None,
            text,
            children: Vec::new(),
        }
    }

    pub fn new_from_source(
        severity: Severity,
        source: SourceRef<'source_context>,
        text: String,
    ) -> Self {
        Message {
            severity,
            source: Some(source),
            first_location_line_number: None,
            first_location_column_number: None,
            last_location_line_number: None,
            last_location_column_number: None,
            text,
            children: Vec::new(),
        }
    }

    pub fn new_from_line(
        severity: Severity,
        source: SourceRef<'source_context>,
        line_number: LineNumber,
        text: String,
    ) -> Self {
        Message {
            severity,
            source: Some(source),
            first_location_line_number: Some(line_number),
            first_location_column_number: None,
            last_location_line_number: Some(line_number),
            last_location_column_number: None,
            text,
            children: Vec::new(),
        }
    }

    pub fn new_from_location_raw(
        severity: Severity,
        source: SourceRef<'source_context>,
        line_number: LineNumber,
        column_number: ColumnNumber,
        text: String,
    ) -> Self {
        Message {
            severity,
            source: Some(source),
            first_location_line_number: Some(line_number),
            first_location_column_number: Some(column_number),
            last_location_line_number: Some(line_number),
            last_location_column_number: Some(column_number),
            text,
            children: Vec::new(),
        }
    }

    pub fn new_from_location(
        severity: Severity,
        location: SourceLocation<'source_context>,
        text: String,
    ) -> Self {
        Self::new_from_location_raw(
            severity,
            location.source,
            location.line_number,
            location.column_number,
            text,
        )
    }

    pub fn new_from_range_raw(
        severity: Severity,
        source: SourceRef<'source_context>,
        first_location_line_number: LineNumber,
        first_location_column_number: ColumnNumber,
        last_location_line_number: LineNumber,
        last_location_column_number: ColumnNumber,
        text: String,
    ) -> Self {
        Message {
            severity,
            source: Some(source),
            first_location_line_number: Some(first_location_line_number),
            first_location_column_number: Some(first_location_column_number),
            last_location_line_number: Some(last_location_line_number),
            last_location_column_number: Some(last_location_column_number),
            text,
            children: Vec::new(),
        }
    }

    pub fn new_from_range(
        severity: Severity,
        first_location: SourceLocation<'source_context>,
        last_location: SourceLocation<'source_context>,
        text: String,
    ) -> Self {
        Self::new_from_range_raw(
            severity,
            first_location.source,
            first_location.line_number,
            first_location.column_number,
            last_location.line_number,
            last_location.column_number,
            text,
        )
    }

    pub fn add_child(&mut self, child: Message<'source_context>) {
        self.children.push(child);
    }

    pub fn iter_children(&self) -> impl Iterator<Item = &Message<'source_context>> {
        self.children.iter()
    }

    pub fn severity(&self) -> Severity {
        self.severity
    }

    pub fn source(&self) -> Option<SourceRef<'source_context>> {
        self.source
    }

    pub fn first_location_line_number(&self) -> Option<LineNumber> {
        self.first_location_line_number
    }

    pub fn first_location_column_number(&self) -> Option<ColumnNumber> {
        self.first_location_column_number
    }

    pub fn last_location_line_number(&self) -> Option<LineNumber> {
        self.last_location_line_number
    }

    pub fn last_location_column_number(&self) -> Option<ColumnNumber> {
        self.last_location_column_number
    }

    pub fn line_number_range(&self) -> Option<(LineNumber, LineNumber)> {
        match (
            self.first_location_line_number,
            self.last_location_line_number,
        ) {
            (Some(first_line_number), Some(last_line_number)) => {
                Some((first_line_number, last_line_number))
            }
            (Some(first_line_number), None) => Some((first_line_number, first_line_number)),
            _ => None,
        }
    }

    pub fn text(&self) -> &str {
        &self.text
    }
}

impl<'source_context> PartialOrd for Message<'source_context> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl<'source_context> Ord for Message<'source_context> {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        match self.severity.cmp(&other.severity) {
            Ordering::Equal => {}
            ord => return ord,
        }
        match (
            self.first_location_line_number,
            other.first_location_line_number,
        ) {
            (None, None) => {}
            (Some(_), None) => return Ordering::Greater,
            (None, Some(_)) => return Ordering::Less,
            (Some(self_first_location_line_number), Some(other_first_location_line_number)) => {
                match self_first_location_line_number.cmp(&other_first_location_line_number) {
                    Ordering::Equal => {}
                    ord => return ord,
                }
            }
        }
        match (
            self.first_location_column_number,
            other.first_location_column_number,
        ) {
            (None, None) => {}
            (Some(_), None) => return Ordering::Greater,
            (None, Some(_)) => return Ordering::Less,
            (Some(self_first_location_column_number), Some(other_first_location_column_number)) => {
                match self_first_location_column_number.cmp(&other_first_location_column_number) {
                    Ordering::Equal => {}
                    ord => return ord,
                }
            }
        }
        Ordering::Equal
    }
}

pub struct MessageContext<'source_context> {
    messages: SortedVec<Message<'source_context>>,
    counts_by_severity: HashMap<Severity, usize>,
    error_count: usize,
    warning_count: usize,
}

impl<'source_context> MessageContext<'source_context> {
    pub fn new() -> Self {
        MessageContext {
            messages: SortedVec::new(),
            counts_by_severity: HashMap::new(),
            error_count: 0,
            warning_count: 0,
        }
    }

    pub fn add(&mut self, message: Message<'source_context>) {
        let severity = message.severity;

        self.messages.insert(message);
        let count = self.counts_by_severity.entry(severity).or_insert(0);
        *count += 1;

        if severity.is_error() {
            self.error_count += 1;
        } else if severity.is_warning() {
            self.warning_count += 1;
        }
    }

    pub fn iter_messages(&self) -> impl Iterator<Item = &Message<'source_context>> {
        self.messages.iter()
    }

    pub fn error_count(&self) -> usize {
        self.error_count
    }

    pub fn warning_count(&self) -> usize {
        self.warning_count
    }

    pub fn max_line_number(&self) -> Option<LineNumber> {
        self.messages
            .iter()
            .filter_map(|message| message.line_number_range())
            .map(|(_, last_line_number)| last_line_number)
            .max()
    }
}
