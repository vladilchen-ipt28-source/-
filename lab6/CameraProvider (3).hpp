#ifndef CAMERA_PROVIDER_HPP
#define CAMERA_PROVIDER_HPP

#include <opencv2/opencv.hpp>
#include <string>

class CameraProvider {
private:
    cv::VideoCapture cap;
    std::string videoPath;

public:
    CameraProvider(const std::string& path);
    ~CameraProvider();
    bool init();
    cv::Mat getFrame();
};

#endif
