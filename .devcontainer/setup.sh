#!/usr/bin/env bash
set -e

apt-get update
apt-get install -y build-essential \
  cmake \
  pkg-config \
  python3 \
  python3-pip \
  git

python3 -m pip install --upgrade pip
python3 -m pip install "conan>=2,<3"

conan --version

conan profile detect --force
conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

git config --global --add safe.directory /workspace

echo "Ready!"
