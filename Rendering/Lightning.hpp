#ifndef LIGHTNING_HPP
#define LIGHTNING_HPP

#include <glm/glm.hpp>

class Lightning {
	private:
		glm::vec3 position;
		glm::vec3 ambientInt;
		glm::vec3 diffuseInt;
		glm::vec3 specularInt;

	public:
		Lightning(glm::vec3 position, glm::vec3 ambientInt, glm::vec3 diffuseInt, glm::vec3 specularInt);
		//glm::vec3 getIntensityInPoint(glm::vec3 pointPosition, glm::vec3 pointNormal, glm::vec3 kAmb, glm::vec3 kDif, glm::vec3 kSpec);
};


#endif