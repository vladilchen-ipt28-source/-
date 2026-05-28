#include "FrameProcessor.hpp"
#include <string>

void FrameProcessor::process(const cv::Mat& input, cv::Mat& output, ProcessMode mode, int trackbarValue) {
    // Регулювання яскравості на основі повзунка (трекбару) для ВСІХ режимів
    cv::Mat adjustedInput;
    input.convertTo(adjustedInput, -1, 1.0, trackbarValue - 50); // Зсув яскравості (-50 до +50)

    switch (mode) {
        case ProcessMode::GRAYSCALE:
            cv::cvtColor(adjustedInput, output, cv::COLOR_BGR2GRAY);
            break;

        case ProcessMode::NEGATIVE:
            cv::bitwise_not(adjustedInput, output);
            break;

        case ProcessMode::CANNY_EDGES: {
            cv::Mat gray;
            cv::cvtColor(adjustedInput, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, output, 50, 150);
            break;
        }

        case ProcessMode::GAUSSIAN_BLUR: {
            int kernelSize = 15; // Стандартне розмиття
            cv::GaussianBlur(adjustedInput, output, cv::Size(kernelSize, kernelSize), 0);
            break;
        }

        case ProcessMode::THRESHOLD: {
            cv::Mat gray;
            cv::cvtColor(adjustedInput, gray, cv::COLOR_BGR2GRAY);
            cv::threshold(gray, output, 127, 255, cv::THRESH_BINARY);
            break;
        }

        case ProcessMode::ORIGINAL:
        default:
            adjustedInput.copyTo(output);
            break;
    }
}

void FrameProcessor::drawOverlayInfo(cv::Mat& frame, double fps, ProcessMode mode) {
    // Якщо кадр одноканальний (чорно-білий фільтр), конвертуємо в BGR для відображення кольорового тексту
    if (frame.channels() == 1) {
        cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
    }

    std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));
    std::string modeText = "Mode: ";
    switch (mode) {
        case ProcessMode::ORIGINAL: modeText += "Original"; break;
        case ProcessMode::GRAYSCALE: modeText += "Grayscale"; break;
        case ProcessMode::NEGATIVE: modeText += "Negative"; break;
        case ProcessMode::CANNY_EDGES: modeText += "Canny Edges"; break;
        case ProcessMode::GAUSSIAN_BLUR: modeText += "Gaussian Blur"; break;
        case ProcessMode::THRESHOLD: modeText += "Threshold"; break;
    }

    // Малюємо текст на кадрі за допомогою інструментів OpenCV
    cv::putText(frame, fpsText, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, modeText, cv::Point(20, 70), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 0), 2);
}
