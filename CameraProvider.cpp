#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider() : isVideoFile(false) {}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) cap.release();
}

bool CameraProvider::initialize(int deviceId, const std::string& fallbackFile) {
    std::cout << "Спроба підключення до камери через V4L2 (ID: " << deviceId << ")..." << std::endl;
    cap.open(deviceId, cv::CAP_V4L2);

    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        cap.set(cv::CAP_PROP_FPS, 30);
    }

    cv::Mat testFrame;
    if (cap.isOpened()) {
        for (int i = 0; i < 5; ++i) cap.read(testFrame);
    }

    if (cap.isOpened() && !testFrame.empty()) {
        std::cout << " Камера ініціалізована успішно!" << std::endl;
        return true;
    }

    if (cap.isOpened()) cap.release();

    std::cout << " Жива камера недоступна. Пробуємо файл: " << fallbackFile << std::endl;
    cap.open(fallbackFile);

    if (!cap.isOpened()) {
        std::cout << " Відеофайл не знайдено. Вмикаємо режим автогенерації тестового відеопотоку!" << std::endl;
    } else {
        isVideoFile = true;
    }

    return true; // Завжди повертаємо true, бо у нас є автогенерація!
}

bool CameraProvider::getFrame(cv::Mat& frame) {
    // Якщо відкрита камера або файл — беремо кадр звідти
    if (cap.isOpened()) {
        cap >> frame;
        if (!frame.empty()) return true;

        // Якщо це був файл і він закінчився — пускаємо по колу
        if (isVideoFile) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            cap >> frame;
            if (!frame.empty()) return true;
        }
    }

    // ГЕНЕРАЦІЯ ШТУЧНОГО ВІДЕОПОТОКУ (якщо фізичного джерела немає)
    // Створюємо динамічну кольорову матрицю, що змінюється з часом
    static int tick = 0;
    tick += 2;

    frame = cv::Mat::zeros(480, 640, CV_8UC3);

    // Малюємо рухоме коло та градієнт для тестування фільтрів яскравості та контурів
    cv::circle(frame, cv::Point(320 + sin(tick * 0.05) * 150, 240), 80, cv::Scalar(0, 0, 255), -1);
    cv::rectangle(frame, cv::Point(50, 50), cv::Point(200, 200), cv::Scalar(0, 255, 0), 5);
    cv::putText(frame, "TEST MODE (NO CAMERA)", cv::Point(150, 440), 
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);

  return true;
}
