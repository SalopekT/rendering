#include "Renderer.hpp"

void Renderer::renderScene() {
	std::vector<std::shared_ptr<Lightning>> lightsInScene = this->scene->getLights();
	int numLights = lightsInScene.size();
	std::vector<int> lightTypes;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::mat3> lightIntensities;
	std::vector<glm::vec3> lightDirections;
	for (std::shared_ptr<Lightning> light : lightsInScene) {
		lightTypes.push_back(light->getType());
		lightPositions.push_back(light->getLightPosition());
		lightIntensities.push_back(light->getIntensitiesMatrix());
		lightDirections.push_back(light->);
	}
	this->shaderProgramme->setUniformsFragmentShader(numLights,lightTypes.data(),lightPositions.data(),lightIntensities.data(),
													);
	for (int i=0;i<this->scene->getObjects().size();i++) {
		this->shaderProgramme->setUniformsVertexShader(this->scene->getObjects().at(i)->getTransform().getModelMatrix(),
														this->scene->getCamera()->getLookAtMatrix(),
														this->scene->getCamera()->getProjectionMatrix());
		
		/*int* typeLights, glm::vec3* lightPosition, glm::mat3* lightIntensities,
			glm::vec3* lightDirections, float* lightCutoffAngles, glm::vec3 materialLightCoefs, glm::vec3 eyePosition,
			bool hasTexture*/
		this->scene->getObjects().at(i)->drawObject();
	}

}
void Renderer::renderSceneToTexture(glm::mat4 viewMat, glm::mat4 projectionMat) {

}