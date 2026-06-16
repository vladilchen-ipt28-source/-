#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider(const std::string& path) : videoPath(path) {}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release();
    }
}

bool CameraProvider::init() {
    cap.open(videoPath);
    return cap.isOpened();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        // Зациклюємо відео, якщо воно закінчилося
        cap.set(cv::CAP_PROP_POS_FRAMES, 0);
        cap >> frame;
    }
    return frame;
}
