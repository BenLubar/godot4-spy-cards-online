#!/bin/sh -e

GODOT=godot
if command -v godot-latest >/dev/null 2>&1; then
	GODOT=godot-latest
fi
if command -v godot-dev >/dev/null 2>&1; then
	GODOT=godot-dev
fi

cd demo
xvfb-run numactl --physcpubind=+1 $GODOT --import
$GODOT --doctool .. --gdextension-docs

# fail if we failed to generate docs
if ! test -f ../doc_classes/RNG.xml; then
	cd ..
	git status
	git checkout ./doc_classes
	false
fi

cd ../doc_classes
ln -sf ../godot4-squirrel/doc_classes/Squirrel*.xml .
