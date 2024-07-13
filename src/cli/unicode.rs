use lazy_static::lazy_static;
use std::env;

lazy_static! {
    static ref ENABLE_UNICODE: bool = match env::var("FORGE_UNICODE") {
        Ok(value) => match value.as_str() {
            "true" => true,
            "1" => true,
            _ => false,
        },
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
