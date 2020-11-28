#ifndef KINECT_MANAGER_H
#define KINECT_MANAGER_H

#ifdef APPLE

#include "KinectManager_MacOS.h"

#include <iostream>
#include <chrono>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QtGui/QOpenGLBuffer>

//typedef struct { float X, Y; } ColorSpacePoint;
//typedef struct { float X, Y, Z; } CameraSpacePoint;

// https://openkinect.github.io/libfreenect2/

int KinectManager::init() {
    libfreenect2::setGlobalLogger(libfreenect2::createConsoleLogger(libfreenect2::Logger::Debug));

    if(freenect2.enumerateDevices() == 0) {
        std::cout << "No device connected!" << std::endl;
        return -1;
    }
    if (serial.empty()) {
        serial = freenect2.getDefaultDeviceSerialNumber();
    }

    // TODO: switch pipelines?
#ifdef LIBFREENECT2_WITH_OPENCL_SUPPORT
    pipeline = new libfreenect2::OpenCLPacketPipeline();
#else
    pipeline = new libfreenect2::CpuPacketPipeline();
#endif
    dev = freenect2.openDevice(serial, pipeline);
    if(dev == nullptr) {
        std::cout << "Failure opening device!" << std::endl;
        return -1;
    }

    int types = libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
    listener = new libfreenect2::SyncMultiFrameListener(types);

    dev->setColorFrameListener(listener);
    dev->setIrAndDepthFrameListener(listener);

    // Start the device
    if (!dev->start())  {
        return -1;
    }
    if (!dev->startStreams(true, true)) {
        return -1;
    }

    startedStreams = true;
    std::cout << "Device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "Device firmware: " << dev->getFirmwareVersion() << std::endl;

    return 0;
}

long KinectManager::AcquireLatestFrame() {
    if (!startedStreams) {
        return -1;
    }
    if (!listener->waitForNewFrame(frames, 10*1000)) {
        return -1;
    }
    rgbFrame = frames[libfreenect2::Frame::Color];
    irFrame = frames[libfreenect2::Frame::Ir];
    depthFrame = frames[libfreenect2::Frame::Depth];

    return 0;
}

void KinectManager::ReleaseLatestFrame() {
    if (startedStreams) {
        listener->release(frames);
    }
}

void KinectManager::getDepthData(QOpenGLBuffer *glBuffer) {
    if (depthFrame == nullptr) {
        return;
    }

    size_t sz = depthFrame->width * depthFrame->height;
    glBuffer->allocate(sz * 3 * sizeof(float));
    glBuffer->bind();
    auto dest = glBuffer->mapRange(0, sz * 3 * sizeof(float), QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
    if (dest == nullptr) {
        return;
    }

    // TODO: How to do this?
    //auto result = mapper->MapDepthFrameToCameraSpace(WIDTH * HEIGHT, buf, WIDTH * HEIGHT, depth2xyz);
    float* fdest = (float*)dest;
    for (unsigned int i = 0; i < sz; i++) {
        *fdest++ = depthFrame->data[i + 0];
        *fdest++ = depthFrame->data[i + 1];
        *fdest++ = depthFrame->data[i + 2];
    }

    //mapper->MapDepthFrameToColorSpace(WIDTH * HEIGHT, buf, WIDTH * HEIGHT, depth2rgb);

    glBuffer->unmap();
}

void KinectManager::getRgbData(QOpenGLBuffer *glBuffer) {
    // Map color images onto depth images
    auto registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(WIDTH, HEIGHT, 4);
    libfreenect2::Frame registered(WIDTH, HEIGHT, 4);
    registration->apply(rgbFrame, depthFrame, &undistorted, &registered);

    if (rgbFrame == nullptr || depthFrame == nullptr) {
        return;
    }

    glBuffer->allocate(WIDTH * HEIGHT * 3 * sizeof(float));
    auto dest = glBuffer->mapRange(0, WIDTH * HEIGHT * 3 * sizeof(float), QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
    if (dest == NULL) {
        return;
    }

    float* fdest = (float*)dest;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        *fdest++ = registered.data[i + 0];
        *fdest++ = registered.data[i + 1];
        *fdest++ = registered.data[i + 2];
        //ColorSpacePoint* p = reinterpret_cast<ColorSpacePoint*>(&registered.data[i]);
        // Check if color pixel coordinates are in bounds
//        if (p->X < 0 || p->Y < 0 || p->X > COLORWIDTH || p->Y > COLORHEIGHT) {
//            *fdest++ = 0;
//            *fdest++ = 0;
//            *fdest++ = 0;
//        }
//        else {
//            int idx = (int)p.X + COLORWIDTH * (int)p.Y;
//            *fdest++ = rgbimage[4 * idx + 0] / 255.;
//            *fdest++ = rgbimage[4 * idx + 1] / 255.;
//            *fdest++ = rgbimage[4 * idx + 2] / 255.;
        //}
        // Don't copy alpha channel
    }

    glBuffer->unmap();
}

void KinectManager::saveRGBImage(std::string path) {
    cv::Mat frameRGB(COLORHEIGHT, COLORWIDTH, CV_8UC4, rgbFrame->data);
    cv::Mat frameBGRA(COLORHEIGHT, COLORWIDTH, CV_8UC4);
    cv::cvtColor(frameRGB, frameBGRA, cv::COLOR_RGBA2BGRA);
    cv::Mat flippedFrame;
    cv::flip(frameBGRA, flippedFrame, 1);
    cv::imwrite(path, flippedFrame);
}

#endif // APPLE

#endif // KINECT_MANAGER_H