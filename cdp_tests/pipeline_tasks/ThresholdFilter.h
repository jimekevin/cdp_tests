#ifndef THRESHOLD_FILTER_H
#define THRESHOLD_FILTER_H

#include "Pipeline.h"

class ThresholdFilter :
	public Pipeline::Task
{
private:
	float minDist;
	float maxDist;

public:
	ThresholdFilter(float minDist, float maxDist);
	const float getMinDistance();
	const float getMaxDistance();
	void updateMinDistance(float minDist);
	void updateMaxDistance(float maxDist);
	void apply(cv::Mat& depthMat, cv::Mat& rgbMat);
};

#endif // THRESHOLD_FILTER_H
