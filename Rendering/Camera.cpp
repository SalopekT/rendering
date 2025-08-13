#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
Camera::Camera(glm::vec3 position, glm::vec3 center, glm::vec3 viewUpVector) : position(position), center(center), viewUpVector(viewUpVector) {}

glm::mat4 Camera::getLookAtMatrix() {
	return glm::lookAt(position, center, viewUpVector);
}

glm::vec3 Camera::getCameraPosition() {
	return this->position;
}

void Camera::moveCamera(int direction) {
	float radiusXZ = std::sqrt(position[0] * position[0] + position[2] * position[2]);
	float currentAngleXZ = std::atan2(this->position[2], this->position[0]);

	float radiusXY = std::sqrt(position[0] * position[0] + position[1] * position[1]);
	float currentAngleXY = std::atan2(this->position[1], this->position[0]);
	if (direction == 1) {//left
		currentAngleXZ += 0.03f;
		this->position[0] = radiusXZ * std::cos(currentAngleXZ);
		this->position[2] = radiusXZ * std::sin(currentAngleXZ);
	}
	else if (direction == 2){
		currentAngleXZ -= 0.03f;
		this->position[0] = radiusXZ * std::cos(currentAngleXZ);
		this->position[2] = radiusXZ * std::sin(currentAngleXZ);
	}
	else if (direction == 3) {
		currentAngleXY -= 0.03f;
		this->position[0] = radiusXY * std::cos(currentAngleXY);
		this->position[1] = radiusXY * std::sin(currentAngleXY);
	}
	else {
		currentAngleXY += 0.03f;
		this->position[0] = radiusXY * std::cos(currentAngleXY);
		this->position[1] = radiusXY * std::sin(currentAngleXY);
	}
	
}
