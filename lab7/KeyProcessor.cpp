#include "KeyProcessor.hpp"

ProcessMode KeyProcessor::processKey(int key, ProcessMode currentMode) {
    switch (key) {
        case '1': return ProcessMode::ORIGINAL;
        case '2': return ProcessMode::GRAYSCALE;
        case '3': return ProcessMode::NEGATIVE;
        case '4': return ProcessMode::CANNY;
        case '5': return ProcessMode::GAUSSIAN_BLUR;
        case '6': return ProcessMode::THRESHOLD;
        case '7': return ProcessMode::FACE; // Додали перемикання на нейромережу по кнопці 7
        default: return currentMode;
    }
}
