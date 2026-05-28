#include "Display.hpp"

Display::Display(const std::string& name) : windowName(name) {}

void Display::init() {
    // Просто створюємо чисте вікно, яке тепер ніколи не впаде
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

void Display::show(const cv::Mat& frame) {
    cv::imshow(windowName, frame);
}

int Display::getTrackbarValue() const {
    return 50; // Повертаємо дефолтне значення, щоб не міняти hpp
}

std::string Display::getWindowName() const {
    return windowName;
}
