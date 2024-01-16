<!--
Copyright (c) 2023-2024 Sophie Katz

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

# Error codes

The compiler generates a number of different errors, all of which can be identified by these codes. They are all of the form:

```
<severity><domain><source><number>
```

## Severity

`<severity>` can be any of the following:

- `W`: Warnings
- `E`: Errors
- `F`: Fatal errors
- `I`: Internal errors

## Domain

`<domain>` can be any of the following:

- `C`: Command line errors
- `F`: Filesystem errors that may involve being unable to read or write files
- `S`: Syntax errors
- `T`: Type errors
- `G`: Code generation errors

Note that errors from `F` and `C` domains are displayed without error codes.

## Source

`<source>` helps identify if any plugins are responsible for the message. They are defined by the plugin but any messages emitted by the Forge compiler itself have `-` as the source.

## Index

| Code   | Description | Example |
| ------ | ----------- | ------- |
| `FC-1` |             |         |
