#pragma once

#include <Kinect.h>
#include "KinectManager.h"
#include <opencv2/imgproc.hpp>
#include <QtGui/QOpenGLBuffer>
#include <iostream>
#include <chrono>

int KinectManager::init() {
	if (FAILED(GetDefaultKinectSensor(&sensor)) || !sensor) {
		return 1;
	}

	sensor->get_CoordinateMapper(&mapper);
	sensor->Open();
	sensor->OpenMultiSourceFrameReader(
		FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color,
		&reader
	);

	/*IColorFrameSource* colorFrameSource = nullptr;
	if (FAILED(result)) return 3;
	result = kinectSensor_->get_ColorFrameSource(&colorFrameSource);
	if (FAILED(result)) return 4;
	result = colorFrameSource->OpenReader(&colorFrameReader_);
	if (!colorFrameReader_) return 5;
	if (colorFrameSource)
		colorFrameSource->Release();

	result = kinectSensor_->Open();
	IDepthFrameSource* depthFrameSource = nullptr;
	if (FAILED(result)) return 6;
	result = kinectSensor_->get_DepthFrameSource(&depthFrameSource);
	if (FAILED(result)) return 7;
		result = depthFrameSource->OpenReader(&depthFrameReader_);
	if (!depthFrameReader_) return 8;
	if (depthFrameSource)
		depthFrameSource->Release();*/

	return S_OK;
}

/*int KinectManager::fetchFrames() {
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();

	if (lastFrameFetch + DELAY_MS > ms) {
		return S_FALSE;
	}

	cv::Mat output;
	HRESULT result;

	IDepthFrame* depthFrame = nullptr;
	result = depthFrameReader_->AcquireLatestFrame(&depthFrame);

	if (FAILED(result)) {
		if (depthFrame)
			depthFrame->Release();
		depthFrame = nullptr;
		return S_FALSE;
	}
	
	frameCount += 1;
	lastFrameFetch = ms;

	std::cout << frameCount << ": Found a frame!" << std::endl;

	IFrameDescription* frameDescription = nullptr;
	int width = 0, height = 0;
	UINT bufferSize = 0;
	UINT16* buffer = nullptr;

	result = depthFrame->get_FrameDescription(&frameDescription);
	if (SUCCEEDED(result))
		result = frameDescription->get_Width(&width);
	if (SUCCEEDED(result))
		result = frameDescription->get_Height(&height);
	if (SUCCEEDED(result))
		result = depthFrame->AccessUnderlyingBuffer(&bufferSize, &buffer);

	if (SUCCEEDED(result)) {
		if (lastFrame != nullptr) {
			delete lastFrame;
		}
		//lastFrame = new cv::Mat(height, width, CV_8UC2, buffer);
		const cv::Mat img(height, width, CV_8UC2, buffer);
		cv::cvtColor(img, lastFrame, cv::COLOR_YUV2BGR_YUY2);
	}

	if (frameDescription)
		frameDescription->Release();
	if (depthFrame)
		depthFrame->Release();

	return S_OK;
}*/


void KinectManager::getDepthData(IMultiSourceFrame* frame, QOpenGLBuffer *glBuffer) {
	IDepthFrame* depthframe;
	IDepthFrameReference* frameref = NULL;

	frame->get_DepthFrameReference(&frameref);
	frameref->AcquireFrame(&depthframe);
	if (frameref) frameref->Release();

	if (!depthframe) {
		return;
	}

	// Get data from frame
	unsigned int sz;
	unsigned short* buf;
	HRESULT result;
	result = depthframe->AccessUnderlyingBuffer(&sz, &buf);

	glBuffer->allocate(sz * 3 * sizeof(float));
	auto dest = glBuffer->mapRange(0, sz * sizeof(float) * 3, QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
	//auto dest = (GLubyte*)glBuffer->map(QOpenGLBuffer::Access::WriteOnly);

	// Write vertex coordinates
	result = mapper->MapDepthFrameToCameraSpace(WIDTH * HEIGHT, buf, WIDTH * HEIGHT, depth2xyz);
	float* fdest = (float*)dest;
	for (int i = 0; i < sz; i++) {
		*fdest++ = depth2xyz[i].X;
		*fdest++ = depth2xyz[i].Y;
		*fdest++ = 1.0; // depth2xyz[i].Z;
	}

	// Fill in depth2rgb map
	//mapper->MapDepthFrameToColorSpace(WIDTH * HEIGHT, buf, WIDTH * HEIGHT, depth2rgb);

	glBuffer->unmap();

	if (depthframe) {
		depthframe->Release();
	}
}

void KinectManager::getRgbData(IMultiSourceFrame* frame, unsigned int* dest) {
	IColorFrame* colorframe;
	IColorFrameReference* frameref = NULL;
	frame->get_ColorFrameReference(&frameref);
	frameref->AcquireFrame(&colorframe);
	if (frameref) frameref->Release();

	if (!colorframe) return;

	// Get data from frame
	colorframe->CopyConvertedFrameDataToArray(COLORWIDTH * COLORHEIGHT * 4, rgbimage, ColorImageFormat_Rgba);

	// Write color array for vertices
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

	if (colorframe) colorframe->Release();
}
