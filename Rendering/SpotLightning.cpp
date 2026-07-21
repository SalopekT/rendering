#include "SpotLightning.hpp"

SpotLightning::SpotLightning(glm::vec3 position, glm::vec3 ambientInt, glm::vec3 diffuseInt, glm::vec3 specularInt,
	glm::vec3 direction, float cutoffAngle) : Lightning(position,ambientInt,diffuseInt,specularInt),
												direction(direction), cutoffAngle(cutoffAngle) {}

