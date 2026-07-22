#include "Scene.hpp"



void Scene::addObject(std::shared_ptr<Object> obj) {
    objects.push_back(obj);
}
const std::vector<std::shared_ptr<Object>>& Scene::getObjects() const {
    return objects;
}

void Scene::addLight(std::shared_ptr<Lightning> light) {
    lightSources.push_back(light);
}
const std::vector<std::shared_ptr<Lightning>>& Scene::getLights() const {
    return lightSources;
}

void Scene::setCamera(std::shared_ptr<Camera> cam) {
    camera = cam;
}
std::shared_ptr<Camera> Scene::getCamera() const {
    return camera;
}