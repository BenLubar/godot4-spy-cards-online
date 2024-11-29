#!/bin/sh -e

# build native debug build for this system
SCONSCACHE=.sconsbuildcache/dev scons dev_build=yes "$@"
# ...so we can update the docs
./update_docs.sh
# build native debug again to update the embedded docs
SCONSCACHE=.sconsbuildcache/dev scons dev_build=yes "$@"

# now build all the versions we're actually going to use:
for target in debug release; do
SCONSCACHE=.sconsbuildcache/linux_$target scons target=template_$target platform=linux arch=x86_64 "$@"
SCONSCACHE=.sconsbuildcache/windows_$target scons target=template_$target platform=windows arch=x86_64 "$@"
SCONSCACHE=.sconsbuildcache/web_threads_$target scons target=template_$target platform=web threads=yes "$@"
SCONSCACHE=.sconsbuildcache/web_nothreads_$target scons target=template_$target platform=web threads=no "$@"
done
