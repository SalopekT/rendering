#include "SpotLightning.hpp"

SpotLightning::SpotLightning(glm::vec3 position, glm::vec3 ambientInt, glm::vec3 diffuseInt, glm::vec3 specularInt,
	glm::vec3 direction, float cutoffAngle) : Lightning(2,position,ambientInt,diffuseInt,specularInt),
												direction(direction), cutoffAngle(cutoffAngle) {}

glm::vec3 SpotLightning::getDirection() {
	return this->direction;
}
float SpotLightning::getCutoffAngle() {
	return this->cutoffAngle;
}

void SpotLightning::moveLightInCircle() {
	Lightning::moveLightInCircle();
	this->direction = -this->getLightPosition();

}