#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include <iostream>
#include <chrono>

// Функція-колбек для слайдера яскравості
void onBrightnessChange(int pos, void* userdata) {
    int* brightnessPtr = static_cast<int*>(userdata);
    if (brightnessPtr) {
        *brightnessPtr = pos;
    }
}

// Функція для обробки подій мишки
void onMouseClick(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        std::cout << "Мишка клік: X=" << x << ", Y=" << y << std::endl;
    }
}

int main() {
    std::string winName = "Лабораторна робота №6 (Архітектурна)";
    
    CameraProvider camera("video.mp4");
    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display(winName);

    if (!camera.init()) {
        std::cerr << "Не вдалося завантажити відео файл!" << std::endl;
        return -1;
    }

    display.createWin();
    
    // Прапорець для відстеження ініціалізації інтерактивних елементів вікна
    bool isWindowUIInitialized = false;

    cv::Mat rawFrame, processedFrame;
    
    std::cout << "\nКерування: 0 - Оригінал | 1 - Gray | 2 - Canny | 3 - Blur | 4 - Inversion | 5 - Glitch | ESC - Вихід\n" << std::endl;

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        rawFrame = camera.getFrame();
        if (rawFrame.empty()) break;

        frameProcessor.processFrame(rawFrame, processedFrame, keyProcessor.getMode());
        display.show(processedFrame);

        // Гарантовано прокачуємо графічний потік перед налаштуванням UI елементів
        cv::waitKey(1);

        // Безпечна ініціалізація трекбара та миші, коли вікно ВЖЕ створено в ОС
        if (!isWindowUIInitialized) {
            try {
                // Налаштовуємо трекбар через безпечний колбек
                display.addTrackbar("Brightness", onBrightnessChange, frameProcessor.getBrightnessSliderPtr(), 100, 50);
                // Налаштовуємо мишку
                display.setMouseCallback(onMouseClick, nullptr);
                
                isWindowUIInitialized = true;
                std::cout << "[Успішно] Інтерактивні елементи інтерфейсу активовано." << std::endl;
            } catch (const cv::Exception& e) {
                // Якщо графічний сервер ОС ще не виділив вікно — спробуємо на наступному кадрі
                isWindowUIInitialized = false;
            }
        }

        int key = cv::waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') { 
            break;
        }
        
        keyProcessor.processKey(key);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        double currentFps = 1.0 / diff.count();
        frameProcessor.updateFPS(currentFps);
    }

    return 0;
}
