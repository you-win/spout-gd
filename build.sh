#!/bin/bash

set -e

ROOT_DIR=$(basename "$0")

echo "Building Spout"
cd Spout2
cmake -DSKIP_INSTALL_ALL=OFF -DSKIP_INSTALL_HEADERS=OFF -DSKIP_INSTALL_LIBRARIES=OFF -DSPOUT_BUILD_LIBRARY=ON --fresh .
cmake --build . --clean-first
cd "$ROOT_DIR"

echo "Building godot-cpp"
cd godot-cpp
scons
cd "$ROOT_DIR"
