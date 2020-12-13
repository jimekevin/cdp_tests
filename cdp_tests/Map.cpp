#include "Map.h"

#include <cassert>

#define MAKE_RECT(x, y, z, w, h, r, g, b) { \
	{ -(x) - w/2, y - h/2, z, r, g, b }, \
	{ -(x) + w/2, y - h/2, z, r, g, b }, \
	{ -(x) + w/2, y + h/2, z, r, g, b }, \
	{ -(x) - w/2, y - h/2, z, r, g, b }, \
	{ -(x) - w/2, y + h/2, z, r, g, b }, \
	{ -(x) + w/2, y + h/2, z, r, g, b }, \
}

const float vertices[][6][6] = {
	//MAKE_RECT(0.0f, 0.0f, 0.50f, 4.0f, 4.0f, 0.5f, 0.5f, 0.5f), // Plane
	//MAKE_RECT(0.0f, 0.0f, 0.4999f, 1.0f, 1.0f, 0.0f, 0.7f, 0.0f), // Building 1
	//MAKE_RECT(1.0f, 1.0f, 0.4999f, 0.5f, 0.5f, 0.0f, 1.0f, 0.2f), // Building 2
	MAKE_RECT(0.2f, 0.25f, 0.0f, 1.2f, 0.65f, 0.5f, 0.5f, 0.5f), // Plane
	MAKE_RECT(-0.0f, 0.2f, -0.0001f, 0.2f, 0.1f, 0.0f, 0.7f, 0.0f), // Building 1
	MAKE_RECT(-0.3f, 0.3f, -0.0001f, 0.1f, 0.2f, 0.0f, 0.7f, 0.0f), // Building 2
	MAKE_RECT(0.5f, 0.4f, -0.0001f, 0.1f, 0.15f, 0.0f, 0.7f, 0.0f), // Building 3
	MAKE_RECT(0.6f, 0.15f, -0.0001f, 0.15f, 0.15f, 0.0f, 0.7f, 0.0f), // Building 4
};

Map::Map() {
	buildings[0] = Building{ MAKE_RECT(0.2f, 0.25f, 0.0f, 1.2f, 0.65f, 0.5f, 0.5f, 0.5f) }; // Plane

	buildings[1] = Building{ MAKE_RECT(-0.0f, 0.2f, -0.0001f, 0.2f, 0.1f, 0.0f, 0.7f, 0.0f) }; // Building 1
	buildings[2] = Building{ MAKE_RECT(-0.3f, 0.3f, -0.0001f, 0.1f, 0.2f, 0.0f, 0.7f, 0.0f) }; // Building 2
	buildings[3] = Building{ MAKE_RECT(0.5f, 0.4f, -0.0001f, 0.1f, 0.15f, 0.0f, 0.7f, 0.0f) }; // Building 3
	buildings[4] = Building{ MAKE_RECT(0.6f, 0.15f, -0.0001f, 0.15f, 0.15f, 0.0f, 0.7f, 0.0f) }; // Building 4

	countBuildings = 5;
}

Map::~Map() {
}

void Map::markBuilding(unsigned int id) {
	assert(id < countBuildings);

	for (int i = 0; i < 6; i++) {
		buildings[id].tile[i][Building::Index::R] = 0.0f;
		buildings[id].tile[i][Building::Index::G] = 0.0f;
		buildings[id].tile[i][Building::Index::B] = 0.7f;
	}
}

void Map::unmarkBuilding(unsigned int id) {
	assert(id < countBuildings);

	for (int i = 0; i < 6; i++) {
		buildings[id].tile[i][Building::Index::R] = 0.0f;
		buildings[id].tile[i][Building::Index::G] = 0.7f;
		buildings[id].tile[i][Building::Index::B] = 0.0f;
	}
}

const void* Map::data() {
	//return buildings.data();
	auto a = &buildings;
	return a;
}

const int Map::size() {
	return countBuildings * sizeof(Map::Building::Tile);
	return sizeof(vertices);
}