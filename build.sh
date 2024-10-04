#!/bin/sh -e

# build native debug build for this system
scons
# ...so we can update the docs
./update_docs.sh

# now build all the versions we're actually going to use:
for target in template_debug template_release; do
scons target=$target platform=linux arch=x86_64
scons target=$target platform=windows arch=x86_64
scons target=$target platform=web threads=yes
scons target=$target platform=web threads=no
done
