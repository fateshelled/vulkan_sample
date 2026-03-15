#!/usr/bin/env bash
set -euo pipefail

# Install required toolchain and Vulkan development packages.
sudo apt update
sudo apt install -y build-essential clang cmake ninja-build git pkg-config libvulkan-dev

# Configure and build the sample.
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j

# Run the sample executable.
./build/vulkan_sample
