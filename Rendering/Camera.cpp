#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera(glm::vec3 position, glm::vec3 center, glm::vec3 viewUpVector) : position(position), center(center), viewUpVector(viewUpVector) {}

glm::mat4 Camera::getLookAtMatrix() {
	return glm::lookAt(position, center, viewUpVector);
}