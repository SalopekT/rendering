#include "Lightning.hpp"


Lightning::Lightning(int type, glm::vec3 position, glm::vec3 ambientInt, glm::vec3 diffuseInt, glm::vec3 specularInt) : position(position), ambientInt(ambientInt),
																								diffuseInt(diffuseInt), specularInt(specularInt),type(type) {}


glm::vec3 Lightning::getLightPosition() {
	return this->position;
}

glm::mat3 Lightning::getIntensitiesMatrix() {
	glm::mat3 result;
	result[0] = this->ambientInt;
	result[1] = this->diffuseInt;
	result[2] = this->specularInt;

	return result;

}

int Lightning::getType() {
	return this->type;
}

//these two are only for spotlights (for now) but here is default implementation
glm::vec3 Lightning::getDirection() {
	return glm::vec3(0.0f);
}
float Lightning::getCutoffAngle() {
	return 0.0;
}