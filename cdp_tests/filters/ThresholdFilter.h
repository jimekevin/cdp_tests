#ifndef THRESHOLD_FILTER_H
#define THRESHOLD_FILTER_H

#include "Filter.h"

class ThresholdFilter :
	public Filter
{
private:
	float minDist;
	float maxDist;

public:
	ThresholdFilter(float minDist, float maxDist);
	const float getMinDistance();
	const float getMaxDistance();
	void ThresholdFilter::updateMinDistance(float minDist);
	void ThresholdFilter::updateMaxDistance(float maxDist);
	void apply(cv::Mat& depthMat, cv::Mat& rgbMat);
};

#endif // THRESHOLD_FILTER_H
