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

import sys
import subprocess
from test_wrapper_common import *

# Get paths
test_path = get_test_path()
hash_path = get_hash_path(test_path)

# Check if we can skip running the test (if the binary hasn't changed since the last
# successful run)
if should_skip_unchanged_tests() and check_test_matches_hash(test_path, hash_path):
    print("Skipping test because it matches hash of last successful run")
    sys.exit(0)

result = subprocess.run(
    test_path,
    cwd=test_path.resolve().parent
)

# Update the hash if the test was successful
if result.returncode == 0:
    update_test_hash(test_path, hash_path)

sys.exit(result.returncode)
