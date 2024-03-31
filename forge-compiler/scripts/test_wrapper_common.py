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

"""Common utilities for the test wrapper scripts."""

import sys
import pathlib
import hashlib
import os
from common import *

def should_skip_unchanged_tests() -> bool:
    """Checks if test skipping should be done."""
    return os.environ.get("FORGE_SKIP_UNCHANGED_TESTS", "0") == "1"

def get_test_path() -> pathlib.Path:
    """Get the path of the test command from the command line."""
    test_path = pathlib.Path(sys.argv[1])

    if not test_path.exists():
        raise RuntimeError("Test command must be a direct filesystem path")
    
    return test_path.resolve()

def get_hash_path(test_path: pathlib.Path) -> pathlib.Path:
    """Get the path of the test hash file."""
    return test_path.with_suffix(".hash")

def get_hash(test_path: pathlib.Path) -> str:
    """Creates a hash of a file."""
    with test_path.open("rb", buffering=0) as file:
        return hashlib.file_digest(file, "sha256").hexdigest()

def check_test_matches_hash(test_path: pathlib.Path, hash_path: pathlib.Path) -> bool:
    """
    Checks if the test matches its last successful hash. This is possible because the
    tests are all statically linked.
    
    Returns:
        ``True`` if the test matches the hash and can safely be skipped, ``False`` otherwise.
    """
    actual_hash = get_hash(test_path)

    if hash_path.exists():
        with hash_path.open("r", encoding="ascii") as file:
            expected_hash = file.read().strip()

        return actual_hash == expected_hash
    else:
        return False

def update_test_hash(test_path: pathlib.Path, hash_path: pathlib.Path) -> bool:
    """Updates the hash file for a given test."""
    actual_hash = get_hash(test_path)

    with hash_path.open("w", encoding="ascii") as file:
        file.write(actual_hash)
