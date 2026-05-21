#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

class FaceDetector {
private:
    cv::dnn::Net net;
    std::thread workerThread;
    std::mutex mtx;
    std::atomic<bool> running;
    cv::Mat frameToProcess;
    std::vector<cv::Rect> detectedFaces;
    bool hasNewFrame = false;
    void update();
public:
    FaceDetector(const std::string& modelProto, const std::string& modelWeight);
    ~FaceDetector();
    void updateFrame(const cv::Mat& frame);
    std::vector<cv::Rect> getFaces();
};

#endif
