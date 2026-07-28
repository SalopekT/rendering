#include "Transform.hpp"
#include <glm/gtc/quaternion.hpp>
Transform::Transform()
	: position(0.0f),
	rotationEuler(0.0f),
	scale(1.0f)
{
}
glm::mat4 Transform::getModelMatrix() {
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), this->position);
	glm::quat q = glm::quat(glm::vec3(glm::radians(rotationEuler[0]),
		glm::radians(rotationEuler[1]),
		glm::radians(rotationEuler[2])));
	glm::mat4 rotation = glm::mat4_cast(q);
	//this rotation is rotation in object's own coordinate space
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
	glm::mat4 model = translation * rotation * scale;
	return model;
}
