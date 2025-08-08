#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 center, glm::vec3 viewUpVector,
	float fov, int width, int height, float zNear, float zFar) : Camera(position, center, viewUpVector),
	fov(fov), width(width), height(height),
	zNear(zNear), zFar(zFar) {}


glm::mat4 PerspectiveCamera::getProjectionMatrix(){
	return glm::perspective(glm::radians(fov), (float)width / (float)height, zNear, zFar);
}