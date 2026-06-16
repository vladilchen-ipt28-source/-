#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

class KeyProcessor {
public:
    enum Mode {
        ORIGINAL = 0,
        GRAYSCALE,
        CANNY,
        BLUR,
        INVERSION,
        GLITCH
    };

private:
    Mode currentMode;

public:
    KeyProcessor() : currentMode(ORIGINAL) {}
    void processKey(int key);
    Mode getMode() const;
};

#endif
