#include "Lightning.hpp"
#include <cmath> 
#include <numbers>   
Lightning::Lightning(int type, glm::vec3 position, glm::vec3 ambientInt, glm::vec3 diffuseInt, glm::vec3 specularInt) : position(position), ambientInt(ambientInt),
																								diffuseInt(diffuseInt), specularInt(specularInt),type(type), phi(0) {}


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
	return -this->position;
}
float Lightning::getCutoffAngle() {
	return 0.0;
}

void Lightning::moveLightInCircle() {
	float radius = 7.0f;
	double radians = this->phi * (3.14145f / 180.0);
	float x_curr = radius * std::cos(radians);
	float y_curr = radius* std::sin(radians);
	this->phi = std::fmod(this->phi + 0.1f, 360.0);
	this->position.x = x_curr;
	this->position.y = y_curr;
}