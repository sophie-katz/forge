use super::commands::demo::{lexing::DemoLexingArgs, messages::DemoMessagesArgs};
use clap::{command, Arg, Command};

pub fn parse_cli_args() -> bool {
    let matches = command!()
        .subcommand_required(true)
        .subcommand(
            Command::new("demo")
                .subcommand_required(true)
                .subcommand(Command::new("messages"))
                .subcommand(
                    Command::new("lexing").arg(
                        Arg::new("source")
                            .help("The path of the source file to lex")
                            .required(true),
                    ),
                ),
        )
        .get_matches();

    if let Some(matches) = matches.subcommand_matches("demo") {
        if let Some(_) = matches.subcommand_matches("messages") {
            return DemoMessagesArgs {}.run();
        } else if let Some(args) = matches.subcommand_matches("lexing") {
            return DemoLexingArgs {
                source: args.get_one::<String>("source").unwrap().clone(),
            }
            .run();
        }
    }

    true
}
