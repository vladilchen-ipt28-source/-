#include "KeyProcessor.hpp"
#include <iostream>

void KeyProcessor::processKey(int key) {
    if (key == '0') {
        currentMode = ORIGINAL;
        std::cout << "Режим: Оригінал" << std::endl;
    } else if (key == '1') {
        currentMode = GRAYSCALE;
        std::cout << "Режим: Grayscale" << std::endl;
    } else if (key == '2') {
        currentMode = CANNY;
        std::cout << "Режим: Canny" << std::endl;
    } else if (key == '3') {
        currentMode = BLUR;
        std::cout << "Режим: Blur" << std::endl;
    } else if (key == '4') {
        currentMode = INVERSION;
        std::cout << "Режим: Інверсія" << std::endl;
    } else if (key == '5') {
        currentMode = GLITCH;
        std::cout << "Режим: Glitch Effect" << std::endl;
    }
}

KeyProcessor::Mode KeyProcessor::getMode() const {
    return currentMode;
}
