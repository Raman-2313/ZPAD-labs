#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    FrameProcessor() = default;

    void process(const cv::Mat& src, cv::Mat& dst, KeyProcessor::Mode mode);
};

#endif