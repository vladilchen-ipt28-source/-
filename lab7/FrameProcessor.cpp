#include "FrameProcessor.hpp"
#include <opencv2/dnn.hpp>
#include <iostream>

void FrameProcessor::process(const cv::Mat& input, cv::Mat& output, ProcessMode mode, int brightnessValue) {
    // Спочатку копіюємо вхідний кадр
    input.copyTo(output);

    // Крок 1. Базове регулювання яскравості
    output += cv::Scalar(brightnessValue - 50, brightnessValue - 50, brightnessValue - 50);

    // Крок 2. Обробка фільтрів залежно від режиму
    switch (mode) {
        case ProcessMode::GRAYSCALE:
            cv::cvtColor(output, output, cv::COLOR_BGR2GRAY);
            cv::cvtColor(output, output, cv::COLOR_GRAY2BGR); // Повертаємо в BGR для накладання тексту
            break;
        case ProcessMode::NEGATIVE:
            cv::bitwise_not(output, output);
            break;
        case ProcessMode::CANNY: {
            cv::Mat edges;
            cv::Canny(output, edges, 50, 150);
            cv::cvtColor(edges, output, cv::COLOR_GRAY2BGR);
            break;
        }
        case ProcessMode::GAUSSIAN_BLUR:
            cv::GaussianBlur(output, output, cv::Size(15, 15), 0);
            break;
        case ProcessMode::THRESHOLD: {
            cv::Mat gray;
            cv::cvtColor(output, gray, cv::COLOR_BGR2GRAY);
            cv::threshold(gray, gray, 128, 255, cv::THRESH_BINARY);
            cv::cvtColor(gray, output, cv::COLOR_GRAY2BGR);
            break;
        }
        case ProcessMode::FACE: {
            // Ініціалізація мережі лише ОДИН раз при першому виклику
            static cv::dnn::Net net = cv::dnn::readNetFromCaffe("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
            
            if (net.empty()) {
                std::cerr << "Помилка: Не вдалося завантажити файли нейромережі!" << std::endl;
                break;
            }

            // Перетворюємо кадр у формат blob для ResNet-10 (розмір 300x300)
            cv::Mat blob = cv::dnn::blobFromImage(output, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);
            net.setInput(blob);
            
            // Запускаємо детекцію
            cv::Mat detections = net.forward();

            // Обробка результатів з 4D матриці
            float* data = (float*)detections.data;
            for (int i = 0; i < detections.total() / 7; ++i) {
                float confidence = data[i * 7 + 2];

                // Відфільтровуємо обличчя з впевненістю понад 50%
                if (confidence > 0.5) {
                    // Рахуємо координати рамки відносно розмірів кадру
                    int xLeftBottom = static_cast<int>(data[i * 7 + 3] * output.cols);
                    int yLeftBottom = static_cast<int>(data[i * 7 + 4] * output.rows);
                    int xRightTop = static_cast<int>(data[i * 7 + 5] * output.cols);
                    int yRightTop = static_cast<int>(data[i * 7 + 6] * output.rows);

                    // Створюємо та обмежуємо прямокутник межами кадру
                    cv::Rect faceBox(xLeftBottom, yLeftBottom, xRightTop - xLeftBottom, yRightTop - yLeftBottom);
                    faceBox &= cv::Rect(0, 0, output.cols, output.rows);

                    // Малюємо зелену рамку навколо обличчя
                    cv::rectangle(output, faceBox, cv::Scalar(0, 255, 0), 2);

                    // Виводимо текст із точністю детекції
                    std::string label = "Face: " + std::to_string(static_cast<int>(confidence * 100)) + "%";
                    cv::putText(output, label, cv::Point(faceBox.x, faceBox.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
                }
            }
            break;
        }
        default:
            break;
    }
}

void FrameProcessor::drawOverlayInfo(cv::Mat& frame, double fps, ProcessMode mode) {
    std::string modeText = "Mode: ";
    switch (mode) {
        case ProcessMode::ORIGINAL:      modeText += "Original"; break;
        case ProcessMode::GRAYSCALE:     modeText += "Grayscale"; break;
        case ProcessMode::NEGATIVE:      modeText += "Negative"; break;
        case ProcessMode::CANNY:         modeText += "Canny (Edges)"; break;
        case ProcessMode::GAUSSIAN_BLUR: modeText += "Blur"; break;
        case ProcessMode::THRESHOLD:     modeText += "Threshold"; break;
        case ProcessMode::FACE:          modeText += "Face Detection"; break;
    }

    std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));
    
    cv::putText(frame, modeText, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);
    cv::putText(frame, fpsText, cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);
}
