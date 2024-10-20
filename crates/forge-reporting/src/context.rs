use std::io;

use forge_parsing::LineNumber;
use sorted_vec::SortedVec;
use termcolor::{Color, ColorSpec, WriteColor};

use crate::rendering::render_report;

use super::report::Report;

/// A context which is used for reporting errors and warnings which can be printed at the end of
/// execution (or at any time).
///
/// # Example
///
/// ```
/// use forge_reporting::{Report, ReportingContext, Severity};
///
/// let mut reporting_context = ReportingContext::default();
///
/// reporting_context.push(Report::new(Severity::Error, "This is an error"));
/// ```
#[derive(Default)]
pub struct ReportingContext<'source_context> {
    reports: SortedVec<Report<'source_context>>,
    error_count: u32,
    warning_count: u32,
    max_line_number: Option<LineNumber>,
}

impl<'source_context> ReportingContext<'source_context> {
    /// Push a new report.
    pub fn push(&mut self, report: Report<'source_context>) {
        if report.severity().is_error() {
            self.error_count += 1;
        } else if report.severity().is_warning() {
            self.warning_count += 1;
        }

        if let Some(line_number) = report.last_line_number() {
            if let Some(max_line_number) = self.max_line_number {
                if line_number > max_line_number {
                    self.max_line_number = Some(line_number);
                }
            } else {
                self.max_line_number = Some(line_number);
            }
        }

        self.reports.insert(report);
    }

    /// Iterate over pushed reports.
    pub fn iter(&self) -> impl Iterator<Item = &Report<'source_context>> {
        self.reports.iter()
    }

    /// Get the number of error reports pushed so far.
    pub fn error_count(&self) -> u32 {
        self.error_count
    }

    /// Get the number of warning reports pushed so far.
    pub fn warning_count(&self) -> u32 {
        self.warning_count
    }

    /// Get the maximum line number out of all reports pushed so far.
    ///
    /// # Returns
    ///
    /// `None` if no reports have a line number.
    pub fn max_line_number(&self) -> Option<LineNumber> {
        self.max_line_number
    }

    /// Render all reports to the given writer.
    ///
    /// This will:
    /// * print them in a sorted order.
    /// * print a summary of the counts of errors and warnings.
    pub fn render(&self, writer: &mut impl WriteColor) -> io::Result<()> {
        let line_number_width = self
            .max_line_number
            .map_or(0, |line_number| line_number.to_string().len());

        for report in self.reports.iter() {
            render_report(writer, report, line_number_width)?;
        }

        if self.error_count > 0 && self.warning_count == 0 {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Red)))?;

            writeln!(
                writer,
                "failed with {} error{}",
                self.error_count,
                if self.error_count == 1 { "" } else { "s" }
            )?;

            writer.set_color(&ColorSpec::new())?;
        } else if self.error_count == 0 && self.warning_count > 0 {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Yellow)))?;

            writeln!(
                writer,
                "succeeded with {} warning{}",
                self.warning_count,
                if self.warning_count == 1 { "" } else { "s" }
            )?;

            writer.set_color(&ColorSpec::new())?;
        } else if self.error_count > 0 && self.warning_count > 0 {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Red)))?;

            write!(
                writer,
                "failed with {} error{}",
                self.error_count,
                if self.error_count == 1 { "" } else { "s" }
            )?;

            writer.set_color(ColorSpec::new().set_fg(Some(Color::Yellow)))?;

            writeln!(
                writer,
                " and {} warning{}",
                self.warning_count,
                if self.warning_count == 1 { "" } else { "s" }
            )?;

            writer.set_color(&ColorSpec::new())?;
        }

        Ok(())
    }
}
