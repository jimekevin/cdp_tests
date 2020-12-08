#include "ThresholdFilter.h"

ThresholdFilter::ThresholdFilter(float minDist, float maxDist) 
: minDist(minDist), maxDist(maxDist) 
{
}

const float ThresholdFilter::getMinDistance() {
	return minDist;
}

const float ThresholdFilter::getMaxDistance() {
	return maxDist;
}

void ThresholdFilter::updateMinDistance(float minDist) {
	this->minDist = minDist;
}

void ThresholdFilter::updateMaxDistance(float maxDist) {
	this->maxDist = maxDist;
}

void ThresholdFilter::apply(cv::Mat *depthMat, cv::Mat *rgbMat) {
	for (int y = 0; y < depthMat->rows; y++) {
		for (int x = 0; x < depthMat->cols; x++) {
			auto &depth = depthMat->at<cv::Vec3f>(y, x);
			if (depth[2] < minDist || depth[2] > maxDist) {
				depth[0] = 0.0f;
				depth[1] = 0.0f;
				depth[2] = 0.0f;
				depthMat->at<cv::Vec3f>(cv::Point(x, y)) = depth;
			}
		}
	}

	/*for (int y = 0; y < rgbMat->rows; y++) {
		for (int x = 0; x < rgbMat->cols; x++) {
			auto &color = rgbMat->at<cv::Vec3f>(y, x);
			color[0] = 1.0f;
			color[1] = 0.5f;
			color[2] = 0.1f;
			rgbMat->at<cv::Vec3f>(cv::Point(x,y)) = color;
		}
	}*/
}
