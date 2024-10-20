use lazy_static::lazy_static;
use std::env;

lazy_static! {
    static ref ENABLE_UNICODE: bool = match env::var("FORGE_UNICODE") {
        Ok(value) => matches!(value.as_str(), "true" | "1"),
        // If unset, default to true
        Err(_) => true,
    };
}

pub fn choose_unicode<'a>(unicode: &'a str, ascii: &'a str) -> &'a str {
    if *ENABLE_UNICODE {
        unicode
    } else {
        ascii
    }
}
