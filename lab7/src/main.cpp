#include <iostream>
#include <opencv2/opencv.hpp>
#include "CameraProvider.hpp"
#include "Display.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "FaceDetector.hpp"

int main() {
    CameraProvider camera(0); 
    if (!camera.isOpened()) {
        std::cerr << "Помилка: не вдалося відкрити камеру" << std::endl;
        return -1;
    }

    Display display("Лабораторна 7 - Computer Vision та багатопотоковість у C++");
    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    
    FaceDetector faceDetector("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
    
    cv::Mat srcFrame;
    cv::Mat processedFrame;

    std::cout << "Програму запущено" << std::endl;
    std::cout << "Керування режимами:" << std::endl;
    std::cout << "  '1' - Оригінальне відео" << std::endl;
    std::cout << "  '2' - Інверсія кольорів" << std::endl;
    std::cout << "  '3' - Розмиття (GaussianBlur)" << std::endl;
    std::cout << "  '4' - Контури (Canny фільтр)" << std::endl;
    std::cout << "  'f' або 'F' - Детекція облич (ШІ)" << std::endl;
    std::cout << "  'q' або 'ESC' - Вихід з програми" << std::endl;

    while (true) {
        if (!camera.getFrame(srcFrame)) {
            std::cerr << "Помилка: втрачено зв'язок з камерою" << std::endl;
            break;
        }

        KeyProcessor::Mode currentMode = keyProcessor.getMode();

        if (currentMode == KeyProcessor::Mode::FACE) {
            faceDetector.updateFrame(srcFrame);
            srcFrame.copyTo(processedFrame);
            std::vector<cv::Rect> faces = faceDetector.getFaces();
            for (const auto& box : faces) {
                cv::rectangle(processedFrame, box, cv::Scalar(0, 255, 0), 3);
                cv::putText(processedFrame, "FACE (AI)", cv::Point(box.x, box.y - 10),
                            cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
            }
        } else {
            frameProcessor.process(srcFrame, processedFrame, currentMode);
        }

        display.show(processedFrame);

        int key = cv::waitKey(30);

        if (key == 'q' || key == 'Q' || key == 27) {
            std::cout << "Завершення роботи." << std::endl;
            break;
        }

        if (key != -1) { 
            keyProcessor.processKey(key);
        }
    }

    return 0;
}
