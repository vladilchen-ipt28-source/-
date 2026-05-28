#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>

enum class ProcessMode {
    ORIGINAL,
    GRAYSCALE,
    NEGATIVE,
    CANNY,
    GAUSSIAN_BLUR,
    THRESHOLD,
    FACE // Додали наш новий режим для детекції облич
};

class FrameProcessor {
public:
    static void process(const cv::Mat& input, cv::Mat& output, ProcessMode mode, int brightnessValue);
    static void drawOverlayInfo(cv::Mat& frame, double fps, ProcessMode mode);
};

#endif
