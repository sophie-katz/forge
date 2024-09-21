use thiserror::Error;

#[derive(Debug, Error)]
pub enum ErrorVisitor {
    #[error("halting visitation down branch")]
    HaltBranch,

    #[error("halting visitation for entire tree")]
    HaltTree,
}

pub trait Visit<Node> {
    fn enter(&mut self, node: &mut Node) -> Result<(), ErrorVisitor>;

    fn leave(&mut self, node: &mut Node) -> Result<(), ErrorVisitor>;
}

pub trait Accept<Visitor>: Sized
where
    Visitor: Visit<Self>,
{
    fn accept_children(&mut self, visitor: &mut Visitor) -> Result<(), ErrorVisitor>;

    fn accept(&mut self, visitor: &mut Visitor) -> Result<(), ErrorVisitor> {
        match visitor.enter(self) {
            // No errors while entering node, continue normally
            Ok(()) => {}
            // Visitation is halted for this branch only, leave early
            Err(ErrorVisitor::HaltBranch) => return Ok(()),
            // Visitation is halted for entire tree, error early
            Err(ErrorVisitor::HaltTree) => return Err(ErrorVisitor::HaltTree),
        };

        match self.accept_children(visitor) {
            // No errors while visiting children, continue normally
            Ok(()) => {}
            // Visitation is halted for this branch only, leave early
            Err(ErrorVisitor::HaltBranch) => return Ok(()),
            // Visitation is halted for entire tree, error early
            Err(ErrorVisitor::HaltTree) => return Err(ErrorVisitor::HaltTree),
        }

        match visitor.enter(self) {
            // No errors while entering node, continue normally
            Ok(()) => {}
            // Visitation is halted for this branch only, leave early
            Err(ErrorVisitor::HaltBranch) => return Ok(()),
            // Visitation is halted for entire tree, error early
            Err(ErrorVisitor::HaltTree) => return Err(ErrorVisitor::HaltTree),
        }
    }
}
