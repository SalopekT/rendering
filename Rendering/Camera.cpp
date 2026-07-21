#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
Camera::Camera(glm::vec3 position, glm::vec3 center, glm::vec3 viewUpVector) : position(position), center(center), viewUpVector(viewUpVector) {}

glm::mat4 Camera::getLookAtMatrix() {
	return glm::lookAt(position, center, viewUpVector);
}

glm::vec3 Camera::getCameraPosition() {
	return this->position;
}

void Camera::moveCamera(int direction) {
	/*float radiusXZ = std::sqrt(position[0] * position[0] + position[2] * position[2]);
	float currentAngleXZ = std::atan2(this->position[2], this->position[0]);

	float radiusXY = std::sqrt(position[0] * position[0] + position[1] * position[1]);
	float currentAngleXY = std::atan2(this->position[1], this->position[0]);
	if (direction == 1) {//left
		currentAngleXZ += 0.01f;
		this->position[0] = radiusXZ * std::cos(currentAngleXZ);
		this->position[2] = radiusXZ * std::sin(currentAngleXZ);
	}
	else if (direction == 2){
		currentAngleXZ -= 0.01f;
		this->position[0] = radiusXZ * std::cos(currentAngleXZ);
		this->position[2] = radiusXZ * std::sin(currentAngleXZ);
	}
	else if (direction == 3) {
		currentAngleXY -= 0.01f;
		this->position[0] = radiusXY * std::cos(currentAngleXY);
		this->position[1] = radiusXY * std::sin(currentAngleXY);
	}
	else {
		currentAngleXY += 0.01f;
		this->position[0] = radiusXY * std::cos(currentAngleXY);
		this->position[1] = radiusXY * std::sin(currentAngleXY);
	}*/
	if (direction == 1) {//left
		this->position[0] -= 0.01f;
		this->center[0] -= 0.01f;
	}
	else if (direction == 2) {
		this->position[0] += 0.01f;
		this->center[0] += 0.01f;
	}
	else if (direction == 3) {
		this->position[1] -= 0.01f;
		this->center[1] -= 0.01f;
	}
	else if(direction == 4){
		this->position[1] += 0.01f;
		this->center[1] += 0.01f;
	}
	else if (direction == 5) {
		this->center[2] += 0.01f;
	}
	else if (direction == 6) {
		this->center[2] -= 0.01f;
	}
	else if (direction == 7) { 
		glm::vec3 viewDir = this->center - this->position;
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(0.1f), this->viewUpVector);
		viewDir = glm::vec3(rot * glm::vec4(viewDir, 0.0f));
		this->center = this->position + viewDir;
	}
	else if (direction == 8) {
		glm::vec3 viewDir = this->center - this->position;
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(-0.1f), this->viewUpVector);
		viewDir = glm::vec3(rot * glm::vec4(viewDir, 0.0f));
		this->center = this->position + viewDir;
	}

	
}
