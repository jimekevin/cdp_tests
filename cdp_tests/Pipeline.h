#ifndef PIPELINE_H
#define PIPELINE_H

#include <vector>
#include <opencv2/imgproc.hpp>
#include "Filter.h"

class Pipeline
{
private:
	std::vector<Filter*> filters;

public:
	void process(cv::Mat *depthMat, cv::Mat *rgbMat);

	size_t addProcessingFilter(Filter *filter);
	void removeProcessingFilter(size_t index);
};

#endif // PIPELINE_H
