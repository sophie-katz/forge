<!--
Copyright (c) 2024 Sophie Katz

This file is part of Forge.

Forge is free software: you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation, either version
3 of the License, or (at your option) any later version.

Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Forge. If
not, see <https://www.gnu.org/licenses/>.
-->

# How to use the Forge specification

This is intended to be the source of truth for how the Forge language is supposed to behave. It is not intended to be a tutorial, but rather a reference for how the language works.

It contains the following sections:

- [Requirements](#requirements)
- [Templates](#templates)

## Requirements

These are parts of the language specification. One requirement is supposed to define a single language feature or a behavior for how language features fit together. They use the `requirement-v*` templates.

Some helpful tools are *(tools valid as of 2023-12-24)*:
- [regex101](https://regex101.com/) for testing regular expressions.
- [EBNF Evaluator](https://mdkrajnak.github.io/ebnftest/) for testing EBNF notation.

### EBNF naming

EBNF rules should follow snake case naming convention. Rules should be indented like this:

```ebnf
rule_name ::= ...
            | ...
            ;
```

## Templates

These are versioned templates for how different parts of the specification should be written. When adding a new document always copy from a template and fill in the suggested sections.
