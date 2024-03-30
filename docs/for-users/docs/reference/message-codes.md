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

# Message codes

The compiler generates a number of different messages, all of which can be identified by these codes. They are all of the form:

```
<severity><domain><source><number>
```

## Severity

`<severity>` can be any of the following:

- `W`: Warnings
- `E`: Errors
- `F`: Fatal errors
- `I`: Internal errors

Notes or debug messages intentionally do not have codes.

## Domain

`<domain>` can be any of the following:

- `C`: Command line errors
- `FS`: Filesystem errors that may involve being unable to read or write files
- `FT`: Feature support errors
- `G`: Code generation errors
- `L`: Linker errors
- `S`: Syntax errors
- `T`: Type errors

Messages from `FS` and `C` domains are displayed without error codes.

## Source

`<source>` helps identify if any plugins are responsible for the message. They are defined by the plugin but any messages emitted by the Forge compiler itself have `-` as the source.

## Index

| Code   | Description | Example |
| ------ | ----------- | ------- |
| `FC-1` |             |         |
