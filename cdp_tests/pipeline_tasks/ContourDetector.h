#ifndef CANNY_FILTER_H
#define CANNY_FILTER_H

#include "Pipeline.h"
#include <opencv2/imgproc.hpp>

class ContourDetector :
	public Pipeline::Task
{
	std::vector<std::vector<cv::Point>> contours;

public:
	float threshold1;
	float threshold2;

	ContourDetector(float threshold1, float threshold2);

	void apply(cv::Mat& depthMat, cv::Mat& rgbMat);
};

#endif // CANNY_FILTER_H
