use std::process;

use cli::args::parse_cli_args;

mod cli;
mod lexing;
mod message;
mod source;

fn main() {
    if !parse_cli_args() {
        process::exit(1);
    }
}
