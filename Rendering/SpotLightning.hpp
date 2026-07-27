#ifndef SPOTLIGHTNING_HPP
#define SPOTLIGHTNING_HPP

#include <glm/glm.hpp>
#include "Lightning.hpp"
class SpotLightning : public Lightning{
private:
	glm::vec3 direction;
	float cutoffAngle;

public:
	SpotLightning(glm::vec3 position, glm::vec3 ambientInt, glm::vec3 diffuseInt, glm::vec3 specularInt,
				glm::vec3 direction, float cutoffAngle);
	//glm::vec3 getIntensityInPoint(glm::vec3 pointPosition, glm::vec3 pointNormal, glm::vec3 kAmb, glm::vec3 kDif, glm::vec3 kSpec);
	glm::vec3 getDirection();
	float getCutoffAngle();
};


#endif