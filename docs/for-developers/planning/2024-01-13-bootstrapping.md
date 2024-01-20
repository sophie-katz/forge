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

# Bootstrapping

This is heavily based on https://rustc-dev-guide.rust-lang.org/building/bootstrapping.html.

Ideally, we would want Forge to have at least these components:

- Compiler
- Standard library
- Build tools

If we want to create a new release for version Y. This is how we would bootstrap these components from existing version X:

1. Download compiler X
2. Download standard library X
3. Download build tools X
4. Use compiler X, standard library X, and build tools X to build dirty compiler Y
5. Use dirty compiler Y to build dirty standard library Y
6. Use dirty compiler Y and dirty standard library Y to build dirty build tools Y
7. Use dirty compiler Y, dirty standard library Y, and dirty build tools Y to build compiler Y
8. Use compiler Y to build standard library Y
9. Use compiler Y and standard library Y to build build tools Y
