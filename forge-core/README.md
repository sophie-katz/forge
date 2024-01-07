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

# Forge compiler

This is the reference implementation of Forge. It follows the specification in the [user documentation](../docs/for-users/).

- [Getting started](#getting-started)
- [**API documentation**](https://sophie-katz.github.io/forge/compiler-api/index.html)

## System setup

### VS Code dockerized development environment (recommended)

Follow [these instructions](../docs/for-developers/guides/vscode-dockerized-development-environment.md) to get the Dockerized development environment up and running in VS Code. This will automatically have all the dependencies installed.

### Manual setup

To build the Forge compiler you need these installed on your system:

- [Python 3](https://www.python.org/)
- [CMake](https://cmake.org/)
- [Glib 2.0](https://docs.gtk.org/glib/)
- [LLVM](https://llvm.org/)
    - [Debian/Ubuntu packages](https://apt.llvm.org/)
    - [macOS package](https://formulae.brew.sh/formula/llvm)
- [Doxygen](https://www.doxygen.nl/index.html) (only needed for building API documentation)

## Building

```shell
# Clone this repository
git clone https://github.com/sophie-katz/forge.git

# Init submodules
cd forge
git submodule update --init

# Install Python dependencies
python3 -m venv .venv
. .venv/bin/activate
pip3 install -r requirements.txt

# Build the Core
cd forge-core
meson setup build
meson compile -C build

# Run tests
meson test -C build

# Run Forge
./build/forge --help
```

## Building in watch mode

You will need to install [`fswatch`](https://github.com/emcrisostomo/fswatch) to do this.

Run this command to watch the `forge-core` directory for changes and rebuild the compiler when changes are detected:

```shell
fswatch -o . | xargs -n1 -I{} meson compile -C build
```

You can also run this to watch a dump of the AST:

```shell
fswatch -o .. | xargs -n1 -I{} bash -c "clear && ./build/forge dump-ast ../example.frg"
```

## Generating code coverage

> [!WARNING]  
> Code coverage isn't really configured correctly yet. It's not correct and is thus basically useless.

Run these commands:

```shell
meson test -C build
ninja coverage -C build
```
