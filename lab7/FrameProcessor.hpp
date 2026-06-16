#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
private:
    int frameCounter;
    double fps;
    int brightnessSlider;

public:
    FrameProcessor();
    void processFrame(const cv::Mat& input, cv::Mat& output, KeyProcessor::Mode mode);
    void updateFPS(double currentFps);
    int* getBrightnessSliderPtr();
};

#endif
