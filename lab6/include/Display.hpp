#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include <string>

class Display {
private:
    std::string windowName;
public:
    Display(const std::string& name = "Lab 6 OpenCV");
    
    ~Display();
    
    void show(const cv::Mat& frame);
    
    std::string getWindowName() const;
};

#endif