#ifndef H_PHYSICS
#define H_PHYSICS

#include <glm/glm.hpp>
#include <map>

class Physics
{
    public:
		//calculates intersection between a point and a plane
        static bool pointPlaneIntersect(glm::vec3 point, std::map<std::string, float> *plane);
};
#endif 