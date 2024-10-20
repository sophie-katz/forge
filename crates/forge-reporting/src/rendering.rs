use forge_parsing::{
    sourcing::{ErrorSource, SourceLocation, SourceReader},
    LineNumber,
};
use std::{cmp, io};
use termcolor::{Color, ColorSpec, WriteColor};

use crate::{
    report::{Report, Severity},
    unicode::choose_unicode,
};

const REPORT_INDENT: usize = 2;
const TAB_WIDTH: usize = 4;

/// Render a report to a writer.
pub fn render_report(
    writer: &mut impl WriteColor,
    report: &Report,
    line_number_width: usize,
) -> Result<(), io::Error> {
    render_report_location(writer, report)?;
    render_severity_and_code(writer, report)?;
    render_message_separator(writer)?;
    render_message(writer, report)?;
    writeln!(writer)?;
    render_context_lines(writer, report, line_number_width)?;
    writeln!(writer)?;

    for child in report.iter_children() {
        render_report_child(writer, child, line_number_width)?;
    }

    Ok(())
}

fn render_report_child(
    writer: &mut impl WriteColor,
    report: &Report,
    line_number_width: usize,
) -> Result<(), io::Error> {
    for _ in 0..REPORT_INDENT {
        write!(writer, " ")?;
    }

    render_severity_and_code(writer, report)?;
    render_message_separator(writer)?;
    render_message(writer, report)?;
    writeln!(writer)?;
    writeln!(writer)?;
    render_context_lines(writer, report, line_number_width)?;

    Ok(())
}

fn render_report_location(writer: &mut impl WriteColor, report: &Report) -> Result<(), io::Error> {
    if let Some(source) = report.source() {
        writer.set_color(ColorSpec::new().set_fg(Some(Color::Cyan)))?;

        write!(writer, "{}:", source.path())?;

        if let Some(line_number) = report.first_line_number() {
            write!(writer, "{}:", line_number)?;

            if let Some(column_number) = report
                .first_location()
                .map(|location| location.column_number)
            {
                write!(writer, "{}:", column_number)?;
            }
        }

        write!(writer, " ")?;
        writer.set_color(&ColorSpec::new())?;
    }

    Ok(())
}

fn render_severity_and_code(
    writer: &mut impl WriteColor,
    report: &Report,
) -> Result<(), io::Error> {
    set_color_for_severity(writer, report.severity(), false)?;

    match report.severity() {
        Severity::FatalError => {
            write!(writer, "fatal error")?;
        }
        Severity::Error => {
            write!(writer, "error")?;
        }
        Severity::Warning => {
            write!(writer, "warning")?;
        }
        Severity::SuggestedFix => {
            write!(writer, "suggested fix")?;
        }
    }

    if let Some(name) = report.code().and_then(|code| code.name()) {
        write!(writer, " {}", name)?;
    }

    writer.set_color(&ColorSpec::new())?;

    Ok(())
}

fn render_message_separator(writer: &mut impl WriteColor) -> Result<(), io::Error> {
    writer.set_color(ColorSpec::new().set_dimmed(true))?;

    write!(writer, " - ")?;

    writer.set_color(&ColorSpec::new())?;

    Ok(())
}

fn render_message(writer: &mut impl WriteColor, report: &Report) -> Result<(), io::Error> {
    writer.set_color(ColorSpec::new().set_bold(true))?;

    write!(writer, "{}", report.message())?;

    writer.set_color(&ColorSpec::new())?;

    Ok(())
}

fn render_context_lines(
    writer: &mut impl WriteColor,
    report: &Report,
    line_number_width: usize,
) -> Result<(), io::Error> {
    if let Some(lines) = get_context_lines_for_range(report).ok().flatten() {
        let lines = trim_context_lines(lines);

        for (index, line) in lines.into_iter().enumerate() {
            render_context_line_text(
                writer,
                report,
                report
                    .first_line_number()
                    .map(|first_line_number| first_line_number + (index as u32)),
                line_number_width,
                &line,
            )?;

            if line.grapheme_clusters_within_range > 0 {
                render_context_line_underline(writer, report, line_number_width, &line)?;
            }
        }
    }

    Ok(())
}

