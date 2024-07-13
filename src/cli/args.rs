use super::commands::demo::messages::DemoMessagesArgs;
use clap::{command, Command};

pub fn parse_cli_args() -> bool {
    let matches = command!()
        .subcommand_required(true)
        .subcommand(
            Command::new("demo")
                .subcommand_required(true)
                .subcommand(Command::new("messages")),
        )
        .get_matches();

    if let Some(matches) = matches.subcommand_matches("demo") {
        if let Some(_) = matches.subcommand_matches("messages") {
            return DemoMessagesArgs {}.run();
        }
    }

    true
}
