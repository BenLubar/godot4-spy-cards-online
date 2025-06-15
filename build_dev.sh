#!/bin/sh -e

# build native debug build for this system
SCONS_CACHE=.cache/dev scons dev_build=yes "$@"
# ...so we can update the docs
./update_docs.sh
# build native debug again to update the embedded docs
SCONS_CACHE=.cache/dev scons dev_build=yes compiledb=yes "$@"
