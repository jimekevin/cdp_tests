#include <iostream>
#include <Kinect.h>
#include <opencv2/imgproc.hpp>
#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>
#include "window.h"
#include "KinectManager.h"

int main(int argc, char **argv) {

	QApplication app(argc, argv);
	QApplication::setOrganizationName("LS AI");
	QApplication::setOrganizationName("cdp.ai.ar.tum.de");
	QApplication::setApplicationName("Collaborative Design Platform");

	QSurfaceFormat format;
	//format.setDepthBufferSize(24);
	//format.setSamples(4);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	QSurfaceFormat::setDefaultFormat(format);

	KinectManager::instance().init();

	Window window;
	window.setFormat(format);
	window.resize(QSize(800, 600));
	window.show();

	app.exec();

	return 0;

	IKinectSensor *kinectSensor_;
	IColorFrameReader *colorFrameReader_;
	IDepthFrameReader *depthFrameReader_;
	ICoordinateMapper *coordinateMapper_;

	HRESULT result;

	result = GetDefaultKinectSensor(&kinectSensor_);
	if (FAILED(result) || !kinectSensor_)
		return false;

	result = kinectSensor_->get_CoordinateMapper(&coordinateMapper_);
	if (FAILED(result) || !coordinateMapper_)
		return false;

	result = kinectSensor_->Open();
	IColorFrameSource* colorFrameSource = nullptr;
	if (SUCCEEDED(result))
		result = kinectSensor_->get_ColorFrameSource(&colorFrameSource);
	if (SUCCEEDED(result))
		result = colorFrameSource->OpenReader(&colorFrameReader_);
	if (colorFrameSource)
		colorFrameSource->Release();

	result = kinectSensor_->Open();
	IDepthFrameSource* depthFrameSource = nullptr;
	if (SUCCEEDED(result))
		result = kinectSensor_->get_DepthFrameSource(&depthFrameSource);
	if (SUCCEEDED(result))
		result = depthFrameSource->OpenReader(&depthFrameReader_);
	else
		return 1;
	if (depthFrameSource)
		depthFrameSource->Release();

	int delayMS = 0;
	if (!depthFrameReader_) return 1;
	Sleep(delayMS);

	cv::Mat output;

	int i;
	for (i = 0; i < 30000; i++) {
		IDepthFrame* depthFrame = nullptr;
		result = depthFrameReader_->AcquireLatestFrame(&depthFrame);
		Sleep(1);
		if (FAILED(result)) {
			if (depthFrame)
				depthFrame->Release();
			depthFrame = nullptr;
			continue;
		}
		else {
			std::cout << i << ": Found a frame!" << std::endl;
		}

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
			const cv::Mat img(height, width, CV_8UC2, buffer);
			cv::cvtColor(img, output, cv::COLOR_YUV2BGR_YUY2);
		}

		if (frameDescription)
			frameDescription->Release();
		if (depthFrame)
			depthFrame->Release();
	}

	std::cout << "Test" << std::endl;
	return 0;
}