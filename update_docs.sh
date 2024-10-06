#!/bin/sh -e

cd demo
godot-latest --headless --import
godot-latest --doctool .. --gdextension-docs

# godot keeps deleting all the docs, so fail if that happens
if ! test -f ../doc_classes/CardDef.xml; then
	cd ..
	git status
	false
fi
