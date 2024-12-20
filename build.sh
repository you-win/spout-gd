#!/bin/bash

set -e

if [[ "$OSTYPE" != "msys" ]]; then
    echo "spout-gd only works on Windows"
    exit 1
fi

ROOT_DIR=$(readlink -f "$0")
ROOT_DIR=$(dirname "$ROOT_DIR")

echo "Building Spout"
cd Spout2
cmake -DSKIP_INSTALL_ALL=OFF -DSKIP_INSTALL_HEADERS=OFF -DSKIP_INSTALL_LIBRARIES=OFF -DSPOUT_BUILD_LIBRARY=ON --fresh .
cmake --build . --clean-first
cd "$ROOT_DIR"

echo "Building godot-cpp"
cd godot-cpp
scons
cd "$ROOT_DIR"

echo "Building spout-gd"
scons
scons target=template_release

echo "Move the .gdextension file + the dlls in out/ + Spout2/Binaries/x64/SpoutLibrary.dll to a new folder in addons called spout-gd"
