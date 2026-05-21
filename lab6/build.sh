#!/bin/bash
echo "Початок збірки"
mkdir -p build
cd build
echo "Генерація файлів конфігурації CMake"
cmake ..
echo "Компіляція коду..."
make 
echo "Збірку завершено"