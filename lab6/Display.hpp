#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include <string>

class Display {
private:
    std::string windowName;

public:
    Display(const std::string& winName);
    ~Display();
    void createWin();
    // Змінено: тепер передаємо функцію-колбек для слайдера
    void addTrackbar(const std::string& barName, cv::TrackbarCallback onChange, void* userdata, int maxValue, int startValue);
    void setMouseCallback(cv::MouseCallback callback, void* userdata);
    void show(const cv::Mat& frame);
};

#endif
