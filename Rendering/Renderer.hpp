#ifndef RENDERER_HPP
#define RENDERER_HPP

#include<vector>
#include "Scene.hpp"
class Renderer {
	private:
		std::shared_ptr<Scene> scene;
		std::shared_ptr<ShaderProgramme> shaderProgramme;

    public:
		Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<ShaderProgramme> shaderProgramme);
		void renderScene();
		void renderSceneToTexture(glm::mat4 viewMat, glm::mat4 projectionMat);

};

#endif