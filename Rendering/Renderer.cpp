#include "Renderer.hpp"

void Renderer::renderScene() {
	std::vector<std::shared_ptr<Lightning>> lightsInScene = this->scene->getLights();
	int numLights = lightsInScene.size();
	std::vector<int> lightTypes;
	for (std::shared_ptr<Lightning> light : lightsInScene) {
		lightTypes.push_back(light->getType());
	}
	this->shaderProgramme->setUniformsFragmentShader(numLights,lightTypes.data(),);
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