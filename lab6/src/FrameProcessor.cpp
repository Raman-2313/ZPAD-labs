#include "FrameProcessor.hpp"

void FrameProcessor::process(const cv::Mat& src, cv::Mat& dst, KeyProcessor::Mode mode) {
    switch (mode) {
        case KeyProcessor::Mode::INVERSION:
            cv::bitwise_not(src, dst);
            break;

        case KeyProcessor::Mode::BLUR:
            cv::GaussianBlur(src, dst, cv::Size(15, 15), 0);
            break;

        case KeyProcessor::Mode::CANNY:
            cv::Canny(src, dst, 50, 150);
            break;

        case KeyProcessor::Mode::ORIGINAL:
        default:
            src.copyTo(dst);
            break;
    }
}