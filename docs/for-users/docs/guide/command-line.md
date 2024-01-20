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

# Forge command line

Forge is accessed on the command line with the `forge` command. Run this to get help for the command:

```shell
forge help
```

## Environment variables

Forge uses the following environment variables:

| Variable             | Description                                                                                                                        |
| -------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| `FORGE_DEBUG`        | If set to `true` enables debug output. If set to `false` disables debug output (this is the default).                              |
| `FORGE_COLOR_MODE`   | Can be set to `enabled`, `disabled`, or `auto` to enable, disable, or automatically detect color support. Defaults to `auto`.      |
| `FORGE_UNICODE_MODE` | Can be set to `enabled`, `disabled`, or `auto` to enable, disable, or automatically detect unicode support. Defaults to `auto`.    |
