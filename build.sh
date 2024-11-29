#!/bin/sh -e

# build native debug build for this system
SCONS_CACHE=.cache/dev scons dev_build=yes "$@"
# ...so we can update the docs
./update_docs.sh
# build native debug again to update the embedded docs
SCONS_CACHE=.cache/dev scons dev_build=yes "$@"

# now build all the versions we're actually going to use:
for target in debug release; do
SCONS_CACHE=.cache/linux_$target scons target=template_$target platform=linux arch=x86_64 "$@"
SCONS_CACHE=.cache/windows_$target scons target=template_$target platform=windows arch=x86_64 "$@"
SCONS_CACHE=.cache/web_threads_$target scons target=template_$target platform=web threads=yes "$@"
SCONS_CACHE=.cache/web_nothreads_$target scons target=template_$target platform=web threads=no "$@"
done
