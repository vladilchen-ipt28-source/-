#include "FrameProcessor.hpp"
#include <string>
#include <vector>

FrameProcessor::FrameProcessor() : frameCounter(0), fps(0.0), brightnessSlider(50) {}

int* FrameProcessor::getBrightnessSliderPtr() {
    return &brightnessSlider;
}

void FrameProcessor::updateFPS(double currentFps) {
    fps = currentFps;
}

void FrameProcessor::processFrame(const cv::Mat& input, cv::Mat& output, KeyProcessor::Mode mode) {
    frameCounter++;
    cv::Mat tempFrame = input.clone();

    // 1. Застосування слайдера яскравості
    int offset = brightnessSlider - 50; 
    tempFrame.convertTo(tempFrame, -1, 1, offset * 2);

    // 2. Обробка згідно з обраним режимом
    switch (mode) {
        case KeyProcessor::GRAYSCALE:
            cv::cvtColor(tempFrame, output, cv::COLOR_BGR2GRAY);
            cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
            break;
        case KeyProcessor::CANNY:
            cv::cvtColor(tempFrame, output, cv::COLOR_BGR2GRAY);
            cv::Canny(output, output, 100, 200);
            cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
            break;
        case KeyProcessor::BLUR:
            cv::GaussianBlur(tempFrame, output, cv::Size(15, 15), 0);
            break;
        case KeyProcessor::INVERSION:
            cv::bitwise_not(tempFrame, output);
            break;
        case KeyProcessor::GLITCH: {
            // Ефект розсуву каналів (Glitch effect)
            std::vector<cv::Mat> channels;
            cv::split(tempFrame, channels);
            
            cv::Mat rShifted = cv::Mat::zeros(channels[2].size(), channels[2].type());
            cv::Mat bShifted = cv::Mat::zeros(channels[0].size(), channels[0].type());
            
            channels[2](cv::Rect(10, 0, channels[2].cols - 10, channels[2].rows)).copyTo(rShifted(cv::Rect(0, 0, channels[2].cols - 10, channels[2].rows)));
            channels[0](cv::Rect(0, 0, channels[0].cols - 10, channels[0].rows)).copyTo(bShifted(cv::Rect(10, 0, channels[0].cols - 10, channels[0].rows)));
            
            channels[2] = rShifted;
            channels[0] = bShifted;
            cv::merge(channels, output);
            break;
        }
        case KeyProcessor::ORIGINAL:
        default:
            output = tempFrame.clone();
            break;
    }

    // 3. Відображення тексту (FPS та Лічильник кадрів)
    std::string infoText = "Frames: " + std::to_string(frameCounter) + " | FPS: " + std::to_string((int)fps);
    cv::putText(output, infoText, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
}
