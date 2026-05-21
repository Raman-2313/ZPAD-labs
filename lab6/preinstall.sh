#!/bin/bash
echo "Початок підготовки системи"
echo "Оновлення списку пакетів (apt update)"
sudo apt update
echo "Встановлення OpenCV, CMake та компіляторів..."
sudo apt install -y libopencv-dev cmake build-essential
echo "Систему успішно підготовлено до збірки"