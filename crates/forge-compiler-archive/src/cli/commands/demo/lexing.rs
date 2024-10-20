use termcolor::{ColorChoice, StandardStream};

use crate::{
    lexing::lexer::lex,
    message::{
        domain::{Message, MessageContext, Severity},
        formatting::write_message_context,
    },
    source::domain::SourceContext,
};

pub struct DemoLexingArgs {
    pub source: String,
}

impl DemoLexingArgs {
    pub fn run(self) -> bool {
        let mut source_context = SourceContext::default();
        let mut message_context = MessageContext::new();

        let source = match source_context.add_from_file(self.source.as_str()) {
            Ok(source) => source,
            Err(error) => {
                message_context.add(Message::new(
                    Severity::FatalError,
                    format!("unable to read file {:?}: {:}", self.source, error),
                ));

                write_message_context(
                    &mut StandardStream::stdout(ColorChoice::Auto),
                    &message_context,
                )
                .unwrap();

                return false;
            }
        };

        let tokens = lex(source, &mut message_context);

        write_message_context(
            &mut StandardStream::stdout(ColorChoice::Auto),
            &message_context,
        )
        .unwrap();

        for token in tokens {
            println!("{}", token);
        }

        true
    }
}
