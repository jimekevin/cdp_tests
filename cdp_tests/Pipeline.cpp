#include "Pipeline.h"

void Pipeline::process(cv::Mat& depthMat, cv::Mat& rgbMat) {
	for (const auto &task : tasks) {
		if (!task->enabled) {
			continue;
		}
		task->apply(depthMat, rgbMat);
	}
}


size_t Pipeline::addProcessingTask(Task *filter) {
	tasks.push_back(filter);
	return tasks.size() - 1;
}

void Pipeline::removeProcessingTask(size_t index) {
	if (index >= 0 && index < tasks.size()) {
		tasks.erase(tasks.begin() + index);
	}
}