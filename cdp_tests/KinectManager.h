#pragma once

#include <Kinect.h>
#include <opencv2/imgproc.hpp>
#include <QtGui/QOpenGLBuffer>

#define KM KinectManager::instance()

class KinectManager {
public:
	const static int DELAY_MS = 33;
	const static int WIDTH = 512;
	const static int HEIGHT = 424;
	const static int COLORWIDTH = 1920;
	const static int COLORHEIGHT = 1080;

	IMultiSourceFrameReader* reader;

private:
	KinectManager() {} // Disallow instantiation outside of the class.

	IKinectSensor *sensor;
	ICoordinateMapper *mapper; // Converts between depth, color, and 3d coordinates

	ColorSpacePoint depth2rgb[WIDTH * HEIGHT];  // Maps depth pixels to rgb pixels
	CameraSpacePoint depth2xyz[WIDTH * HEIGHT];
	unsigned char rgbimage[COLORWIDTH * COLORHEIGHT * 4];

	IColorFrameReader *colorFrameReader;
	IDepthFrameReader *depthFrameReader;

	int frameCount = 0;
	long long lastFrameFetch = 0;
	cv::Mat lastFrame;

public:
	KinectManager(const KinectManager&) = delete;
	KinectManager& operator=(const KinectManager &) = delete;
	KinectManager(KinectManager &&) = delete;
	KinectManager & operator=(KinectManager &&) = delete;

	static auto& instance() {
		static KinectManager test;
		return test;
	}

	int init();
	//int fetchFrames();

	void getDepthData(IMultiSourceFrame* frame, QOpenGLBuffer *glBuffer);
	void getRgbData(IMultiSourceFrame* frame, unsigned int* dest);
};