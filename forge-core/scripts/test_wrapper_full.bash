#!/bin/bash

set -e

# Run test as normal

$*

# If it passes, re-run using Valgrind to check for memory leaks

rm -f valgrind.log
valgrind --tool=memcheck --log-file=valgrind.log $*

if grep -P -q -e "no leaks are possible" valgrind.log; then
    echo
    echo "No memory leaks detected"
else
    echo
    echo "ERROR: Test had memory leaks"
    exit 1
fi

cat valgrind.log

rm -f valgrind.log
