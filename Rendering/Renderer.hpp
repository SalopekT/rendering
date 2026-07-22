#ifndef RENDERER_HPP
#define RENDERER_HPP

#include<vector>
#include "Scene.hpp"
class Renderer {
	private:
		std::shared_ptr<Scene> scene;

    public:
		void renderScene();
		void renderSceneToTexture();

};

#endif