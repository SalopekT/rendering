#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>
class Camera {
	private:
		glm::vec3 position;
		glm::vec3 center; //center is direction in which the camera watches
		glm::vec3 viewUpVector;

	public:
		Camera(glm::vec3 position, glm::vec3 center, glm::vec3 viewUpVector);
		glm::mat4 getLookAtMatrix(); //viewMatrix
		virtual glm::mat4 getProjectionMatrix() = 0;
		
	
};


class PerspectiveCamera : public Camera {
	private:
		float fov; //in degrees
		int width;
		int height;
		float zNear;
		float zFar;

	public:
		PerspectiveCamera(glm::vec3 position, glm::vec3 center, glm::vec3 viewUpVector, float fov, int width, int height, float zNear, float zFar);
		glm::mat4 getProjectionMatrix() override;

};

#endif