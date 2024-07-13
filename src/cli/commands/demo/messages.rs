use indoc::indoc;
use termcolor::{ColorChoice, StandardStream};

use crate::{
    message::{
        domain::{Message, MessageContext, Severity},
        formating::write_message_context,
    },
    source::domain::SourceContext,
};

pub struct DemoMessagesArgs {}

impl DemoMessagesArgs {
    pub fn run(self) -> bool {
        let source_text = indoc! {r#"
            import std.io.*;

            fn main() {
                print("hello, world\n");
            }
        "#}
        .trim();

        let mut source_context = SourceContext::new();

        let source = source_context
            .add_from_str("test.frg".to_owned(), source_text)
            .unwrap();

        let mut message_context = MessageContext::new();

        message_context.add(Message::new(
            Severity::Warning,
            "this is a global warning message".to_owned(),
        ));

        message_context.add(Message::new_from_source(
            Severity::Error,
            source,
            "this is a file error message".to_owned(),
        ));

        message_context.add(Message::new_from_line(
            Severity::Error,
            source,
            1,
            "this is a line error message".to_owned(),
        ));

        message_context.add(Message::new_from_location_raw(
            Severity::Error,
            source,
            3,
            4,
            "this is a location error message".to_owned(),
        ));

        message_context.add(Message::new_from_range_raw(
            Severity::Error,
            source,
            3,
            4,
            4,
            9,
            "this is a range error message".to_owned(),
        ));

        println!("{}", source_text);
        println!();

        write_message_context(
            &mut StandardStream::stdout(ColorChoice::Auto),
            &message_context,
        )
        .unwrap();

        true
    }
}