fn trim_context_lines(mut lines: Vec<SplitContextLines>) -> Vec<SplitContextLines> {
    let mut min_spaces_before: Option<usize> = None;

    for line in lines.iter() {
        let spaces_before = count_spaces_at_start(line.before_range.as_str());

        min_spaces_before = match min_spaces_before {
            Some(min_spaces_before) => Some(cmp::min(min_spaces_before, spaces_before)),
            None => Some(spaces_before),
        };
    }

    if let Some(min_spaces_before) = min_spaces_before {
        if min_spaces_before > 0 {
            for line in lines.iter_mut() {
                line.before_range = line.before_range.chars().skip(min_spaces_before).collect();

                line.grapheme_clusters_before_range -= min_spaces_before;
            }
        }
    }

    lines
}

fn count_spaces_at_start(value: &str) -> usize {
    value.chars().take_while(|&c| c == ' ').count()
}

fn render_context_line_text(
    writer: &mut impl WriteColor,
    report: &Report,
    line_number: Option<LineNumber>,
    line_number_width: usize,
    line: &SplitContextLines,
) -> Result<(), io::Error> {
    render_line_number(writer, line_number, line_number_width)?;

    write!(writer, "{}", line.before_range)?;

    if !line.within_range.is_empty() {
        set_color_for_severity(writer, report.severity(), true)?;
        write!(writer, "{}", line.within_range)?;
        writer.set_color(&ColorSpec::new())?;
    }

    write!(writer, "{}", line.after_range)?;

    writeln!(writer)?;

    Ok(())
}

fn render_context_line_underline(
    writer: &mut impl WriteColor,
    report: &Report,
    line_number_width: usize,
    line: &SplitContextLines,
) -> Result<(), io::Error> {
    render_line_number(writer, None, line_number_width)?;

    for _ in 0..line.grapheme_clusters_before_range {
        write!(writer, " ")?;
    }

    set_color_for_severity(writer, report.severity(), true)?;

    for _ in 0..line.grapheme_clusters_within_range {
        write!(writer, "{}", choose_unicode("▔", "^"))?;
    }

    writer.set_color(&ColorSpec::new())?;

    writeln!(writer)?;

    Ok(())
}

fn render_line_number(
    writer: &mut impl WriteColor,
    line_number: Option<LineNumber>,
    line_number_width: usize,
) -> Result<(), io::Error> {
    writer.set_color(ColorSpec::new().set_dimmed(true))?;

    if let Some(line_number) = line_number {
        write!(
            writer,
            "{: >1$}",
            line_number,
            line_number_width + REPORT_INDENT
        )?;
    } else {
        for _ in 0..line_number_width + REPORT_INDENT {
            write!(writer, " ")?;
        }
    }

    write!(writer, "{}", choose_unicode("▕ ", " | "))?;

    writer.set_color(&ColorSpec::new())?;

    Ok(())
}

fn set_color_for_severity(
    writer: &mut impl WriteColor,
    severity: Severity,
    bold: bool,
) -> Result<(), io::Error> {
    match severity {
        Severity::FatalError | Severity::Error => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Red)).set_bold(bold))?;
        }
        Severity::Warning => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Yellow)).set_bold(bold))?;
        }
        Severity::SuggestedFix => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Green)).set_bold(bold))?;
        }
    }

    Ok(())
}

fn get_context_lines_for_range(
    report: &Report,
) -> Result<Option<Vec<SplitContextLines>>, ErrorSource> {
    let first_line_number = match report.first_line_number() {
        Some(line_number) => line_number,
        None => return Ok(None),
    };

    let last_line_number = match report.last_line_number() {
        Some(line_number) => line_number,
        None => return Ok(None),
    };

    let mut lines = Vec::new();

    for line_number in first_line_number..=last_line_number {
        match get_context_line_by_number(report, line_number)? {
            Some(line) => lines.push(line),
            _ => return Ok(None),
        };
    }

    Ok(Some(lines))
}

struct SplitContextLines {
    before_range: String,
    grapheme_clusters_before_range: usize,
    within_range: String,
    grapheme_clusters_within_range: usize,
    after_range: String,
}

impl SplitContextLines {
    fn add_grapheme_cluster(&mut self, part: ContextLinePart, grapheme: &str) {
        match part {
            ContextLinePart::Before => {
                self.before_range.push_str(grapheme);
                self.grapheme_clusters_before_range += 1;
            }
            ContextLinePart::Within => {
                self.within_range.push_str(grapheme);
                self.grapheme_clusters_within_range += 1;
            }
            ContextLinePart::After => self.after_range.push_str(grapheme),
        }
    }
}

