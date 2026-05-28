#!/bin/bash
echo "=== Компіляція проєкту за допомогою CMake ==="
mkdir -p build
cd build
cmake ..
make -j$(nproc)
echo "=== Збірка завершена ==="
