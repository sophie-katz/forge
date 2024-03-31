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

# Updating dependency versions

These are all the places where dependency versions are defined. To keep Forge up to date, go through these and update as necessary:

- `.vscode/settings.json`
  - Update `C_Cpp.default.includePath`
- `docker/Dockerfile.development`
- `forge-core/meson.build`
- `forge-core/subprojects/*`
  - Update submodule commits
- `forge-documentation-template/package.json`
- `forge-vscode-extension/package.json`
- `requirements.txt`
