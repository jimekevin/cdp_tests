#pragma once

#include <Kinect.h>
#include "KinectManager_Windows.h"
#include <opencv2/imgproc.hpp>
#include <QtGui/QOpenGLBuffer>
#include <iostream>
#include <chrono>
#include <opencv2/imgcodecs.hpp>

int KinectManager::init() {
    if (FAILED(GetDefaultKinectSensor(&sensor)) || !sensor) {
        return S_FALSE;
    }

    sensor->get_CoordinateMapper(&mapper);
    sensor->Open();
    sensor->OpenMultiSourceFrameReader(
            FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color,
            &reader
    );

    return S_OK;
}

long KinectManager::AcquireLatestFrame() {
	return reader->AcquireLatestFrame(&lastFrame);
}

void KinectManager::ReleaseLatestFrame() {
}

void KinectManager::getDepthData(QOpenGLBuffer *glBuffer) {
    IDepthFrame* depthframe;
    IDepthFrameReference* frameref = NULL;

	lastFrame->get_DepthFrameReference(&frameref);
    frameref->AcquireFrame(&depthframe);
    if (frameref) {
        frameref->Release();
    }

    if (!depthframe) {
        return;
    }

    // Get data from frame
    unsigned int sz;
    unsigned short* buf;
    HRESULT result;
    result = depthframe->AccessUnderlyingBuffer(&sz, &buf);

    glBuffer->allocate(sz * 3 * sizeof(float));
    glBuffer->bind();
    auto dest = glBuffer->mapRange(0, sz * 3 * sizeof(float), QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
    if (dest == NULL) {
        return;
    }

    result = mapper->MapDepthFrameToCameraSpace(WIDTH * HEIGHT, buf, WIDTH * HEIGHT, depth2xyz);
    float* fdest = (float*)dest;
    for (unsigned int i = 0; i < sz; i++) {
        *fdest++ = depth2xyz[i].X;
        *fdest++ = depth2xyz[i].Y;
        *fdest++ = depth2xyz[i].Z;
    }

    mapper->MapDepthFrameToColorSpace(WIDTH * HEIGHT, buf, WIDTH * HEIGHT, depth2rgb);

    glBuffer->unmap();	
	
	if (depthframe) {
		depthframe->Release();
	}
}

void KinectManager::getRgbData(QOpenGLBuffer *glBuffer) {
    IColorFrame* colorframe;
    IColorFrameReference* frameref = NULL;

	lastFrame->get_ColorFrameReference(&frameref);
    frameref->AcquireFrame(&colorframe);
    if (frameref) {
        frameref->Release();
    }

    if (!colorframe) {
        return;
    }

    glBuffer->allocate(WIDTH * HEIGHT * 3 * sizeof(float));
    auto dest = glBuffer->mapRange(0, WIDTH * HEIGHT * 3 * sizeof(float), QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
    if (dest == NULL) {
        return;
    }

    colorframe->CopyConvertedFrameDataToArray(COLORWIDTH * COLORHEIGHT * 4, rgbimage, ColorImageFormat_Rgba);

    float* fdest = (float*)dest;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        ColorSpacePoint p = depth2rgb[i];
        // Check if color pixel coordinates are in bounds
        if (p.X < 0 || p.Y < 0 || p.X > COLORWIDTH || p.Y > COLORHEIGHT) {
            *fdest++ = 0;
            *fdest++ = 0;
            *fdest++ = 0;
        }
        else {
            int idx = (int)p.X + COLORWIDTH * (int)p.Y;
            *fdest++ = rgbimage[4 * idx + 0] / 255.;
            *fdest++ = rgbimage[4 * idx + 1] / 255.;
            *fdest++ = rgbimage[4 * idx + 2] / 255.;
        }
        // Don't copy alpha channel
    }

    glBuffer->unmap();

	if (colorframe) {
		colorframe->Release();
	}
}

void KinectManager::saveRGBImage(std::string path) {
    cv::Mat frameRGB(COLORHEIGHT, COLORWIDTH, CV_8UC4, rgbimage);
    cv::Mat frameBGRA(COLORHEIGHT, COLORWIDTH, CV_8UC4);
    cv::cvtColor(frameRGB, frameBGRA, cv::COLOR_RGBA2BGRA);
    cv::Mat flippedFrame;
    cv::flip(frameBGRA, flippedFrame, 1);
    cv::imwrite(path, flippedFrame);
}