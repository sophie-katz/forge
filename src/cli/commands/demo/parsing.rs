use termcolor::{ColorChoice, StandardStream};

use crate::{
    lexing::{lexer::lex, reader::TokenReader},
    message::{
        domain::{Message, MessageContext, Severity},
        formatting::write_message_context,
    },
    parsing::parser::parse,
    source::domain::SourceContext,
};

pub struct DemoParsingArgs {
    pub source: String,
}

impl DemoParsingArgs {
    pub fn run(self) -> bool {
        let mut source_context = SourceContext::new();
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

        let mut token_reader = TokenReader::new(tokens);

        let declarations = parse(&mut token_reader, &mut message_context);

        write_message_context(
            &mut StandardStream::stdout(ColorChoice::Auto),
            &message_context,
        )
        .unwrap();

        if let Some(declarations) = declarations {
            for declaration in declarations {
                println!("{:#?}", declaration);
            }

            true
        } else {
            false
        }
    }
}
