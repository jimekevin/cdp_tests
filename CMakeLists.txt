cmake_minimum_required(VERSION 3.17.3)

project(cdp_tests)

message(STATUS "Compiling for Mac OS")

add_definitions(-DAPPLE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(Qt5_DIR /usr/local/opt/qt/lib/cmake/Qt5)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
#find_package(libfreenect REQUIRED)
find_package(freenect2 REQUIRED PATHS /usr/local/Cellar/libfreenect2/0.2.0/lib)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
        ${PROJECT_SOURCE_DIR}/cdp_tests/main.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/MainWindow.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/KinectManager_MacOS.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/resources.qrc
        ${PROJECT_SOURCE_DIR}/cdp_tests/OutputTextBrowser.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/Pipeline.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/RecordVideoPushButton.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/Filter.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/MainGLWidget.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/filters/CannyFilter.cpp
        ${PROJECT_SOURCE_DIR}/cdp_tests/filters/ThresholdFilter.cpp
        #${PROJECT_SOURCE_DIR}/cdp_tests/glad/src/glad.c
        cdp_tests/KinectManager_MacOS.h)

add_executable(cdp_tests ${SOURCES})

include_directories(
        #${PROJECT_SOURCE_DIR}/cdp_tests/glad/include
        ${GLFW3_INCLUDE_DIRS}
        ${flextGL_INCLUDE_DIRS}
        #/usr/local/Cellar/libfreenect/0.6.1/include/libfreenect
        ${freenect2_INCLUDE_DIR}
        /usr/local/opt/qt/include
        ${PROJECT_SOURCE_DIR}/cdp_tests
        )

target_link_libraries(cdp_tests
        ${OpenCV_LIBS}
        ${OPENGL_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        #${FREENECT_LIBRARIES}
        ${freenect2_LIBRARIES}
        Qt5::Core Qt5::Gui Qt5::Widgets
        )
