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
Usage: python3 lint.py <pkg-config path> <llvm-config path> <clang-tidy path>

Runs ``clang-tidy`` on all source files.
"""

import pathlib
import subprocess
import sys
import os
from common import *

SOURCE_ROOTS = ["include", "lib", "src", "tests"]

pkg_config_path = sys.argv[1]
llvm_config_path = sys.argv[2]
clang_tidy_path = sys.argv[3]

def enumerate_source_files() -> list[pathlib.Path]:
    files = []

    for root in SOURCE_ROOTS:
        for dirpath, dirnames, filenames in os.walk(get_source_directory() / root):
            for filename in filenames:
                if filename.endswith('.h') or filename.endswith('.hpp') or filename.endswith('.c') or filename.endswith('.cpp'):
                    files.append(pathlib.Path(dirpath) / filename)

    return files

def get_cflags_from_pkg_config() -> list[str]:
    return subprocess.run(
            [
                pkg_config_path,
                "--cflags",
                "glib-2.0",
            ],
            check=True,
            capture_output=True,
            text=True,
        ).stdout.strip().split(" ")

def get_cflags_from_llvm_config() -> list[str]:
    return subprocess.run(
            [
                llvm_config_path,
                "--cflags",
            ],
            check=True,
            capture_output=True,
            text=True,
        ).stdout.strip().split(" ")

def get_cflags() -> list[str]:
    return get_cflags_from_pkg_config() + get_cflags_from_llvm_config()

sys.exit(
    subprocess.run(
        [
            clang_tidy_path,
            "--warnings-as-errors",
            f"--config-file={get_source_directory() / '.clang-tidy'}",
            *enumerate_source_files(),
            "--",
            f"-I{get_source_directory() / 'include'}",
            f"-I{get_source_directory() / 'build'}",
            f"-I{get_source_directory() / 'subprojects' / 'utf8proc'}",
            f"-I{get_source_directory() / 'subprojects' / 'Unity' / 'src'}",
            *get_cflags(),
        ],
    ).returncode
)
