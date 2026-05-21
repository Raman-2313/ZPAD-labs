#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include <string>

class Display {
private:
    std::string windowName;
public:
    Display(const std::string& name);
    ~Display();
    void show(const cv::Mat& frame);
};

#endif
