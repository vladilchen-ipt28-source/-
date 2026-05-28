#ifndef CAMERA_PROVIDER_HPP
#define CAMERA_PROVIDER_HPP

#include <opencv2/opencv.hpp>
#include <string>

class CameraProvider {
private:
    cv::VideoCapture cap;
    bool isVideoFile;

public:
    CameraProvider();
    ~CameraProvider();
    bool initialize(int deviceId = 0, const std::string& fallbackFile = "video.mp4");
    bool getFrame(cv::Mat& frame);
};

#endif
