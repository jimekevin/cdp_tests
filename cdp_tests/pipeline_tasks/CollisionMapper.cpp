#include "CollisionMapper.h"

CollisionMapper::CollisionMapper(ContourDetector *cd, Map *map)
	: cd(cd), map(map)
{
}

void CollisionMapper::apply(cv::Mat& depthMat, cv::Mat& rgbMat) {
	if (cd->contours.size() <= 0) {
		return;
	}

	map->unmarkAllBuildings();

	// 1. Collision detection
	int i = -1;
	for (const auto &c : cd->contours) {
		i++;
		for (const auto &p : c) {
			if (cd->areas[i] > 5000 || cd->areas[i] < 1000) {
				continue;
			}

			std::vector<int> ids;
			map->buildingsCollideWithPoint(p.x / 512, p.y /424, ids);
			for (auto const &id : ids) {
				map->markBuilding(id);
			}
		}
	}

	// 2. Mark detected object
}
