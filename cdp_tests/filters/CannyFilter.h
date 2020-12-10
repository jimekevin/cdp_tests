#ifndef CANNY_FILTER_H
#define CANNY_FILTER_H

#include "Filter.h"
#include <opencv2/imgproc.hpp>

class CannyFilter :
	public Filter
{
private:
	int threshold1;
	int threshold2;

public:
	CannyFilter(int threshold1, int threshold2);

	const int getThreshold1();
	const int getThreshold2();

	void updateThreshold1(int threshold1);
	void updateThreshold2(int threshold2);

	void apply(cv::Mat& depthMat, cv::Mat& rgbMat);
};

#endif // CANNY_FILTER_H
