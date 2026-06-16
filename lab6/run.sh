#!/bin/bash
if [ -f "build/lab6_bin" ]; then
    ./build/lab6_bin
else
    echo "Помилка: спочатку запустіть ./build.sh"
fi
