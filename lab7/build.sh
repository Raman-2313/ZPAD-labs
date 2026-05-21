#!/bin/bash
mkdir -p build
cd build
echo "Запуск генерації файлів збірки через CMake..."
cmake ..
echo "Запуск компіляції проєкту через Make..."
make
cd ..
echo "Збірка успішно завершена"