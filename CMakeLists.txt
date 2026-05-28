cmake_minimum_required(VERSION 3.10)
project(OpenCV_Lab6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab6
    main.cpp
    CameraProvider.cpp
    KeyProcessor.cpp
    FrameProcessor.cpp
    Display.cpp
)

target_link_libraries(lab6 ${OpenCV_LIBS})
