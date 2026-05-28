#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include <string>

class Display {
private:
    std::string windowName;
    int trackbarValue; // Пряма змінна для повзунка

public:
    Display(const std::string& name = "OpenCV Лабораторна 6");
    void init();
    void show(const cv::Mat& frame);
    int getTrackbarValue() const;
    std::string getWindowName() const;
};

#endif
