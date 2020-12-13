#include "CollisionMapper.h"

CollisionMapper::CollisionMapper(ContourDetector cd, Map map) 
	: cd(cd), map(map)
{
}

void CollisionMapper::apply(cv::Mat& depthMat, cv::Mat& rgbMat) {
	if (cd.contours.size() <= 0) {
		return;
	}

	// 1. Collision detection
	for (const auto &c : cd.contours) {
		for (const auto &p : c) {
			// Map has only rectangles (so far)
			for (map.)
			if (p.x
		}
	}

	// 2. Mark detected object
}