fn get_context_line_by_number(
    report: &Report,
    line_number: LineNumber,
) -> Result<Option<SplitContextLines>, ErrorSource> {
    let source = match report.source() {
        Some(source) => source,
        None => return Ok(None),
    };

    let location = match source.get_first_location_of_line(line_number)? {
        Some(location) => location,
        None => return Ok(None),
    };

    let mut reader = SourceReader::new(location);

    let mut result = SplitContextLines {
        before_range: String::new(),
        grapheme_clusters_before_range: 0,
        within_range: String::new(),
        grapheme_clusters_within_range: 0,
        after_range: String::new(),
    };

    while reader.is_more() {
        let grapheme_cluster = reader.peek_next_grapheme_cluster()?;

        if grapheme_cluster == "\n" {
            break;
        }

        let part = get_context_line_part(
            &reader.current_location(),
            &report.first_location(),
            &report.last_location(),
        );

        if grapheme_cluster == "\t" {
            for _ in 0..TAB_WIDTH {
                result.add_grapheme_cluster(part, " ");
            }
        } else {
            result.add_grapheme_cluster(part, grapheme_cluster.as_str());
        }

        reader.read_next_grapheme_cluster()?;
    }

    Ok(Some(result))
}

#[derive(Clone, Copy)]
enum ContextLinePart {
    Before,
    Within,
    After,
}

fn get_context_line_part(
    current_location: &SourceLocation,
    first_location: &Option<&SourceLocation>,
    last_location: &Option<&SourceLocation>,
) -> ContextLinePart {
    match (first_location, last_location) {
        (Some(first_location), Some(last_location)) => {
            if current_location < first_location {
                ContextLinePart::Before
            } else if current_location <= last_location {
                ContextLinePart::Within
            } else {
                ContextLinePart::After
            }
        }
        (Some(first_location), None) => {
            if current_location < first_location {
                ContextLinePart::Before
            } else {
                ContextLinePart::Within
            }
        }
        (None, Some(last_location)) => {
            if current_location < last_location {
                ContextLinePart::Within
            } else {
                ContextLinePart::After
            }
        }
        (None, None) => ContextLinePart::Before,
    }
}

#[cfg(test)]
mod tests {
    use forge_parsing::sourcing::SourceContext;
    use termcolor::Buffer;

    use crate::report::{Report, ReportCode, Severity};

    use super::*;

    #[test]
    fn test_global() {
        let report = Report::new_global(
            Severity::Error,
            ReportCode::ES001UnexpectedCharacter,
            "Unexpected character".to_owned(),
        );

        let mut output = Buffer::no_color();

        render_report(&mut output, &report, 3).unwrap();

        let output = String::from_utf8(output.into_inner()).unwrap();

        assert_eq!(output, "error ES001 - Unexpected character\n\n");
    }

    #[test]
    fn test_source() {
        let mut source_context = SourceContext::default();

        let source = source_context
            .add_from_string(
                "test.frg",
                "this is some text\n\nsome more text\nsdkfjhskdf\n\n".to_owned(),
            )
            .unwrap();

        let report = Report::new_from_source(
            Severity::Error,
            ReportCode::ES001UnexpectedCharacter,
            source,
            "Unexpected character".to_owned(),
        );

        let mut output = Buffer::no_color();

        render_report(&mut output, &report, 3).unwrap();

        let output = String::from_utf8(output.into_inner()).unwrap();

        assert_eq!(output, "test.frg: error ES001 - Unexpected character\n\n");
    }

    #[test]
    fn test_line() {
        let mut source_context = SourceContext::default();

        let source = source_context
            .add_from_string(
                "test.frg",
                "this is some text\n\nsome more text\nsdkfjhskdf\n\n".to_owned(),
            )
            .unwrap();

        let report = Report::new_from_line(
            Severity::Error,
            ReportCode::ES001UnexpectedCharacter,
            source,
            3,
            "Unexpected character".to_owned(),
        );

        let mut output = Buffer::no_color();

        render_report(&mut output, &report, 3).unwrap();

        let output = String::from_utf8(output.into_inner()).unwrap();

        assert_eq!(
            output,
            "test.frg:3: error ES001 - Unexpected character\n    3▕ some more text\n\n"
        );
    }

