#!/bin/sh -e

./build_dev.sh "$@"

# now build all the versions we're actually going to use:
for target in debug release; do
docker run --rm -ti \
	--user "$(id -u):$(id -g)" \
	--volume "$(pwd):$(pwd)" \
	--workdir "$(pwd)" \
	--env SCONS_CACHE=.cache/linux_$target \
	registry.gitlab.steamos.cloud/steamrt/sniper/sdk \
	scons target=template_$target platform=linux arch=x86_64 "$@"
SCONS_CACHE=.cache/windows_$target scons target=template_$target platform=windows arch=x86_64 "$@"
SCONS_CACHE=.cache/web_threads_$target scons target=template_$target platform=web threads=yes "$@"
SCONS_CACHE=.cache/web_nothreads_$target scons target=template_$target platform=web threads=no "$@"
done
