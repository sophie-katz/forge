#!/bin/bash

set -e

# Run test as normal

cd $(dirname $1) && $*
