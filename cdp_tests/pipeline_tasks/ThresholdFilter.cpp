#include "ThresholdFilter.h"

#include <cmath>

ThresholdFilter::ThresholdFilter(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
: minX(minX), maxX(maxX), minY(minY), maxY(maxY), minZ(minZ), maxZ(maxZ)
{
}

void ThresholdFilter::updateMinMax(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
	this->minX = minX != std::numeric_limits<double>::infinity() ? minX : this->minX;
	this->maxX = maxX != std::numeric_limits<double>::infinity() ? maxX : this->maxX;
	this->minY = minY != std::numeric_limits<double>::infinity() ? minY : this->minY;
	this->maxY = maxY != std::numeric_limits<double>::infinity() ? maxY : this->maxY;
	this->minZ = minZ != std::numeric_limits<double>::infinity() ? minZ : this->minZ;
	this->maxZ = maxZ != std::numeric_limits<double>::infinity() ? maxZ : this->maxZ;
}

void ThresholdFilter::apply(cv::Mat& depthMat, cv::Mat& rgbMat) {
	//float minX = 1000, maxX = FLT_MIN, minY = 1000, maxY = FLT_MIN, minZ = 1000, maxZ = FLT_MIN;
	for (int y = 0; y < depthMat.rows; y++) {
		for (int x = 0; x < depthMat.cols; x++) {
			auto &depth = depthMat.at<cv::Vec3f>(y, x);
			auto &rgb = rgbMat.at<cv::Vec3f>(y, x);
			//minX = depth[0] < minX && depth[0] > -1000 ? depth[0] : minX;
			//minY = depth[1] < minY && depth[1] > -1000 ? depth[1] : minY;
			//minZ = depth[2] < minZ && depth[2] > -1000 ? depth[2] : minZ;
			//maxX = depth[0] > maxX ? depth[0] : maxX;
			//maxY = depth[1] > maxY ? depth[1] : maxY;
			//maxZ = depth[2] > maxZ ? depth[2] : maxZ;

			if (depth[0] < minX || depth[0] > maxX || 
				depth[1] < minY || depth[1] > maxY || 
				depth[2] < minZ || depth[2] > maxZ
			) {
				depth[0] = 0.0f;
				depth[1] = 0.0f;
				depth[2] = 0.0f;
				depthMat.at<cv::Vec3f>(cv::Point(x, y)) = depth;

				auto &color = rgbMat.at<cv::Vec3f>(y, x);
				color[0] = 1.0f;
				color[1] = 1.0f;
				color[2] = 1.0f;
				rgbMat.at<cv::Vec3f>(cv::Point(x, y)) = color;
			}
		}
	}
	if (drawBorder) {
		cv::rectangle(rgbMat, cv::Rect(minX, minY, maxX - minX, maxY - minY), cv::Scalar(1.0f, 0.5f, 0.0f), 2);
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
