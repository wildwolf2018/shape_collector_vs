#include <iostream>
#include "physics.h"

bool Physics::pointPlaneIntersect(glm::vec3 playerPosition, std::map<std::string, float> *plane)
{
   //std::cout << "x = " << playerPosition.x << ", y = " << playerPosition.y << ", z = " << playerPosition.z << std::endl;
	return ((playerPosition.x >= (plane -> at("min_x") - 1.0f)) && (playerPosition.x <= (plane -> at("max_x") + 1.0f))) &&
			((playerPosition.y >= (plane -> at("min_y") - 1.0f)) && (playerPosition.y <= (plane -> at("max_y") + 1.0f))) &&
			((playerPosition.z >= (plane -> at("min_z") - 1.0f)) && (playerPosition.z <= (plane -> at("max_z") + 1.0f)));
}