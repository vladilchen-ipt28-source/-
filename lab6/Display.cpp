#include "Display.hpp"

Display::Display(const std::string& winName) : windowName(winName) {}

Display::~Display() {
    cv::destroyAllWindows();
}

void Display::createWin() {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

// Реалізація безпечного трекбара через callback
void Display::addTrackbar(const std::string& barName, cv::TrackbarCallback onChange, void* userdata, int maxValue, int startValue) {
    cv::createTrackbar(barName, windowName, nullptr, maxValue, onChange, userdata);
    cv::setTrackbarPos(barName, windowName, startValue);
}

void Display::setMouseCallback(cv::MouseCallback callback, void* userdata) {
    cv::setMouseCallback(windowName, callback, userdata);
}

void Display::show(const cv::Mat& frame) {
    cv::imshow(windowName, frame);
}
