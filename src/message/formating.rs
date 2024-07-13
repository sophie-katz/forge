use std::io;

use crate::{
    cli::unicode::choose_unicode,
    source::{
        domain::{ColumnNumber, LineNumber, SourceRef},
        reader::SourceReader,
    },
};

use super::domain::{Message, MessageContext, Severity};
use termcolor::{Color, ColorSpec, WriteColor};

fn write_message(
    writer: &mut impl WriteColor,
    line_number_width: ColumnNumber,
    message: &Message,
) -> io::Result<()> {
    write_message_location(writer, message)?;

    write_message_severity(writer, message.severity())?;

    write_message_text(writer, message.text())?;

    write_message_source(writer, line_number_width, message)?;

    writeln!(writer)?;

    Ok(())
}

fn write_message_location(writer: &mut impl WriteColor, message: &Message) -> io::Result<()> {
    if let Some(source) = message.source() {
        writer.set_color(ColorSpec::new().set_fg(Some(Color::Cyan)))?;

        write!(writer, "{}", source.path())?;

        if let Some(line) = message.first_location_line_number() {
            write!(writer, ":{}", line)?;

            if let Some(column) = message.first_location_column_number() {
                write!(writer, ":{}", column)?;
            }
        }

        writer.set_color(ColorSpec::new().set_dimmed(true))?;

        write!(writer, " - ")?;
    }

    Ok(())
}

fn write_message_severity(writer: &mut impl WriteColor, severity: Severity) -> io::Result<()> {
    match severity {
        Severity::FatalError => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Red)).set_bold(true))?;
            write!(writer, "fatal error:")?;
            writer.set_color(&ColorSpec::new())?;
        }
        Severity::Error => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Red)).set_bold(true))?;
            write!(writer, "error:")?;
            writer.set_color(&ColorSpec::new())?;
        }
        Severity::Warning => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Yellow)).set_bold(true))?;
            write!(writer, "warning:")?;
            writer.set_color(&ColorSpec::new())?;
        }
        Severity::Info => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Cyan)).set_bold(true))?;
            write!(writer, "info:")?;
            writer.set_color(&ColorSpec::new())?;
        }
        Severity::Fix => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Green)).set_bold(true))?;
            write!(writer, "fix:")?;
            writer.set_color(&ColorSpec::new())?;
        }
        Severity::Suggestion => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Blue)).set_bold(true))?;
            write!(writer, "suggestion:")?;
            writer.set_color(&ColorSpec::new())?;
        }
        Severity::Note => {
            writer.set_color(ColorSpec::new().set_dimmed(true).set_bold(true))?;
            write!(writer, "note:")?;
            writer.set_color(&ColorSpec::new())?;
        }
        Severity::Debug => {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Magenta)).set_bold(true))?;
            write!(writer, "debug:")?;
            writer.set_color(&ColorSpec::new())?;
        }
    }

    Ok(())
}

fn write_message_source(
    writer: &mut impl WriteColor,
    line_number_width: ColumnNumber,
    message: &Message,
) -> io::Result<()> {
    if let Some(source) = message.source() {
        if let Some((first_line_number, last_line_number)) = message.line_number_range() {
            let mut line_number = first_line_number;

            while line_number <= last_line_number {
                let first_column_number = if line_number == first_line_number {
                    message.first_location_column_number()
                } else {
                    None
                };

                let last_column_number = if line_number == last_line_number {
                    message.last_location_column_number()
                } else {
                    None
                };

                write_message_source_line(
                    writer,
                    source,
                    line_number_width,
                    line_number,
                    first_column_number,
                    last_column_number,
                    false,
                )?;

                write_message_source_line(
                    writer,
                    source,
                    line_number_width,
                    line_number,
                    first_column_number,
                    last_column_number,
                    true,
                )?;

                line_number += 1;
            }
        }
    }
    Ok(())
}

