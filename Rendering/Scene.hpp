#ifndef SCENE_HPP
#define SCENE_HPP
#include <vector>
#include "Object.hpp"
#include "Lightning.hpp"
#include "Camera.hpp"

class Scene {
	private: 
		std::vector<std::shared_ptr<Object>> objects;

		std::vector<std::shared_ptr<Lightning>> lightSources;
        /*std::vector<float> lightTypes;
        std::vector<glm::vec3> lightPositions;*/

		std::shared_ptr<Camera> camera;

	public:
        void addObject(std::shared_ptr<Object> obj);
        const std::vector<std::shared_ptr<Object>>& getObjects() const;

        void addLight(std::shared_ptr<Lightning> light);
        const std::vector<std::shared_ptr<Lightning>>& getLights() const;

        void setCamera(std::shared_ptr<Camera> cam);
        std::shared_ptr<Camera> getCamera() const;
};

#endif