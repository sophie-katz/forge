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

"""
Usage: python3 check_subprojects.py

Checks to make sure all Meson subprojects are present. If not, it throws an error
instructing the developer to update Git submodules.
"""

import sys
from common import *

SUBPROJECTS = [
    "doxygen-awesome-css",
    "Unity",
    "utf8proc",
]

def does_subproject_exist(name: str) -> bool:
    subproject_directory = get_source_directory() / "subprojects" / name
    return subproject_directory.exists() and (subproject_directory / ".git").exists()

for subproject_name in SUBPROJECTS:
    if not does_subproject_exist(subproject_name):
        sys.stderr.write("Subprojects are missing. Run `git submodule update --init --recursive` to fetch them.\n")
        sys.stderr.flush()
        sys.exit(1)
