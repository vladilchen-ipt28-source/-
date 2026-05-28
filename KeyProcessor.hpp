#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

#include "FrameProcessor.hpp" // Він сам підтягне ProcessMode звідси

class KeyProcessor {
public:
    static ProcessMode processKey(int key, ProcessMode currentMode);
};

#endif
