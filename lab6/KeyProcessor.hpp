#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

enum class ProcessMode {
    ORIGINAL = 1,
    GRAYSCALE = 2,
    NEGATIVE = 3,
    CANNY_EDGES = 4,
    GAUSSIAN_BLUR = 5,
    THRESHOLD = 6
};

class KeyProcessor {
public:
    static ProcessMode processKey(int key, ProcessMode currentMode);
};

#endif
