#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <string>
#include "Mesh.hpp"
#include "Shader.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class Object {
private:
    std::string pathToModel;
    std::string pathToTexture;
    unsigned int textureId;
    std::vector<Mesh> meshes;

public:
    Object(const std::vector<Mesh>& meshes);
    Object(const std::string& pathToModel, const std::string& pathToTexture);
    Object copy(); //copy constructor
    ~Object() {}

    void print() const;
    float* getVerticesArray();
    Mesh& getMesh(int i);
    unsigned int generateTexture();

    void drawObject(ShaderProgramme* sp, glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projectionMat);
   
private:
    void processNode(aiNode* node, const aiScene* scene);
};

#endif


