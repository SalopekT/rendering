#include "Renderer.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#define CHECK_GL(label) { GLenum e; while((e = glGetError()) != GL_NO_ERROR) std::cout << "GL error " << e << " at " << label << std::endl; };
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
	CHECK_GL("after setUniformsFragmentShader");
	for (int i=0;i<this->scene->getObjects().size();i++) {
		this->shaderProgramme->setUniformsVertexShader(glm::mat4(1.0f),
														this->scene->getCamera()->getLookAtMatrix(),
														this->scene->getCamera()->getProjectionMatrix());
		CHECK_GL("after setUniformsVertexShader");
		this->shaderProgramme->setObjectUniformsFragmentShader(this->scene->getObjects().at(i)->getMaterialCoeffs(), this->scene->getObjects().at(i)->hasTexture());
		CHECK_GL("after setObjectUniformsFragmentShader");
		this->scene->getObjects().at(i)->drawObject();
		CHECK_GL("after drawObject");
	}

}
void Renderer::renderSceneToTexture(glm::mat4 viewMat, glm::mat4 projectionMat) {

}