#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    static void process(const cv::Mat& input, cv::Mat& output, ProcessMode mode, int trackbarValue);
    static void drawOverlayInfo(cv::Mat& frame, double fps, ProcessMode mode);
};

#endif

