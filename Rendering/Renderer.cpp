#include "Renderer.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
Renderer::Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<ShaderProgramme> shaderProgramme) : scene(scene), shaderProgramme(shaderProgramme) {}

void Renderer::renderScene() {
	std::vector<std::shared_ptr<Lightning>> lightsInScene = this->scene->getLights();
	int numLights = lightsInScene.size();
	std::vector<int> lightTypes;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::mat3> lightIntensities;
	std::vector<glm::vec3> lightDirections;
	std::vector<float> lightCutoffAngles;
	std::shared_ptr<Camera> camera = this->scene->getCamera();
	for (std::shared_ptr<Lightning> light : lightsInScene) {
		lightTypes.push_back(light->getType());
		lightPositions.push_back(light->getLightPosition());
		lightIntensities.push_back(light->getIntensitiesMatrix());
		lightDirections.push_back(light->getDirection());
		lightCutoffAngles.push_back(light->getCutoffAngle());
	}
	this->shaderProgramme->setUniformsFragmentShader(numLights,lightTypes.data(),lightPositions.data(),lightIntensities.data(),
													lightDirections.data(), lightCutoffAngles.data(), camera->getCameraPosition());
	for (int i=0;i<this->scene->getObjects().size();i++) {
		this->shaderProgramme->setUniformsVertexShader(this->scene->getObjects().at(i)->getTransform().getModelMatrix(),
														this->scene->getCamera()->getLookAtMatrix(),
														this->scene->getCamera()->getProjectionMatrix());
		this->shaderProgramme->setObjectUniformsFragmentShader(this->scene->getObjects().at(i)->getMaterialCoeffs(), this->scene->getObjects().at(i)->hasTexture());
		this->scene->getObjects().at(i)->drawObject();
	}

}
void Renderer::renderSceneToTexture() {
	//for now i will enable shadow mapping with only 1 spotlight
	std::shared_ptr<Lightning> firstLight = this->scene->getLights().at(0);
	float innerConeAngleCosine = firstLight->getCutoffAngle();
	float fov = 2 * innerConeAngleCosine;
	float aspectRatio = 1.0f;
	float zNear = 0.01f;
	float zFar = 100.0f;
	glm::mat4 lightProjectionMat = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
	glm::mat4 lightViewMat = glm::lookAt(firstLight->getLightPosition(),
		firstLight->getLightPosition() + firstLight->getDirection(), glm::vec3{ 0.0f,0.0f,1.0f });

	glm::mat4 lightSpaceMatrix = lightProjectionMat * lightViewMat;
	for (int i = 0;i < this->scene->getObjects().size();i++) {
		this->shaderProgramme->setUniformsShadowShader(lightSpaceMatrix, this->scene->getObjects().at(i)->getTransform().getModelMatrix());
	}


}