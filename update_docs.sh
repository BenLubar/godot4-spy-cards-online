#!/bin/sh -e

cd demo
godot-latest --headless --import
godot-latest --doctool .. --gdextension-docs
