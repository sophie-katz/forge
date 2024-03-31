<!--
This program is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this
program. If not, see <https://www.gnu.org/licenses/>.

Copyright 2023-2024 Sophie Katz
-->

# How to use the Forge specification

This is intended to be the source of truth for how the Forge language is supposed to behave. It is not intended to be a tutorial, but rather a reference for how the language works.

It contains the following sections:

- [Requirements](#requirements)
- [Proposals](#proposals)
- [Templates](#templates)

There should be a clear distinction between what is a requirement and what is a proposal:

- _Requirements_ say "this is how the language is defined." They should be explicit and should be written from the perspective of someone implementing a compiler for the language.
- _Proposals_ say "this is how the language could be changed." Ideally, when a proposal is accepted it should cause an update to the requirements and to the compiler.

## Requirements

These are parts of the language specification. One requirement is supposed to define a single language feature or a behavior for how language features fit together. They use the `Requirement-v*` templates.

Some helpful tools are _(tools valid as of 2024-03-31)_:

- [regex101](https://regex101.com/) for testing regular expressions.
- [EBNF Evaluator](https://mdkrajnak.github.io/ebnftest/) for testing EBNF notation.

### EBNF formatting

EBNF rules should follow snake case naming convention. Rules should be indented like this:

```ebnf
rule_name ::= ...
            | ...
            ;
```

## Proposals

These are proposals for how the language could be changed or improved. They use the `Proposal-v*` templates.

Proposal status can be any of:

- **Draft**: The proposal is still being written and is not ready for review.

- **In review**: The proposal is being reviewed and is not ready for implementation.

- **Accepted**: The proposal has been accepted and is ready for implementation.

- **Rejected**: The proposal has been rejected and will not be implemented.

- **In progress**: The proposal has been accepted and is being implemented.

- **Implemented**: The proposal has been implemented.

- **Withdrawn**: The proposal has been withdrawn and will not be implemented.

- **Deferred**: The proposal has been deferred and may be re-reviewed at a later date.

- **Superseded**: The proposal has been superseded by another proposal and will not be implemented.

## Subitems

Requirements and proposals both have subitems, which are parts of the proposal which can be implemented independently. There is a section at the top of the template for listing subitems.

In the requirement or proposal, subitems should be referenced like `(subitem 1)`. Outside of the document, subitems should be referenced like `Requirement-2.1`.

## Templates

These are versioned templates for how different parts of the specification should be written. When adding a new document always copy from a template and fill in the suggested sections.

- [`Proposal-v0.md`](https://github.com/sophie-katz/forge/blob/main/docs/for-users/docs/specification/templates/Proposal-v0.md)
- [`Requirement-v0.md`](https://github.com/sophie-katz/forge/blob/main/docs/for-users/docs/specification/templates/Requirement-v0.md)
