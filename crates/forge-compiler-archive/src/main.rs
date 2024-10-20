#![feature(map_try_insert)]

use std::process;

use cli::args::parse_cli_args;

mod cli;
mod codegen;
mod domain;
mod lexing;
mod message;
mod parsing;
mod source;
mod visitor;

fn main() {
    if !parse_cli_args() {
        process::exit(1);
    }
}