    #[test]
    fn test_location() {
        let mut source_context = SourceContext::default();

        let source = source_context
            .add_from_string(
                "test.frg",
                "this is some text\n\nsome more text\nsdkfjhskdf\n\n".to_owned(),
            )
            .unwrap();

        let mut source_reader = SourceReader::new(SourceLocation::new_at_start(source));

        for _ in 0..13 {
            source_reader.read_next_grapheme_cluster().unwrap();
        }

        let report = Report::new_from_location(
            Severity::Error,
            ReportCode::ES001UnexpectedCharacter,
            source_reader.current_location(),
            "Unexpected character".to_owned(),
        );

        let mut output = Buffer::no_color();

        render_report(&mut output, &report, 3).unwrap();

        let output = String::from_utf8(output.into_inner()).unwrap();

        assert_eq!(output, "test.frg:1:14: error ES001 - Unexpected character\n    1▕ this is some text\n     ▕              ▔\n\n");
    }

    #[test]
    fn test_range_single_line() {
        let mut source_context = SourceContext::default();

        let source = source_context
            .add_from_string(
                "test.frg",
                "this is some text\n\nsome more text\nsdkfjhskdf\n\n".to_owned(),
            )
            .unwrap();

        let mut source_reader = SourceReader::new(SourceLocation::new_at_start(source));

        for _ in 0..13 {
            source_reader.read_next_grapheme_cluster().unwrap();
        }

        let first_location = source_reader.current_location();

        for _ in 0..4 {
            source_reader.read_next_grapheme_cluster().unwrap();
        }

        let last_location = source_reader.current_location();

        let report = Report::new_from_range(
            Severity::Error,
            ReportCode::ES001UnexpectedCharacter,
            first_location,
            last_location,
            "Unexpected character".to_owned(),
        );

        let mut output = Buffer::no_color();

        render_report(&mut output, &report, 3).unwrap();

        let output = String::from_utf8(output.into_inner()).unwrap();

        assert_eq!(output, "test.frg:1:14: error ES001 - Unexpected character\n    1▕ this is some text\n     ▕              ▔▔▔▔\n\n");
    }

    #[test]
    fn test_range_multi_line() {
        let mut source_context = SourceContext::default();

        let source = source_context
            .add_from_string(
                "test.frg",
                "this is some text\n\nsome more text\nsdkfjhskdf\n\n".to_owned(),
            )
            .unwrap();

        let mut source_reader = SourceReader::new(SourceLocation::new_at_start(source));

        for _ in 0..13 {
            source_reader.read_next_grapheme_cluster().unwrap();
        }

        let first_location = source_reader.current_location();

        for _ in 0..9 {
            source_reader.read_next_grapheme_cluster().unwrap();
        }

        let last_location = source_reader.current_location();

        let report = Report::new_from_range(
            Severity::Error,
            ReportCode::ES001UnexpectedCharacter,
            first_location,
            last_location,
            "Unexpected character".to_owned(),
        );

        let mut output = Buffer::no_color();

        render_report(&mut output, &report, 3).unwrap();

        let output = String::from_utf8(output.into_inner()).unwrap();

        assert_eq!(output, "test.frg:1:14: error ES001 - Unexpected character\n    1▕ this is some text\n     ▕              ▔▔▔▔\n    2▕ \n    3▕ some more text\n     ▕ ▔▔▔▔\n\n");
    }

    #[test]
    fn test_suggested_fix() {
        let mut source_context = SourceContext::default();

        let source = source_context
            .add_from_string(
                "test.frg",
                "this is some text\n\nsome more text\nsdkfjhskdf\n\n".to_owned(),
            )
            .unwrap();

        let mut source_reader = SourceReader::new(SourceLocation::new_at_start(source));

        for _ in 0..13 {
            source_reader.read_next_grapheme_cluster().unwrap();
        }

        let first_location = source_reader.current_location();

        for _ in 0..9 {
            source_reader.read_next_grapheme_cluster().unwrap();
        }

        let last_location = source_reader.current_location();

        let mut report = Report::new_from_range(
            Severity::Error,
            ReportCode::ES001UnexpectedCharacter,
            first_location,
            last_location,
            "Unexpected character".to_owned(),
        );

        report.push_child(Severity::SuggestedFix, "Suggested fix".to_owned());

        let mut output = Buffer::no_color();

        render_report(&mut output, &report, 3).unwrap();

        let output = String::from_utf8(output.into_inner()).unwrap();

        assert_eq!(output, "test.frg:1:14: error ES001 - Unexpected character\n    1▕ this is some text\n     ▕              ▔▔▔▔\n    2▕ \n    3▕ some more text\n     ▕ ▔▔▔▔\n\n  suggested fix - Suggested fix\n\n");
    }
}
