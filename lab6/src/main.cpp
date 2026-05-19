#include "CameraProvider.hpp"
#include "Display.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include <iostream>

int main() {
    CameraProvider camera(0); 
    if (!camera.isOpened()) {
        std::cerr << "Помилка: не вдалося відкрити камеру" << std::endl;
        return -1;
    }

    Display display("Лабораторна 6 - OpenCV Обробка відео");
    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    
    cv::Mat srcFrame;
    cv::Mat processedFrame;

    std::cout << "Програму запущено" << std::endl;
    std::cout << "Керування режимами:" << std::endl;
    std::cout << "  '1' - Оригінальне відео" << std::endl;
    std::cout << "  '2' - Інверсія кольорів" << std::endl;
    std::cout << "  '3' - Розмиття (GaussianBlur)" << std::endl;
    std::cout << "  '4' - Контури (Canny фільтр)" << std::endl;
    std::cout << "  'q' або 'ESC' - Вихід з програми" << std::endl;

    // 3. Головний цикл
    while (true) {
        if (!camera.getFrame(srcFrame)) {
            std::cerr << "Помилка: втрачено зв'язок з камерою" << std::endl;
            break;
        }

        frameProcessor.process(srcFrame, processedFrame, keyProcessor.getMode());

        display.show(processedFrame);

        int key = cv::waitKey(30);

        if (key == 113 || key == 27) {
            std::cout << "Завершення роботи." << std::endl;
            break;
        }

        if (key != -1) { 
            keyProcessor.processKey(key);
        }
    }

    return 0;
}