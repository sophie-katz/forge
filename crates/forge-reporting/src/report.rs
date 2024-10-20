use std::cmp::Ordering;

use forge_parsing::{
    sourcing::{SourceLocation, SourceRef},
    LineNumber,
};

/// The severity of a report.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Severity {
    /// A suggestion for a fix.
    ///
    /// This will always be a child report of an error or warning.
    SuggestedFix,

    /// A warning.
    Warning,

    /// An error.
    Error,

    /// A fatal error which caused the compilation process to terminate.
    FatalError,
}

impl Severity {
    /// Check if the severity is an error.
    pub fn is_error(&self) -> bool {
        matches!(self, Severity::FatalError | Severity::Error)
    }

    /// Check if the severity is a warning.
    pub fn is_warning(&self) -> bool {
        matches!(self, Severity::Warning)
    }

    /// Check if the severity is applicable for child reports.
    ///
    /// # Returns
    ///
    /// * `true` if the severity is applicable only for child reports
    /// * `false` if the severity is applicable only for top-level reports
    pub fn is_for_child_report(&self) -> bool {
        matches!(self, Severity::SuggestedFix)
    }
}

/// The code of a report.
///
/// This is used to create unique error codes that are searchable.
#[allow(missing_docs)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ReportCode {
    /// A code needs to be created for this type of report.
    NeedsToBeCreated,

    ES001UnexpectedCharacter,
    ES002NoClosingQuoteForStringOrCharacter,
}

impl ReportCode {
    /// Gets the severity of the report code (if applicable).
    pub fn severity(&self) -> Option<Severity> {
        match self {
            Self::NeedsToBeCreated => None,
            Self::ES001UnexpectedCharacter => Some(Severity::Error),
            Self::ES002NoClosingQuoteForStringOrCharacter => Some(Severity::Error),
        }
    }

    /// Gets the name of the report code.
    pub fn name(&self) -> Option<&'static str> {
        match self {
            Self::NeedsToBeCreated => None,
            Self::ES001UnexpectedCharacter => Some("ES001"),
            Self::ES002NoClosingQuoteForStringOrCharacter => Some("ES002"),
        }
    }
}

/// A report of an error or warning.
#[allow(missing_docs)]
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Report<'source_context> {
    /// A global report which is not specific to any source.
    Global { base: ReportBase<'source_context> },
    /// A report from a specific source.
    FromSource {
        base: ReportBase<'source_context>,
        source: SourceRef<'source_context>,
    },
    /// A report from a specific line in a source.
    FromLine {
        base: ReportBase<'source_context>,
        source: SourceRef<'source_context>,
        line_number: LineNumber,
    },
    /// A report from a specific grapheme cluster location in a source.
    FromLocation {
        base: ReportBase<'source_context>,
        location: SourceLocation<'source_context>,
    },
    /// A report from a specific range of grapheme clusters in a source.
    FromRange {
        base: ReportBase<'source_context>,
        first: SourceLocation<'source_context>,
        last: SourceLocation<'source_context>,
    },
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ReportBase<'source_context> {
    severity: Severity,
    code: Option<ReportCode>,
    message: String,
    children: Vec<Report<'source_context>>,
}

impl<'source_context> Report<'source_context> {
    /// Create a new global report.
    pub fn new_global(severity: Severity, code: ReportCode, message: String) -> Self {
        Self::Global {
            base: ReportBase {
                severity,
                code: Some(code),
                message,
                children: Vec::new(),
            },
        }
        .verified(false)
    }

    /// Create a new report from a source.
    pub fn new_from_source(
        severity: Severity,
        code: ReportCode,
        source: SourceRef<'source_context>,
        message: String,
    ) -> Self {
        Self::FromSource {
            base: ReportBase {
                severity,
                code: Some(code),
                message,
                children: Vec::new(),
            },
            source,
        }
        .verified(false)
    }

    /// Create a new report from a line in a source.
    pub fn new_from_line(
        severity: Severity,
        code: ReportCode,
        source: SourceRef<'source_context>,
        line_number: LineNumber,
        message: String,
    ) -> Self {
        Self::FromLine {
            base: ReportBase {
                severity,
                code: Some(code),
                message,
                children: Vec::new(),
            },
            source,
            line_number,
        }
        .verified(false)
    }

    /// Create a new report from a location in a source.
    pub fn new_from_location(
        severity: Severity,
        code: ReportCode,
        location: SourceLocation<'source_context>,
        message: String,
    ) -> Self {
        Self::FromLocation {
            base: ReportBase {
                severity,
                code: Some(code),
                message,
                children: Vec::new(),
            },
            location,
        }
        .verified(false)
    }

    /// Create a new report from a range in a source.
    pub fn new_from_range(
        severity: Severity,
        code: ReportCode,
        first: SourceLocation<'source_context>,
        last: SourceLocation<'source_context>,
        message: String,
    ) -> Self {
        Self::FromRange {
            base: ReportBase {
                severity,
                code: Some(code),
                message,
                children: Vec::new(),
            },
            first,
            last,
        }
        .verified(false)
    }

    fn verified(self, is_child: bool) -> Self {
        self.verify_base(is_child);
        self.verify_variant(is_child);
        self.verify_children(is_child);
        self
    }

