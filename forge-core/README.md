<!--
Copyright (c) 2023 Sophie Katz

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

## Getting started

To build the Forge compiler you need these installed on your system:

- [Meson](https://mesonbuild.com/)
- [CMake](https://cmake.org/)
- [Glib 2.0](https://docs.gtk.org/glib/)
- [Doxygen](https://www.doxygen.nl/index.html) (only needed for building API documentation)

Once these are all installed, run:

```shell
# Clone this repository
git clone https://github.com/sophie-katz/forge.git

# Init submodules
cd forge
git submodule update --init

# Build the Core
cd forge-core
meson setup build
meson compile -C build

# Run tests
meson test -C build

# Run the compiler
./build/forge-compiler --help

# Run the interpreter
./build/forge-interpreter --help

# Build the documentation (to build/doxygen)
doxygen
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
