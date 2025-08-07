#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <string>
#include "Mesh.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class Object {
private:
    std::vector<Mesh> meshes;

public:
    Object(const std::string&);
    Object copy(); //copy constructor
    ~Object() {}

    void print() const;
   
private:
    void processNode(aiNode* node, const aiScene* scene);
};

#endif


