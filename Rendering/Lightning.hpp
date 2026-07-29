#ifndef LIGHTNING_HPP
#define LIGHTNING_HPP

#include <glm/glm.hpp>

class Lightning {
	private:
		int type;
		glm::vec3 position;
		glm::vec3 ambientInt;
		glm::vec3 diffuseInt;
		glm::vec3 specularInt;

	public:
		Lightning(int type, glm::vec3 position, glm::vec3 ambientInt, glm::vec3 diffuseInt, glm::vec3 specularInt);
		//glm::vec3 getIntensityInPoint(glm::vec3 pointPosition, glm::vec3 pointNormal, glm::vec3 kAmb, glm::vec3 kDif, glm::vec3 kSpec);
		glm::vec3 getLightPosition();
		glm::mat3 getIntensitiesMatrix();
		int getType();
		virtual glm::vec3 getDirection();
		virtual float getCutoffAngle();
};


#endif