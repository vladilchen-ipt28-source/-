#include <iostream>
#include <chrono>
#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"

cv::Mat globalFrameForScreenshot;

void onMouseClick(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        if (!globalFrameForScreenshot.empty()) {
            std::string filename = "screenshot_" + std::to_string(time(nullptr)) + ".jpg";
            cv::imwrite(filename, globalFrameForScreenshot);
            std::cout << "Screenshot saved: " << filename << std::endl;
        }
    }
}

int main() {
    CameraProvider camera;
    Display display;
    ProcessMode currentMode = ProcessMode::ORIGINAL;
    int brightnessValue = 50; 

    if (!camera.initialize(0, "video.mp4")) {
        return -1;
    }

    display.init();

    std::cout << "\n=== PROGRAM STARTED ===" 
              << "\n1-6 - Switch filters"
              << "\n[ = ] (PLUS key) - Increase brightness"
              << "\n[ - ] (MINUS key) - Decrease brightness"
              << "\nMouse click on video - Take screenshot"
              << "\nEsc - Exit\n" << std::endl;

    cv::Mat frame, processedFrame;
    bool isRunning = true;
    bool mouseInitialized = false; 
    double fps = 0.0;

    while (isRunning) {
        auto startTime = std::chrono::high_resolution_clock::now();

        if (!camera.getFrame(frame)) {
            break;
        }

        FrameProcessor::process(frame, processedFrame, currentMode, brightnessValue);
        FrameProcessor::drawOverlayInfo(processedFrame, fps, currentMode);
        processedFrame.copyTo(globalFrameForScreenshot);

        display.show(processedFrame);

        if (!mouseInitialized) {
            cv::setMouseCallback(display.getWindowName(), onMouseClick, nullptr);
            mouseInitialized = true;
            std::cout << "Mouse callback successfully attached to the window." << std::endl;
        }

        int key = cv::waitKey(30);
        if (key == 27) { 
            isRunning = false;
        } else if (key == '=' || key == '+') { 
            brightnessValue = std::min(100, brightnessValue + 5);
            std::cout << "Brightness increased: " << brightnessValue << "%" << std::endl;
        } else if (key == '-') { 
            brightnessValue = std::max(0, brightnessValue - 5);
            std::cout << "Brightness decreased: " << brightnessValue << "%" << std::endl;
        } else {
            currentMode = KeyProcessor::processKey(key, currentMode);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;
        fps = 1.0 / elapsed.count();
    }

    std::cout << "Program finished successfully." << std::endl;
    return 0;
}
