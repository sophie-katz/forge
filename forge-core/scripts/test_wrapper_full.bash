#!/bin/bash

set -e

# Run test as normal

cd $(dirname $1) && $*

# If it passes, re-run using Valgrind to check for memory leaks

rm -f $(dirname $1)/valgrind.log
cd $(dirname $1) && valgrind \
    --tool=memcheck \
    --suppressions=/usr/share/glib-2.0/valgrind/glib.supp \
    --suppressions=../forge.supp \
    --leak-check=full \
    --show-leak-kinds=all \
    --log-file=valgrind.log \
    $*

if grep -P -q -e "Conditional jump or move depends on uninitialised value" $(dirname $1)/valgrind.log; then
    echo
    echo "[test_wrapper_full] ERROR: Test had conditional jump or move depends on uninitialised value"
    echo
    cat $(dirname $1)/valgrind.log
    exit 1
elif grep -P -q -e "definitely lost: 0 bytes" $(dirname $1)/valgrind.log && \
    grep -P -q -e "indirectly lost: 0 bytes" $(dirname $1)/valgrind.log && \
    grep -P -q -e "possibly lost: 0 bytes" $(dirname $1)/valgrind.log && \
    grep -P -q -e "still reachable: 0 bytes" $(dirname $1)/valgrind.log; then
    echo
    echo "[test_wrapper_full] No memory leaks detected"
elif grep -P -q -e "All heap blocks were freed -- no leaks are possible" $(dirname $1)/valgrind.log; then
    echo
    echo "[test_wrapper_full] No memory leaks detected"
else
    echo
    echo "[test_wrapper_full] ERROR: Test had memory leaks"
    echo
    cat $(dirname $1)/valgrind.log
    exit 1
fi

rm -f $(dirname $1)/valgrind.log
