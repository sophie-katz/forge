use super::token::Token;

#[derive(Clone)]
pub struct TokenReaderState {
    current_offset: usize,
}

pub struct TokenReader<'source_context> {
    tokens: Vec<Token<'source_context>>,
    state: TokenReaderState,
}

impl<'source_context> TokenReader<'source_context> {
    pub fn new(tokens: Vec<Token<'source_context>>) -> Self {
        Self {
            tokens,
            state: TokenReaderState { current_offset: 0 },
        }
    }

    pub fn is_more(&self) -> bool {
        self.state.current_offset < self.tokens.len()
    }

    pub fn peek_next(&self) -> Option<&Token<'source_context>> {
        self.tokens.get(self.state.current_offset)
    }

    pub fn read_next(&mut self) -> Option<&Token<'source_context>> {
        // Need to use this instead of peek_next because of lifetimes
        let token = self.tokens.get(self.state.current_offset);

        if token.is_some() {
            self.state.current_offset += 1;
        }

        token
    }

    pub fn save_state(&self) -> TokenReaderState {
        self.state.clone()
    }

    pub fn restore_state(&mut self, state: TokenReaderState) {
        self.state = state;
    }
}
