#include "Object.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Object::Object(const std::string& pathName) {

    // https://learnopengl.com/Model-Loading/Model
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(pathName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    this->processNode(scene->mRootNode, scene);

}

void Object::processNode(aiNode* node, const aiScene* scene) {
    //node->mMeshes contains indices(integers) of meshes in scene array of meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(Mesh::processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


Object Object::copy() {
    return *this;
}

void Object::print() const {
    for (int i=0;i<this->meshes.size();i++) {
        std::cout << "Mesh n: " << i << "\n";
        this->meshes.at(i).print();
    }

}

Mesh Object::getMesh(int i) {
    return this->meshes.at(i);
}


float* Object::getVerticesArray() {
    //float* arr = new float[this->vertices.size() * 3];
}