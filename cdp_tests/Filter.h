#ifndef FILTER_H
#define FILTER_H

#include <opencv2/imgproc.hpp>

class Filter
{
public:
	bool enabled = true;

	virtual void Filter::apply(cv::Mat *depthMat, cv::Mat *rgbMat);
};

#endif // FILTER_H