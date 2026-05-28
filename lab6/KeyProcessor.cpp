#include "KeyProcessor.hpp"

ProcessMode KeyProcessor::processKey(int key, ProcessMode currentMode) {
    if (key == '1') return ProcessMode::ORIGINAL;
    if (key == '2') return ProcessMode::GRAYSCALE;
    if (key == '3') return ProcessMode::NEGATIVE;
    if (key == '4') return ProcessMode::CANNY_EDGES;
    if (key == '5') return ProcessMode::GAUSSIAN_BLUR;
    if (key == '6') return ProcessMode::THRESHOLD;
    return currentMode;
}
