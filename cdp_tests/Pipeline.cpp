#include "Pipeline.h"
#include "Filter.h"

void Pipeline::process(cv::Mat& depthMat, cv::Mat& rgbMat) {
	for (const auto &filter : filters) {
		if (!filter->enabled) {
			continue;
		}
		filter->apply(depthMat, rgbMat);
	}
}


size_t Pipeline::addProcessingFilter(Filter *filter) {
	filters.push_back(filter);
	return filters.size() - 1;
}

void Pipeline::removeProcessingFilter(size_t index) {
	if (index >= 0 && index < filters.size()) {
		filters.erase(filters.begin() + index);
	}
}