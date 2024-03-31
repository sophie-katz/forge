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
import pathlib
import re
import uuid
from test_wrapper_common import *

# Get paths
script_path = pathlib.Path(__file__).resolve()
test_path = get_test_path()
hash_path = get_hash_path(test_path)
valgrind_log_path = pathlib.Path(f"valgrind-{uuid.uuid4()}.log")
glib2_valgrind_suppression_file = os.environ.get("FORGE_GLIB2_VALGRIND_SUPPRESSION_FILE")

if glib2_valgrind_suppression_file is None:
    raise RuntimeError("Environment variable FORGE_GLIB2_VALGRIND_SUPPRESSION_FILE must be set")

# Check if we can skip running the test (if the binary hasn't changed since the last
# successful run)
if should_skip_unchanged_tests() and check_test_matches_hash(test_path, hash_path):
    print("Skipping test because it matches hash of last successful run")
    sys.exit(0)

# Run test as normal
result = subprocess.run(
    sys.argv[1:],
    cwd=test_path.resolve().parent,
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
)

if result.returncode != 0:
    sys.stdout.buffer.write(result.stdout)
    sys.stdout.flush()
    sys.exit(result.returncode)

# Run test again with Valgrind
valgrind_log_path.unlink(missing_ok=True)
returncode = subprocess.run(
    [
        "valgrind",
        "--tool=memcheck",
        f"--suppressions={glib2_valgrind_suppression_file}",
        f"--suppressions={script_path.parent.parent}/forge.supp",
        "--leak-check=full",
        "--show-leak-kinds=all",
        f"--log-file={valgrind_log_path}"
    ] + sys.argv[1:],
    cwd=test_path.absolute().parent
).returncode

if returncode != 0:
    sys.exit(returncode)

# Check Valgrind log for errors
pattern = re.compile(
    r"Conditional jump or move depends on uninitialised value|definitely lost: 0 bytes|indirectly lost: 0 bytes|possibly lost: 0 bytes|still reachable: 0 bytes|All heap blocks were freed -- no leaks are possible"
)
with valgrind_log_path.open("r", encoding="utf-8") as file:
    valgrind_log_text = file.read()

valgrind_log_path.unlink(missing_ok=True)

messages = set()

for match in pattern.finditer(valgrind_log_text):
    messages.add(match.group())

if "Conditional jump or move depends on uninitialised value" in messages:
    sys.stdout.write(valgrind_log_text)
    sys.stdout.flush()
    sys.exit(1)
elif "definitely lost: 0 bytes" in messages and "indirectly lost: 0 bytes" in messages and "possibly lost: 0 bytes" in messages and "still reachable: 0 bytes" in messages:
    pass
elif "All heap blocks were freed -- no leaks are possible" in messages:
    pass
else:
    sys.stdout.write(valgrind_log_text)
    sys.stdout.flush()
    sys.exit(1)

# Update the hash since the test was successful
update_test_hash(test_path, hash_path)
