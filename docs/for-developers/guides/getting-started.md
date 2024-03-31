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

# Getting started

These are instructions for how to get started working with this repository and all the projects in it.

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

## Clone this repository

```shell
# Clone this repository
git clone --recurse-submodules https://github.com/sophie-katz/forge.git
```

## Instal Python dependencies

```shell
python3 -m venv .venv
. .venv/bin/activate
pip3 install -r requirements.txt
```
