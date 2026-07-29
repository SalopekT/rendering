#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <string>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class Object {
private:
    std::string pathToModel;
    std::string pathToTexture;
    unsigned int textureId;
    std::vector<Mesh> meshes;
    Transform transform;
    glm::vec3 materialCoeffs;
    bool hasTextureFlag;

public:
    Object(const std::vector<Mesh>& meshes);
    Object(const std::string& pathToModel, const std::string& pathToTexture);
    Object copy(); //copy constructor
    ~Object() {}

    void print() const;
    float* getVerticesArray();
    Mesh& getMesh(int i);
    unsigned int generateTexture();
    Transform& getTransform();
    glm::vec3 getMaterialCoeffs();
    bool hasTexture();
    void setTextureFlag(bool t);
    void setTransform();
    void setMaterialCoeffs(glm::vec3 materialCoeffs);

    void drawObject();
   
private:
    void processNode(aiNode* node, const aiScene* scene);
};

#endif


