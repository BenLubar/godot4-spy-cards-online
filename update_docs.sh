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

# godot keeps deleting all the docs, so fail if that happens
# https://github.com/godotengine/godot/issues/97937
if ! test -f ../doc_classes/CardDef.xml; then
	cd ..
	git status
	git checkout ./doc_classes
	false
fi
