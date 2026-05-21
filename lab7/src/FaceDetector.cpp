#include "FaceDetector.hpp"
#include <chrono>

FaceDetector::FaceDetector(const std::string& modelProto, const std::string& modelWeight) {
    net = cv::dnn::readNetFromCaffe(modelProto, modelWeight);
    running = true;
    workerThread = std::thread(&FaceDetector::update, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(mtx);
    frame.copyTo(frameToProcess);
    hasNewFrame = true;
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(mtx);
    return detectedFaces;
}

void FaceDetector::update() {
    while (running) {
        cv::Mat img;
        bool process = false;

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (hasNewFrame) {
                frameToProcess.copyTo(img);
                hasNewFrame = false;
                process = true;
            }
        }

        if (process && !img.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            cv::Mat blob = cv::dnn::blobFromImage(img, 1.0, cv::Size(300, 300), 
                                                 cv::Scalar(104.0, 177.0, 123.0));
            net.setInput(blob);
            cv::Mat detection = net.forward(); 

            cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
            std::vector<cv::Rect> localFaces;

            for (int i = 0; i < detectionMat.rows; i++) {
                float confidence = detectionMat.at<float>(i, 2);

                if (confidence > 0.5) {
                    int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * img.cols);
                    int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * img.rows);
                    int xRightTop   = static_cast<int>(detectionMat.at<float>(i, 5) * img.cols);
                    int yRightTop   = static_cast<int>(detectionMat.at<float>(i, 6) * img.rows);

                    localFaces.push_back(cv::Rect(xLeftBottom, yLeftBottom, 
                                                 xRightTop - xLeftBottom, yRightTop - yLeftBottom));
                }
            }

            {
                std::lock_guard<std::mutex> lock(mtx);
                detectedFaces = localFaces;
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}
