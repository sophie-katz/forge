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

# Forge compiler

This is the reference implementation of Forge. It follows the specification in the [user documentation](../docs/for-users/).

- [System setup](#system-setup)
- [Building](#building)
- [Generating code coverage](#generating-code-coverage)
- [Formatting](#formatting)
- [Linting](#linting)
- [API documentation &#x2197;](https://sophie-katz.github.io/forge/compiler-api/index.html)

## Building

Follow the [Getting Started](../docs/for-developers//guides//getting-started.md) guide first before building.

```shell
# Build the compiler
cd forge-compiler
meson setup build
meson compile -C build

# Run tests
meson test -C build --print-errorlogs -q

# Run Forge
./build/forge --help
```

## Generating code coverage

```shell
# When switching between code coverage modes, always delete the build directory
rm -rf build
meson setup build

# For everything
meson configure build -Dbuildtype=debug -Db_coverage=true -Dtest_mode=fast -Dtest_only_changed=false -Dshould_build_integration_tests=true -Dshould_build_demos=true && \
    meson compile -C build && \
    meson test -C build && \
    ninja coverage-html -C build

# For only unit tests (can be helpful for improving unit tests specifically)
meson configure build -Dbuildtype=debug -Db_coverage=true -Dtest_mode=fast -Dtest_only_changed=false -Dshould_build_integration_tests=false -Dshould_build_demos=false && \
    meson compile -C build && \
    meson test -C build && \
    ninja coverage-html -C build
```

## Formatting

```shell
ninja clang-format -C build
```

## Linting

```shell
ninja lint -C build
```

## API documentation

```shell
ninja doxygen -C build
```
