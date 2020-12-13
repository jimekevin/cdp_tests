#ifndef CANNY_FILTER_H
#define CANNY_FILTER_H

#include "Pipeline.h"
#include <opencv2/imgproc.hpp>

class ContourFilter :
	public Pipeline::Task
{
private:
	float threshold1;
	float threshold2;

public:
	ContourFilter(float threshold1, float threshold2);

	const float getThreshold1();
	const float getThreshold2();

	void updateThreshold1(float threshold1);
	void updateThreshold2(float threshold2);

	void apply(cv::Mat& depthMat, cv::Mat& rgbMat);
};

#endif // CANNY_FILTER_H
