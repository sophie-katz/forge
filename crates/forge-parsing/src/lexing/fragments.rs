use unicode_ident::{is_xid_continue, is_xid_start};

use crate::sourcing::ErrorSource;

use super::LexingContext;

/// Read until the ending quote is found.
///
/// If this is the beginning state of the lexer:
///
/// ```plaintext
/// This is a "quoted string".
///            ^
/// ```
///
/// Then this will be the state after the function is called:
///
/// ```plaintext
/// This is a "quoted string".
///                         ^
/// ```
pub fn read_until_ending_quote<TokenValue>(
    lexing_context: &mut LexingContext<TokenValue>,
    quote_grapheme_cluster: &str,
) -> Result<(), ErrorSource> {
    while let Ok(grapheme_cluster) = lexing_context.peek_next_grapheme_cluster() {
        if grapheme_cluster == quote_grapheme_cluster {
            if lexing_context.last_grapheme_cluster() == Some("\\") {
                lexing_context.read_next_grapheme_cluster()?;
            } else {
                lexing_context.read_next_grapheme_cluster()?;
                break;
            }
        } else {
            lexing_context.read_next_grapheme_cluster()?;
        }
    }

    Ok(())
}

/// Read the next number literal. This can be either an integer or a floating-point number.
///
/// If this is the beginning state of the lexer:
///
/// ```plaintext
/// This is a number: 0x53.5fe-5ff0.
///                   ^
/// ```
///
/// Then this will be the state after the function is called:
///
/// ```plaintext
/// This is a number: 0x53.5fe-5ff0.
///                               ^
/// ```
pub fn read_number<TokenValue>(
    lexing_context: &mut LexingContext<TokenValue>,
) -> Result<bool, ErrorSource> {
    let mut found_period = false;
    let mut found_minus = false;

    while let Ok(grapheme_cluster) = lexing_context.peek_next_grapheme_cluster() {
        if is_grapheme_cluster_digit(&grapheme_cluster) || grapheme_cluster == "_" {
            lexing_context.read_next_grapheme_cluster()?;
        } else if grapheme_cluster == "." {
            if found_period {
                break;
            } else {
                lexing_context.read_next_grapheme_cluster()?;
                found_period = true;
            }
        } else if grapheme_cluster == "-" {
            if found_minus {
                break;
            } else if lexing_context.last_grapheme_cluster() == Some("e") {
                lexing_context.read_next_grapheme_cluster()?;
                found_minus = true;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    Ok(found_period)
}

/// Reads grapheme clusters while they match the given condition.
pub fn read_while_matching<TokenValue>(
    lexing_context: &mut LexingContext<TokenValue>,
    grapheme_cluster_condition: impl Fn(&str) -> bool,
) -> Result<(), ErrorSource> {
    while let Ok(grapheme_cluster) = lexing_context.peek_next_grapheme_cluster() {
        if grapheme_cluster_condition(&grapheme_cluster) {
            lexing_context.read_next_grapheme_cluster()?;
        } else {
            break;
        }
    }

    Ok(())
}

/// Checks if the given grapheme cluster is allowed to be the first character of a symbol.
pub fn is_grapheme_cluster_allowed_first_of_symbol(grapheme_cluster: &str) -> bool {
    let mut iterator = grapheme_cluster.chars();

    match iterator.next() {
        Some(first_char) => {
            if !is_xid_start(first_char) {
                return false;
            }
        }
        None => return false,
    };

    for char in iterator {
        if !is_xid_continue(char) {
            return false;
        }
    }

    true
}

/// Checks if the given grapheme cluster is allowed to be part of a symbol.
pub fn is_grapheme_cluster_allowed_in_symbol(grapheme_cluster: &str) -> bool {
    for char in grapheme_cluster.chars() {
        if !is_xid_continue(char) {
            return false;
        }
    }

    true
}

/// Checks if the given grapheme cluster is allowed to be the first character of a number.
pub fn is_grapheme_cluster_allowed_first_of_number(grapheme_cluster: &str) -> bool {
    grapheme_cluster.chars().all(|char| char.is_ascii_digit())
}

fn is_grapheme_cluster_digit(grapheme_cluster: &str) -> bool {
    grapheme_cluster
        .chars()
        .all(|char| char.is_ascii_alphanumeric())
}
