# This program is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this
# program. If not, see <https://www.gnu.org/licenses/>.
#
# Copyright 2023-2024 Sophie Katz

"""Common utilities for all scripts."""

import pathlib

def get_source_directory() -> pathlib.Path:
    result = pathlib.Path(__file__).parent.parent

    assert (result / "meson.build").exists()

    return result