fn write_message_source_line(
    writer: &mut impl WriteColor,
    source: SourceRef,
    line_number_width: ColumnNumber,
    line_number: LineNumber,
    first_column_number: Option<ColumnNumber>,
    last_column_number: Option<ColumnNumber>,
    underline: bool,
) -> io::Result<()> {
    writer.set_color(ColorSpec::new().set_dimmed(true))?;

    if underline {
        for _ in 0..line_number_width {
            write!(writer, " ")?;
        }

        write!(writer, "{}  ", choose_unicode("\u{2595}", " |"))?;
    } else {
        write!(writer, "{: >1$}", line_number, line_number_width as usize,)?;

        write!(writer, "{}  ", choose_unicode("\u{2595}", " |"))?;
    }

    writer.set_color(&ColorSpec::new())?;

    let first_location_of_line = match source.get_first_location_of_line(line_number) {
        Ok(Some(first_location_of_line)) => first_location_of_line,
        _ => return Ok(()),
    };

    let mut reader = SourceReader::new(first_location_of_line);

    while let Some(byte) = reader.peek_next() {
        if byte == b'\n' {
            break;
        }

        if first_column_number.map_or(true, |first_column_number| {
            reader.current_location().column_number >= first_column_number
        }) && last_column_number.map_or(true, |last_column_number| {
            reader.current_location().column_number <= last_column_number
        }) {
            writer.set_color(ColorSpec::new().set_fg(Some(Color::Green)))?;

            if underline {
                write!(writer, "{}", choose_unicode("\u{2594}", "^"))?;
            }
        } else {
            writer.set_color(&ColorSpec::new())?;

            if underline {
                write!(writer, " ")?;
            }
        }

        if !underline {
            writer.write_all(&[byte])?;
        }

        writer.set_color(&ColorSpec::new())?;

        reader.read_next();
    }

    writeln!(writer)?;

    Ok(())
}

fn write_message_text(writer: &mut impl WriteColor, text: &str) -> io::Result<()> {
    writer.set_color(ColorSpec::new().set_bold(true))?;

    write!(writer, " {}", text)?;

    writer.set_color(&ColorSpec::new())?;

    writeln!(writer)?;

    Ok(())
}

pub fn write_message_context(
    writer: &mut impl WriteColor,
    message_context: &MessageContext,
) -> io::Result<()> {
    let line_number_width = message_context.max_line_number().map_or(0, |line_number| {
        line_number.to_string().len() as ColumnNumber
    });

    for message in message_context.iter_messages() {
        write_message(writer, line_number_width, message)?;
    }

    if message_context.error_count() > 0 && message_context.warning_count() == 0 {
        writer.set_color(ColorSpec::new().set_fg(Some(Color::Red)).set_bold(true))?;

        writeln!(
            writer,
            "failed with {} error{}",
            message_context.error_count(),
            if message_context.error_count() == 1 {
                ""
            } else {
                "s"
            }
        )?;

        writer.set_color(&ColorSpec::new())?;
    } else if message_context.error_count() == 0 && message_context.warning_count() > 0 {
        writer.set_color(ColorSpec::new().set_fg(Some(Color::Yellow)).set_bold(true))?;

        writeln!(
            writer,
            "succeeded with {} warning{}",
            message_context.warning_count(),
            if message_context.warning_count() == 1 {
                ""
            } else {
                "s"
            }
        )?;

        writer.set_color(&ColorSpec::new())?;
    } else if message_context.error_count() > 0 && message_context.warning_count() > 0 {
        writer.set_color(ColorSpec::new().set_fg(Some(Color::Red)).set_bold(true))?;

        write!(
            writer,
            "failed with {} error{}",
            message_context.error_count(),
            if message_context.error_count() == 1 {
                ""
            } else {
                "s"
            }
        )?;

        writer.set_color(ColorSpec::new().set_fg(Some(Color::Yellow)).set_bold(true))?;

        writeln!(
            writer,
            " and {} warning{}",
            message_context.warning_count(),
            if message_context.warning_count() == 1 {
                ""
            } else {
                "s"
            }
        )?;

        writer.set_color(&ColorSpec::new())?;
    }

    Ok(())
}
