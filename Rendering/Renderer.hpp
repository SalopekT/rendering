#ifndef RENDERER_HPP
#define RENDERER_HPP

#include<vector>
#include "Scene.hpp"
class Renderer {
	private:
		std::shared_ptr<Scene> scene;
		std::shared_ptr<ShaderProgramme> shaderProgramme;
		std::shared_ptr<ShaderProgramme> shaderProgrammeShadow;

    public:
		Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<ShaderProgramme> shaderProgramme, 
					std::shared_ptr<ShaderProgramme> shaderProgrammeShadow);
		void renderScene();
		void renderSceneToTexture();

};

#endif