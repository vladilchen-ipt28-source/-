cmake_minimum_required(VERSION 3.10)
project(Lab6_OpenCV)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Додаємо всі нові .cpp файли сюди
add_executable(lab6_bin 
    main.cpp 
    CameraProvider.cpp 
    KeyProcessor.cpp 
    FrameProcessor.cpp 
    Display.cpp
)

target_link_libraries(lab6_bin ${OpenCV_LIBS})
