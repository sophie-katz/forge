use forge_parsing::{
    lexing::{
        is_grapheme_cluster_allowed_first_of_number, is_grapheme_cluster_allowed_first_of_symbol,
        is_grapheme_cluster_allowed_in_symbol, read_number, read_until_ending_quote,
        read_while_matching, LexingContext, Token,
    },
    sourcing::{ErrorSource, SourceLocation, SourceReader, SourceRef},
};
use forge_reporting::{Report, ReportCode, ReportingContext, Severity};

use super::token_value::TokenValue;

#[allow(dead_code)]
pub fn lex<'source_context>(
    reporting_context: &mut ReportingContext<'source_context>,
    source: SourceRef<'source_context>,
) -> Result<Vec<Token<'source_context, TokenValue>>, ErrorSource> {
    lex_from(reporting_context, SourceLocation::new_at_start(source))
}

pub fn lex_from<'source_context>(
    reporting_context: &mut ReportingContext<'source_context>,
    start: SourceLocation<'source_context>,
) -> Result<Vec<Token<'source_context, TokenValue>>, ErrorSource> {
    let mut reader = SourceReader::new(start);

    let mut tokens = Vec::new();

    let mut lexing_context = LexingContext::<TokenValue>::new(&mut reader, &mut tokens);

    while lexing_context.is_more() {
        lex_next_token(reporting_context, &mut lexing_context)?;
    }

    Ok(tokens)
}

fn lex_next_token<'source_context>(
    reporting_context: &mut ReportingContext<'source_context>,
    lexing_context: &mut LexingContext<'source_context, '_, '_, TokenValue>,
) -> Result<(), ErrorSource> {
    match lexing_context.peek_next_grapheme_cluster()?.as_str() {
        symbol_start if is_grapheme_cluster_allowed_first_of_symbol(symbol_start) => {
            lexing_context.read_next_grapheme_cluster()?;

            read_while_matching(lexing_context, is_grapheme_cluster_allowed_in_symbol)?;

            lexing_context.pop_token(TokenValue::Symbol);
        }
        number_start if is_grapheme_cluster_allowed_first_of_number(number_start) => {
            lexing_context.read_next_grapheme_cluster()?;

            let contains_period = read_number(lexing_context)?;

            lexing_context.pop_token(if contains_period {
                TokenValue::LiteralInteger
            } else {
                TokenValue::LiteralFloat
            });
        }
        "'" => {
            lexing_context.read_next_grapheme_cluster()?;

            read_until_ending_quote(lexing_context, "'")?;

            match lexing_context.peek_next_grapheme_cluster() {
                Ok(grapheme_cluster) => {
                    if grapheme_cluster == "'" {
                        lexing_context.read_next_grapheme_cluster()?;
                    } else {
                        reporting_context.push(Report::new_from_location(
                            Severity::Error,
                            ReportCode::ES002NoClosingQuoteForStringOrCharacter,
                            lexing_context.current_location(),
                            "Character that starts with opening ' requires closing '".to_string(),
                        ));
                    }
                }
                Err(_) => {
                    reporting_context.push(Report::new_from_location(
                        Severity::Error,
                        ReportCode::ES002NoClosingQuoteForStringOrCharacter,
                        lexing_context.current_location(),
                        "Character that starts with opening ' requires closing '".to_string(),
                    ));
                }
            };

            lexing_context.pop_token(TokenValue::LiteralCharacter);
        }
        "\"" => {
            lexing_context.read_next_grapheme_cluster()?;

            read_until_ending_quote(lexing_context, "\"")?;

            match lexing_context.peek_next_grapheme_cluster() {
                Ok(grapheme_cluster) => {
                    if grapheme_cluster == "\"" {
                        lexing_context.read_next_grapheme_cluster()?;
                    } else {
                        reporting_context.push(Report::new_from_location(
                            Severity::Error,
                            ReportCode::ES002NoClosingQuoteForStringOrCharacter,
                            lexing_context.current_location(),
                            "String that starts with opening \" requires closing \"".to_string(),
                        ));
                    }
                }
                Err(_) => {
                    reporting_context.push(Report::new_from_location(
                        Severity::Error,
                        ReportCode::ES002NoClosingQuoteForStringOrCharacter,
                        lexing_context.current_location(),
                        "String that starts with opening '\"' requires closing '\"'".to_string(),
                    ));
                }
            };

            lexing_context.pop_token(TokenValue::LiteralString);
        }
        " " | "\t" | "\n" => {
            lexing_context.read_next_grapheme_cluster()?;

            lexing_context.skip_token();
        }
        unexpected => {
            reporting_context.push(Report::new_from_location(
                Severity::Error,
                ReportCode::ES001UnexpectedCharacter,
                lexing_context.current_location(),
                format!("Unexpected character {:?}", unexpected),
            ));

            lexing_context.read_next_grapheme_cluster()?;

            lexing_context.skip_token();
        }
    }

    Ok(())
}
