#include <Kinect.h>
#include "KinectManager_Windows.h"
#include <opencv2/imgproc.hpp>
#include <QtGui/QOpenGLBuffer>
#include <iostream>
#include <fstream>
#include <chrono>
#include <opencv2/imgcodecs.hpp>

int KinectManager::initialize() {
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

void KinectManager::terminate() {
	sensor->Close();
}

long KinectManager::AcquireLatestFrame() {
	auto result = reader->AcquireLatestFrame(&lastFrame);
	if (result) {
		frameCount++;
	}
	return result;
}

void KinectManager::ReleaseLatestFrame() {
}

const int KinectManager::getDepthSize() {
	return WIDTH * HEIGHT * 3 * sizeof(float);
}

const int KinectManager::getRgbSize() {
	return WIDTH * HEIGHT * 3 * sizeof(float);
}

void KinectManager::writeDepthData(void *dest) {
	if (dest == NULL) {
		return;
	}

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

    /*glBuffer->allocate(sz * 3 * sizeof(float));
    glBuffer->bind();
    auto dest = glBuffer->mapRange(0, sz * 3 * sizeof(float), QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
    if (dest == NULL) {
        return;
    }*/

    result = mapper->MapDepthFrameToCameraSpace(WIDTH * HEIGHT, buf, WIDTH * HEIGHT, depth2xyz);
    float* fdest = (float*)dest;
    for (unsigned int i = 0; i < sz; i++) {
        *fdest++ = depth2xyz[i].X;
        *fdest++ = depth2xyz[i].Y;
        *fdest++ = depth2xyz[i].Z;
    }

    mapper->MapDepthFrameToColorSpace(WIDTH * HEIGHT, buf, WIDTH * HEIGHT, depth2rgb);

    //glBuffer->unmap();	
	
	if (depthframe) {
		depthframe->Release();
	}
}

void KinectManager::writeRgbData(void *dest) {
	if (dest == NULL) {
		return;
	}

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

    /*glBuffer->allocate(WIDTH * HEIGHT * 3 * sizeof(float));
    auto dest = glBuffer->mapRange(0, WIDTH * HEIGHT * 3 * sizeof(float), QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
    if (dest == NULL) {
        return;
    }*/

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
			// Don't copy alpha channel
        }
    }

    //glBuffer->unmap();

	if (colorframe) {
		colorframe->Release();
	}
}

void KinectManager::saveRGBImage(std::string path) {
	saveRGBImage(path, rgbimage, COLORWIDTH, COLORHEIGHT);
}

void KinectManager::saveRGBImage(std::string path, unsigned char *input, int width, int height) {
    cv::Mat frameRGB(height, width, CV_8UC4, input);
    cv::Mat frameBGRA(height, width, CV_8UC4);
    cv::cvtColor(frameRGB, frameBGRA, cv::COLOR_RGBA2BGRA);
    cv::Mat flippedFrame;
    cv::flip(frameBGRA, flippedFrame, 1);
    cv::imwrite(path, flippedFrame);
}

void KinectManager::startVideoRecording(std::string path) {
	if (videoRecording) {
		return;
	}

	// Thread dispatch 

	std::ofstream ofs(path, std::ofstream::out | std::ofstream::trunc);
	int count = 0;
	while (videoRecording) {
		if (count >= frameCount) {
			continue;
		}

		ofs.write((char*)depth2xyz, WIDTH * HEIGHT * 4);
		ofs.write((char*)rgbimage, COLORWIDTH * COLORHEIGHT * 4);
		std::cout << "Wrote frame " << frameCount << std::endl;
		count++;
	}
	ofs.close();
}

void KinectManager::stopVideoRecording() {
	if (!videoRecording) {
		return;
	}

	videoRecording = false;
}