    fn verify_base(&self, is_child: bool) {
        if self.message().is_empty() {
            panic!("Reports should have a message");
        }

        if !self.message().is_ascii() {
            panic!("Report messages should be entirely ASCII for compatibility");
        }

        if !self.message().chars().next().unwrap().is_ascii_uppercase() {
            panic!("Report messages should start with an uppercase character");
        }

        if is_child {
            if !self.severity().is_for_child_report() {
                panic!(
                    "Severity {:?} is not applicable to child reports",
                    self.severity()
                );
            }

            if self.code().is_some() {
                panic!("Child reports should not have a code");
            }
        } else {
            if self.severity().is_for_child_report() {
                panic!(
                    "Severity {:?} is only applicable to child reports",
                    self.severity()
                );
            }

            if self.code().is_none() {
                panic!("Reports should have a code");
            }

            let code_severity = self.code().and_then(|code| code.severity());

            if code_severity.is_some() && code_severity != Some(self.severity()) {
                panic!(
                    "Severity mismatch between severity {:?} for code {:?} and actual severity {:?}",
                    code_severity,
                    self.code(),
                    self.severity()
                );
            }
        }
    }

    fn verify_variant(&self, is_child: bool) {
        if is_child && !matches!(self, Self::Global { .. }) {
            panic!("Child reports should only be created from the global variant");
        }
    }

    fn verify_children(&self, is_child: bool) {
        if is_child && !self.base().children.is_empty() {
            panic!("Child reports should not have children");
        }
    }

    /// Get the severity of the report.
    pub fn severity(&self) -> Severity {
        self.base().severity
    }

    /// Get the code of the report (if applicable).
    pub fn code(&self) -> Option<ReportCode> {
        self.base().code
    }

    /// Get the message of the report.
    pub fn message(&self) -> &str {
        &self.base().message
    }

    /// Push a report as a child of the current one.
    pub fn push_child(&mut self, severity: Severity, message: String) -> &mut Self {
        let child = Self::Global {
            base: ReportBase {
                severity,
                code: None,
                message,
                children: Vec::new(),
            },
        }
        .verified(true);

        self.base_mut().children.push(child);
        self
    }

    /// Iterate over the children of the report.
    pub fn iter_children(&self) -> impl Iterator<Item = &Self> {
        self.base().children.iter()
    }

    /// Get the source of the report (if applicable).
    pub fn source(&self) -> Option<&SourceRef<'source_context>> {
        match self {
            Report::Global { .. } => None,
            Report::FromSource { source, .. } => Some(source),
            Report::FromLine { source, .. } => Some(source),
            Report::FromLocation { location, .. } => Some(&location.source),
            Report::FromRange { first, .. } => Some(&first.source),
        }
    }

    /// Get the line number of the report (if applicable).
    pub fn first_line_number(&self) -> Option<LineNumber> {
        match self {
            Report::FromLine { line_number, .. } => Some(*line_number),
            Report::FromLocation { location, .. } => Some(location.line_number),
            Report::FromRange { first, .. } => Some(first.line_number),
            _ => None,
        }
    }

    /// Get the line number of the report (if applicable).
    pub fn last_line_number(&self) -> Option<LineNumber> {
        match self {
            Report::FromLine { line_number, .. } => Some(*line_number),
            Report::FromLocation { location, .. } => Some(location.line_number),
            Report::FromRange { last, .. } => Some(last.line_number),
            _ => None,
        }
    }

    /// Get the first location of the report (if applicable).
    pub fn first_location(&self) -> Option<&SourceLocation<'source_context>> {
        match self {
            Report::FromLocation { location, .. } => Some(location),
            Report::FromRange { first, .. } => Some(first),
            _ => None,
        }
    }

    /// Get the last location of the report (if applicable).
    pub fn last_location(&self) -> Option<&SourceLocation<'source_context>> {
        match self {
            Report::FromLocation { location, .. } => Some(location),
            Report::FromRange { last, .. } => Some(last),
            _ => None,
        }
    }

    fn base(&self) -> &ReportBase<'source_context> {
        match self {
            Report::Global { base } => base,
            Report::FromSource { base, .. } => base,
            Report::FromLine { base, .. } => base,
            Report::FromLocation { base, .. } => base,
            Report::FromRange { base, .. } => base,
        }
    }

    fn base_mut(&mut self) -> &mut ReportBase<'source_context> {
        match self {
            Report::Global { base } => base,
            Report::FromSource { base, .. } => base,
            Report::FromLine { base, .. } => base,
            Report::FromLocation { base, .. } => base,
            Report::FromRange { base, .. } => base,
        }
    }
}

impl<'source_context> PartialOrd for Report<'source_context> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl<'source_context> Ord for Report<'source_context> {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        // Order by severity first
        match self.severity().cmp(&other.severity()) {
            Ordering::Equal => {}
            // Higher severity reports should come first
            Ordering::Greater => return Ordering::Less,
            Ordering::Less => return Ordering::Greater,
        }

        // Then by line number
        match (self.first_line_number(), other.first_line_number()) {
            (None, None) => {}
            (Some(_), None) => return Ordering::Greater,
            (None, Some(_)) => return Ordering::Less,
            (Some(self_first_line_number), Some(other_first_line_number)) => {
                match self_first_line_number.cmp(&other_first_line_number) {
                    Ordering::Equal => {}
                    ord => return ord,
                }
            }
        }

        // Then by column number
        match (
            self.first_location().map(|location| location.column_number),
            other
                .first_location()
                .map(|location| location.column_number),
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
