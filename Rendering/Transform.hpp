#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include <glm/glm.hpp>
class Transform {
private:
	glm::vec3 position;
	glm::vec3 rotationEuler; //yaw,pitch,roll
	glm::vec3 scale{1.0f};

public:
	glm::mat4 getModelMatrix();
};



#endif